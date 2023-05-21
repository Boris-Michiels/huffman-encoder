#ifdef TEST_BUILD

#include "catch.hpp"
#include "data/binary-tree.h"

#include <iostream>


TEST_CASE("Branch & Leaf tests") {
    auto left_leaf = std::make_unique<data::Leaf<char>>('a');
    auto right_leaf = std::make_unique<data::Leaf<char>>('b');

    data::Branch<char> root(std::move(left_leaf), std::move(right_leaf));
    auto& left_value = dynamic_cast<const data::Leaf<char>&>(root.left_child());
    auto& right_value = dynamic_cast<const data::Leaf<char>&>(root.right_child());

    REQUIRE(left_value.value() == 'a');
    REQUIRE(right_value.value() == 'b');
}

#endif