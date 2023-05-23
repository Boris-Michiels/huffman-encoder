#ifdef TEST_BUILD

#include "catch.hpp"

#include "encoding/eof-encoding.h"
#include "encoding/encoding.h"
#include "io/memory-buffer.h"


TEST_CASE("EOF tests") {
    io::MemoryBuffer<17> buffer17;
    io::MemoryBuffer<18> buffer18;

    buffer17.data()->push_back(0);
    buffer17.data()->push_back(1);
    buffer17.data()->push_back(2);

    const auto eof_encoding = encoding::eof_encoding<17>();
    encoding::encode(buffer17.source(), eof_encoding, buffer18.destination());

    REQUIRE((buffer17.data()->size() == 3 && buffer18.data()->size() == 4));
    REQUIRE((*buffer18.data())[3] == 17);

    /*io::MemoryBuffer<17> empty;
    encoding::encode(buffer18.source(), eof_encoding, empty.destination());*/
}

#endif