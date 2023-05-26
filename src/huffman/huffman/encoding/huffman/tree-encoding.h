#ifndef HUFFMAN_TREE_ENCODING_H
#define HUFFMAN_TREE_ENCODING_H

#include "util.h"
#include "data/binary-tree.h"
#include "io/streams.h"
#include "io/binary-io.h"


namespace encoding {
	namespace huffman {
		void encode_tree(const data::Node<Datum>&, const unsigned, io::OutputStream&);
		std::unique_ptr<data::Node<Datum>> decode_tree(const unsigned, io::InputStream&);
	}
}

#endif