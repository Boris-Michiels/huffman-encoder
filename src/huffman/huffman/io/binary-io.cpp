#include "io/binary-io.h"
#include <iostream>


u64 io::read_bits(unsigned nbits, io::InputStream& input) {
	u64 result = 0;

	while (nbits-- != 0) {
		result <<= 1;
		result |= input.end_reached() ? 0 : input.read();
	}

	return result;
}

// Waarom falen de tests die meer dan 8 bits nodig hebben als de bitwise AND "& 1" er niet staat?
void io::write_bits(u64 value, unsigned nbits, io::OutputStream& output) {
	while (nbits-- != 0) {
		output.write((value >> nbits) & 1);
	}
}