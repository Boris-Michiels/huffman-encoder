#ifndef ENCODING_H
#define ENCODING_H

#include "io/streams.h"
#include "io/data-endpoints.h"

#include <memory>


namespace encoding {
	class EncodingImplementation {
	public:
		virtual ~EncodingImplementation() {}

		virtual void encode(io::InputStream&, io::OutputStream&) const;
		virtual void decode(io::InputStream&, io::OutputStream&) const;
	};

	template<u64 IN, u64 OUT>
	class Encoding {
		std::shared_ptr<EncodingImplementation> _enc_impl;

	public:
		Encoding(std::shared_ptr<EncodingImplementation> enc_impl) : _enc_impl(enc_impl) {}

		std::shared_ptr<EncodingImplementation> operator ->() { return _enc_impl; }
		const std::shared_ptr<EncodingImplementation> operator ->() const { return _enc_impl; }
	};

	template<u64 IN, u64 OUT>
	void encode(io::DataSource<IN> data_source, Encoding<IN, OUT> encoding, io::DataDestination<OUT> data_destination) {
		auto input_stream_ptr = data_source->create_input_stream();
		auto output_stream_ptr = data_destination->create_output_stream();

		encoding->encode(*input_stream_ptr, *output_stream_ptr);
	}

	template<u64 IN, u64 OUT>
	void decode(io::DataSource<IN> data_source, Encoding<IN, OUT> encoding, io::DataDestination<OUT> data_destination) {
		auto input_stream_ptr = data_source->create_input_stream();
		auto output_stream_ptr = data_destination->create_output_stream();

		encoding->decode(*input_stream_ptr, *output_stream_ptr);
	}
}

#endif