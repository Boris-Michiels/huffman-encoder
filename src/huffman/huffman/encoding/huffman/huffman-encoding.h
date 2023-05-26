#ifndef HUFFMAN_ENCODING_H
#define HUFFMAN_ENCODING_H

#include "util.h"
#include "data/frequency-table.h"
#include "encoding/encoding.h"
#include "encoding/huffman/tree-encoding.h"
#include "data/binary-tree.h"
#include "io/streams.h"
#include "io/binary-io.h"

#include <vector>
#include <list>
#include <map>


namespace encoding {
	namespace huffman {
		std::vector<Datum> copy_to_vector(io::InputStream&);
		std::unique_ptr<data::Node<std::pair<Datum, unsigned>>> build_tree(const data::FrequencyTable<Datum>&);
		unsigned weight(const data::Node<std::pair<Datum, unsigned>>&);
		std::unique_ptr < std::map<Datum, std::vector<Datum>>> build_codes(const data::Node<Datum>&);
		void build_codes(std::map<Datum, std::vector<Datum>>&, const data::Node<Datum>&, std::vector<Datum>);
		Datum decode_single_datum(io::InputStream&, const data::Node<Datum>&);
		void decode_bits(io::InputStream&, const data::Node<Datum>&, io::OutputStream&);
	}

	std::shared_ptr<encoding::EncodingImplementation> create_huffman_implementation(u64);

	template<u64 N>
	encoding::Encoding<N, 2> huffman_encoding() {
		return Encoding<N, 2>(create_huffman_implementation(N));
	}
}

#endif