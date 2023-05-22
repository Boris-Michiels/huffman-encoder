#ifndef FILE_OUTPUT_STREAM_H
#define FILE_OUTPUT_STREAM_H

#include "io/streams.h"
#include "io/data-endpoints.h"
#include <cstdint>
#include <memory>
#include <string>


namespace io {
    std::unique_ptr<io::InputStream> create_file_input_stream(const std::string& path);
    std::unique_ptr<io::OutputStream> create_file_output_stream(const std::string& path);

    io::DataSource<256> create_file_data_source(const std::string& path);
    io::DataDestination<256> create_file_data_destination(const std::string& path);
}

#endif