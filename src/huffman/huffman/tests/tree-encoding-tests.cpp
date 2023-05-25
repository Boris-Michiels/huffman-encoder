#ifdef TEST_BUILD

#include "catch.hpp"

#include "util.h"
#include "data/binary-tree.h"
#include "encoding/huffman/tree-encoding.h"
#include "io/memory-buffer.h"


TEST_CASE("Tree Encoding tests") {
    auto leaf_1= std::make_unique<data::Leaf<Datum>>(1);
    auto leaf_2= std::make_unique<data::Leaf<Datum>>(11);
    auto leaf_3 = std::make_unique<data::Leaf<Datum>>(10);
    auto branch_1 = std::make_unique<data::Branch<Datum>>(std::move(leaf_2), std::move(leaf_3));
    const auto root = data::Branch<Datum>(std::move(leaf_1), std::move(branch_1));

    auto buffer2 = io::MemoryBuffer<2>();
    encoding::huffman::encode_tree(root, 2, *(buffer2.destination()->create_output_stream()));
    auto root2 = encoding::huffman::decode_tree(2, *(buffer2.source()->create_input_stream()));
    const data::Branch<Datum>* decoded = dynamic_cast<const data::Branch<Datum>*>(root2.get());
    REQUIRE(!decoded->is_leaf());
    auto leaf_12 = dynamic_cast<const data::Leaf<Datum>*>(&decoded->left_child());
    REQUIRE(leaf_12->value() == 1);
}

#endif