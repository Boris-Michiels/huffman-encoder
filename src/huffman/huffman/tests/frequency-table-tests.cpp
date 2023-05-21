#ifdef TEST_BUILD

#include "catch.hpp"
#include "data/frequency-table.h"

#include <iostream>


TEST_CASE("FrequencyTable testing") {
    // Create new frequency table to count chars
    data::FrequencyTable<char> freqtable;

    // Count 'a' one time
    freqtable.increment('a');

    // Count 'b' twice
    freqtable.increment('b');
    freqtable.increment('b');

    // Query frequencies
    REQUIRE(freqtable['a'] == 1);  // -> 1
    REQUIRE(freqtable['b'] == 2);  // -> 2
    REQUIRE(freqtable['c'] == 0);  // -> 0

    // Ask for list of values
    std::vector<char> letters = freqtable.values(); // -> { 'a', 'b' }
    REQUIRE(letters == std::vector<char>({ 'a', 'b' }));
}

#endif