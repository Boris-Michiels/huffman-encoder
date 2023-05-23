#ifndef NECODING_COMBINER_H
#define ENCODING_COMBINER_H

#include "util.h"
#include "encoding/encoding.h"
#include "io/memory-buffer.h"


namespace encoding {
	template<u64 N1, u64 N2, u64 N3>
	class EncodingCombinerImplementation : public EncodingImplementation {
		const Encoding<N1, N2> _encoding_1;
		const Encoding<N2, N3> _encoding_2;

	public:
		EncodingCombinerImplementation(const Encoding<N1, N2> encoding_1, const Encoding<N2, N3> encoding_2)
			: _encoding_1(encoding_1), _encoding_2(encoding_2) {}

		void encode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			auto temp = io::MemoryBuffer<N2>();
			_encoding_1->encode(input_stream, *(temp.destination()->create_output_stream()));
			_encoding_2->encode(*(temp.source()->create_input_stream()), output_stream);
		}

		void decode(io::InputStream& input_stream, io::OutputStream& output_stream) const override {
			auto temp = io::MemoryBuffer<N2>();
			_encoding_2->decode(input_stream, *(temp.destination()->create_output_stream()));
			_encoding_1->decode(*(temp.source()->create_input_stream()), output_stream);
		}
	};

	template<u64 N1, u64 N2, u64 N3>
	Encoding<N1, N3> operator |(const Encoding<N1, N2> encoding_1, const Encoding<N2, N3> encoding_2) {
		return Encoding<N1, N3>(std::make_shared<EncodingCombinerImplementation<N1, N2, N3>>(encoding_1, encoding_2));
	}
}

#endif