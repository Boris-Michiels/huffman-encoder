#ifdef TEST_BUILD

#include "catch.hpp"
#include "data/frequency-table.h"
#include "encoding/bit-grouper.h"
#include "encoding/eof-encoding.h"
#include "encoding/encoding.h"
#include "encoding/huffman/huffman-encoding.h"
#include "io/memory-buffer.h"
#include "encoding/encoding-combiner.h"


void encode_decode_compare(std::string string) {
    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;

    io::MemoryBuffer<256> input;
    for (auto letter : string)
    {
        input.data()->push_back(letter);
    }
    io::MemoryBuffer<256> enc;
    io::MemoryBuffer<256> dec;

    encoding::encode(input.source(), combined, enc.destination());

    encoding::decode(enc.source(), combined, dec.destination());

    REQUIRE(*input.data() == *dec.data());
}

TEST_CASE("Huffman Encoding tests") {
    encode_decode_compare("test");
    encode_decode_compare("test \n bababa");
    encode_decode_compare("azertyuiopqsdfghjklmwxcvbn,;:=       &é\"'\"(ç!ueff5");
    encode_decode_compare("sqeeeeeeeeeeefffffffffffff");
}

#endif