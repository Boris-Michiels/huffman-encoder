#ifndef EOF_ENCODING_H
#define EOF_ENCODING_H

#include "util.h"
#include "encoding/encoding.h"


namespace encoding {
	std::shared_ptr<EncodingImplementation> create_eof_implementation(u64);

	template<unsigned N>
	Encoding<N, N + 1> eof_encoding() {
		return Encoding<N, N + 1>(create_eof_implementation(N));
	}
}

#endif