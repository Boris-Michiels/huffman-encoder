#ifdef TEST_BUILD

#include "catch.hpp"
#include "util.h"


TEST_CASE("bits_needed tests") {
    REQUIRE(bits_needed(2) == 1);
    REQUIRE(bits_needed(3) == 2);
    REQUIRE(bits_needed(4) == 2);
    REQUIRE(bits_needed(5) == 3);
    REQUIRE(bits_needed(6) == 3);
    REQUIRE(bits_needed(7) == 3);
    REQUIRE(bits_needed(8) == 3);
    REQUIRE(bits_needed(9) == 4);
    REQUIRE(bits_needed(128) == 7);
    REQUIRE(bits_needed(129) == 8);
    REQUIRE(bits_needed(1000000) == 20);
}

TEST_CASE("bytes_needed tests") {
    REQUIRE(bytes_needed(2) == 1);
    REQUIRE(bytes_needed(3) == 1);
    REQUIRE(bytes_needed(128) == 1);
    REQUIRE(bytes_needed(129) == 1);
    REQUIRE(bytes_needed(255) == 1);
    REQUIRE(bytes_needed(256) == 1);
    REQUIRE(bytes_needed(1000000) == 3);
}

#endif