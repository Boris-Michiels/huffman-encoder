#ifndef BIT_GROUPER_H
#define BIT_GROUPER_H

#include "encoding/encoding.h"


namespace encoding {
	std::shared_ptr<EncodingImplementation> create_bit_grouper_implementation(u64);

	template<unsigned GROUP_SIZE>
	Encoding<2, 1 << GROUP_SIZE> bit_grouper() {
		return Encoding<2, 1 << GROUP_SIZE>(create_bit_grouper_implementation(GROUP_SIZE));
	}
}

#endif