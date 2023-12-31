#include "encoding/huffman/tree-encoding.h"
#include "io/binary-io.h"


void encoding::huffman::encode_tree(const data::Node<Datum>& root, const unsigned nbits, io::OutputStream& output_stream) {
	if (root.is_leaf()) {
		const auto& leaf = static_cast<const data::Leaf<Datum>&>(root);
		output_stream.write(0);
		io::write_bits(leaf.value(), nbits, output_stream);
	}
	else {
		const auto& branch = static_cast<const data::Branch<Datum>&>(root);
		output_stream.write(1);
		encode_tree(branch.left_child(), nbits, output_stream);
		encode_tree(branch.right_child(), nbits, output_stream);
	}
}

std::unique_ptr<data::Node<Datum>> encoding::huffman::decode_tree(const unsigned nbits, io::InputStream& input_stream) {
	if (input_stream.read() == 0) {
		return std::make_unique<data::Leaf<Datum>>(io::read_bits(nbits, input_stream));
	}
	else {
		std::unique_ptr<data::Node<Datum>> left_child = decode_tree(nbits, input_stream);
		std::unique_ptr<data::Node<Datum>> right_child = decode_tree(nbits, input_stream);
		return std::make_unique<data::Branch<Datum>>(std::move(left_child), std::move(right_child));
	}
}