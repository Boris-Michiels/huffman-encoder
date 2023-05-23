#include "encoding/bit-grouper.h"
#include "encoding/encoding.h"
#include "io/binary-io.h"


namespace {
	class BitGrouperImplementation : public encoding::EncodingImplementation {
		unsigned _group_size;

	public:
		BitGrouperImplementation(unsigned group_size) : _group_size(group_size) {}

		void encode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			while (!input_stream.end_reached()) output_stream.write(io::read_bits(_group_size, input_stream));
		}

		void decode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			while (!input_stream.end_reached()) io::write_bits(input_stream.read(), _group_size, output_stream);
		}
	};
}

std::shared_ptr<encoding::EncodingImplementation> encoding::create_bit_grouper_implementation(u64 group_size) {
	return std::make_shared<BitGrouperImplementation>(group_size);
}