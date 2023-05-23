#include "encoding/eof-encoding.h"
#include "encoding/encoding.h"


namespace {
	class EofEncodingImplementation : public encoding::EncodingImplementation {
		const u64 _domain_size;

	public:
		EofEncodingImplementation(const u64 domain_size) : _domain_size(domain_size) {}

		void encode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			while (!input_stream.end_reached()) output_stream.write(input_stream.read());
			output_stream.write(_domain_size);
		}

		void decode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			const u64 eof = _domain_size;

			Datum datum = input_stream.read();

			while (datum != eof) {
				output_stream.write(datum);
				datum = input_stream.read();
			}
		}
	};
}

std::shared_ptr<encoding::EncodingImplementation> encoding::create_eof_implementation(u64 domain_size) {
	return std::make_shared<EofEncodingImplementation>(domain_size);
}