#include "encoding/huffman/huffman-encoding.h"
#include "encoding/huffman/tree-encoding.h"
#include "io/binary-io.h"

#include <vector>
#include <algorithm>
#include <iostream>


namespace {
	class HuffmanEncodingImplementation : public encoding::EncodingImplementation {
		u64 _domain_size;

	public:
		HuffmanEncodingImplementation(u64 domain_size) : _domain_size(domain_size) {}

		void encode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			auto data = encoding::huffman::copy_to_vector(input_stream);
			auto freq_table = data::count_frequencies(data);
			const auto weighted_tree = encoding::huffman::build_tree(freq_table);
			const auto tree = data::map<std::pair<Datum, unsigned>, Datum>(*weighted_tree, [](std::pair<Datum, unsigned> pair) { return pair.first; });
			const auto codes = encoding::huffman::build_codes(*tree, _domain_size);
			encoding::huffman::encode_tree(*tree, bits_needed(_domain_size), output_stream);
			
			for (auto datum : data) {
				auto& bits = codes->at(datum);

				for (auto bit : bits) {
					output_stream.write(bit);
				}
			}
		}

		void decode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			auto tree = encoding::huffman::decode_tree(bits_needed(_domain_size), input_stream);
			encoding::huffman::decode_bits(input_stream, *tree, output_stream);
		}
	};
}

std::shared_ptr<encoding::EncodingImplementation> encoding::create_huffman_implementation(u64 domain_size) {
	return std::make_shared<HuffmanEncodingImplementation>(domain_size);
}

std::vector<Datum> encoding::huffman::copy_to_vector(io::InputStream& input_stream) {
	std::vector<Datum> data;
	while (!input_stream.end_reached()) data.push_back(input_stream.read());
	return data;
}

std::unique_ptr<data::Node<std::pair<Datum, unsigned>>> encoding::huffman::build_tree(const data::FrequencyTable<Datum>& freq_table) {
	std::vector<std::unique_ptr<data::Node<std::pair<Datum, unsigned>>>> nodes;
	nodes.reserve(freq_table.frequency_map().size());

	for (auto& pair : freq_table.frequency_map()) {
		nodes.push_back(std::make_unique<data::Leaf<std::pair<Datum, unsigned>>>(pair));
	}

	auto nodes_comparator = [](const std::unique_ptr<data::Node<std::pair<Datum, unsigned>>>& a, const std::unique_ptr<data::Node<std::pair<Datum, unsigned>>>& b) {
		return encoding::huffman::weight(*a) > encoding::huffman::weight(*b);
	};
	std::sort(nodes.begin(), nodes.end(), nodes_comparator);

	while (nodes.size() != 1) {
		auto node2 = std::move(nodes.back());
		nodes.pop_back();
		auto node1 = std::move(nodes.back());
		nodes.pop_back();

		auto branch = std::make_unique<data::Branch<std::pair<Datum, unsigned>>>(std::move(node1), std::move(node2));

		unsigned weight = encoding::huffman::weight(*branch);
		auto insert_location = std::find_if(nodes.begin(), nodes.end(), [weight](auto& c) { return encoding::huffman::weight(*c) <= weight; });
		nodes.insert(insert_location, std::move(branch));
	}

	return std::move(nodes.front());
}

unsigned encoding::huffman::weight(const data::Node<std::pair<Datum, unsigned>>& node) {
	unsigned weight;

	if (node.is_leaf()) {
		const auto& leaf = static_cast<const data::Leaf<std::pair<Datum, unsigned>>&>(node);
		weight = leaf.value().second;
	}
	else {
		const auto& branch = static_cast<const data::Branch<std::pair<Datum, unsigned>>&>(node);
		weight = encoding::huffman::weight(branch.left_child()) + encoding::huffman::weight(branch.right_child());
	}

	return weight;
}

std::unique_ptr<std::vector<std::vector<Datum>>> encoding::huffman::build_codes(const data::Node<Datum>& tree, u64 domain_size) {
	std::unique_ptr<std::vector<std::vector<Datum>>> codes = std::make_unique<std::vector<std::vector<Datum>>>(domain_size);
	encoding::huffman::build_codes(*codes, tree, std::vector<Datum>());
	return codes;
}

void encoding::huffman::build_codes(std::vector<std::vector<Datum>>& codes, const data::Node<Datum>& node, std::vector<Datum> code) {
	if (node.is_leaf()) {
		auto& leaf = static_cast<const data::Leaf<Datum>&>(node);
		codes[leaf.value()] = code;
	}
	else {
		auto& branch = static_cast<const data::Branch<Datum>&>(node);
		code.push_back(0);
		encoding::huffman::build_codes(codes, branch.left_child(), code);

		code.back() = 1;
		encoding::huffman::build_codes(codes, branch.right_child(), code);
	}
}

Datum encoding::huffman::decode_single_datum(io::InputStream& input_stream, const data::Node<Datum>& tree) {
	const data::Node<Datum>* node_ptr = &tree;

	while (!input_stream.end_reached()) {
		auto current_node = static_cast<const data::Branch<Datum>*>(node_ptr);

		if (input_stream.read() == 0) {
			node_ptr = &(current_node->left_child());
		}
		else {
			node_ptr = &(current_node->right_child());
		}

		if (node_ptr->is_leaf()) {
			auto leaf = static_cast<const data::Leaf<Datum>*>(node_ptr);
			return leaf->value();
		}
	}

	return 0;
}

void encoding::huffman::decode_bits(io::InputStream& input_stream, const data::Node<Datum>& tree, io::OutputStream& output_stream) {
	while (!input_stream.end_reached()) {
		output_stream.write(encoding::huffman::decode_single_datum(input_stream, tree));
	}
}