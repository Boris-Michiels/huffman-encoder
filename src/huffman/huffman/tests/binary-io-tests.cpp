#ifdef TEST_BUILD

#include "catch.hpp"

#include "io/binary-io.h"
#include "io/memory-buffer.h"


void check(u64 n, unsigned nbits)
{
    io::MemoryBuffer<2> buffer;
    auto input = buffer.source()->create_input_stream();
    auto output = buffer.destination()->create_output_stream();
    io::write_bits(n, nbits, *output);
    auto result = io::read_bits(nbits, *input);

    REQUIRE(n == result);
}

TEST_CASE("Binary IO tests") {
    check(0, 8);
    check(1, 8);
    check(2, 8);
    check(127, 8);
    check(128, 8);
    check(256, 9);
}

#endif