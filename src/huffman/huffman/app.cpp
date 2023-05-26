#ifndef TEST_BUILD

/*
*   This file is only active in non-test builds (i.e., builds where TEST_BUILD is not defined).
*/
#include "encoding/encoding.h"
#include "encoding/huffman/huffman-encoding.h"
#include "encoding/bit-grouper.h"
#include "encoding/encoding-combiner.h"
#include "encoding/eof-encoding.h"
#include "io/files.h"

#include <iostream>


void encode(const std::string& file_1, const std::string& file_2, encoding::Encoding<256, 256> encoding) {
    std::cout << "Encoding" << file_1 << std::endl;
    encoding::encode(io::create_file_data_source(file_1), encoding, io::create_file_data_destination(file_2));
    std::cout << "File " << file_1 << " encoded successfully to " << file_2 << "!" << std::endl;
}

void decode(const std::string& file_1, const std::string& file_2, encoding::Encoding<256, 256> encoding) {
    std::cout << "Decoding..." << std::endl;
    encoding::decode(io::create_file_data_source(file_1), encoding, io::create_file_data_destination(file_2));
    std::cout << "File " << file_1 << " decoded successfully to " << file_2 << "!" << std::endl;
}

/*
* This is the main entry point of your application.
* Note that it will only work in non-test builds.
*/

int main(const int argc, char* argv[])
{
#ifdef NDEBUG
    std::cout << "You are running the release build" << std::endl;
#else
    std::cout << "You are running the debug build" << std::endl;
#endif

    std::string wrong_arguments = "Wrong number of arguments. Arguments are: method [encode, decode, encdec], input_file, output_file.";

    if (argc != 4) {
        std::cout << wrong_arguments << std::endl;
        return -1;
    }

    const std::string method(argv[1]);
    const std::string file_1(argv[2]);
    const std::string file_2(argv[3]);

    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();
    const auto combined = eof | huffman | grouper;

    if (method == "encode") {
        encode(file_1, file_2, combined);
    }
    else if (method == "decode") {
        decode(file_1, file_2, combined);
    }
    else if (method == "encdec") {
        auto encoded_file = "huffman_encoded.txt";
        encode(file_1, encoded_file, combined);
        decode(encoded_file, file_2, combined);
    }
    else {
        std::cout << wrong_arguments << std::endl;
        return -1;
    }

    return 0;
}

#endif