#ifdef TEST_BUILD

#include "catch.hpp"

#include "encoding/bit-grouper.h"
#include "encoding/encoding.h"
#include "io/memory-buffer.h"


TEST_CASE("Bit Grouper tests") {
    io::MemoryBuffer<2> buffer2;
    // Waarom worden de Data geinterpreteerd als letters als de group_size groter is dan 4?
    // Omdat de achterliggende integer size dan 8 bit is. En dus als ascii wordt geinterpreteerd denk ik.
    io::MemoryBuffer<16> buffer16;

    buffer2.data()->push_back(1);
    buffer2.data()->push_back(0);
    buffer2.data()->push_back(0);
    buffer2.data()->push_back(1);
    buffer2.data()->push_back(1);

    const auto bit_grouper = encoding::bit_grouper<4>();
    encoding::encode(buffer2.source(), bit_grouper, buffer16.destination());

    REQUIRE(buffer16.data()->front() == 9);
    REQUIRE(buffer16.data()->at(1) == 8);
}

#endif