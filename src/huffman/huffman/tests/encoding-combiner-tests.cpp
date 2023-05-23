#ifdef TEST_BUILD

#include "catch.hpp"

#include "encoding/eof-encoding.h"
#include "encoding/encoding-combiner.h"
#include "io/memory-buffer.h"


TEST_CASE("Encoding Combiner tests") {
    const auto combiner = encoding::eof_encoding<17>() | encoding::eof_encoding<18>();

    io::MemoryBuffer<17> buffer17;
    io::MemoryBuffer<19> buffer19;

    buffer17.data()->push_back(0);
    buffer17.data()->push_back(1);
    buffer17.data()->push_back(2);

    encoding::encode(buffer17.source(), combiner, buffer19.destination());

    io::MemoryBuffer<17> empty;

    encoding::decode(buffer19.source(), combiner, empty.destination());

    REQUIRE(*buffer17.data() == *empty.data());
}

#endif