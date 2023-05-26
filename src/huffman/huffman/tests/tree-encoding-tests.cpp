#ifdef TEST_BUILD

#include "catch.hpp"

#include "util.h"
#include "data/binary-tree.h"
#include "encoding/huffman/tree-encoding.h"
#include "encoding/huffman/huffman-encoding.h"
#include "io/memory-buffer.h"
#include "data/frequency-table.h"


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

TEST_CASE("building tree test 3") {
    std::vector<Datum> input = { 'B', 'A', 'C', 'A', 'B', 'A', 'A' };
    auto freq_table = std::make_unique<data::FrequencyTable<Datum>>(data::count_frequencies(input));

    auto tree = std::move(encoding::huffman::build_tree(*freq_table));

    const data::Branch<std::pair<Datum, unsigned int>>& tree_root = dynamic_cast<const data::Branch<std::pair<Datum, unsigned int>>&>(*(tree.get()));
    REQUIRE(encoding::huffman::weight(tree_root) == 7);

    auto branch1 = dynamic_cast<const data::Branch<std::pair<Datum, unsigned int>>*>(&(tree_root).right_child());
    assert(&branch1 != nullptr);
    REQUIRE(encoding::huffman::weight(*branch1) == 3);

    auto L1 = dynamic_cast<const data::Leaf<std::pair<Datum, unsigned int>>*>(&(tree_root).left_child());
    assert(&L1 != nullptr);
    REQUIRE(L1->value().first == 65);
    REQUIRE(L1->value().second == 4);

    auto L2 = dynamic_cast<const data::Leaf<std::pair<Datum, unsigned int>>*>(&(branch1->right_child()));
    assert(&L2 != nullptr);
    REQUIRE(L2->value().first == 67);
    REQUIRE(L2->value().second == 1);

    auto L3 = dynamic_cast<const data::Leaf<std::pair<Datum, unsigned int>>*>(&(branch1->left_child()));
    assert(&L3 != nullptr);
    REQUIRE(L3->value().first == 66);
    REQUIRE(L3->value().second == 2);
}

#endif