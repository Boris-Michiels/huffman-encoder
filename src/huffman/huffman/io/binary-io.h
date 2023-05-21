#ifndef BINARY_IO_H
#define BINARY_IO_H

#include "util.h"
#include "streams.h"


namespace io {
	u64 read_bits(unsigned, io::InputStream&);
	void write_bits(u64, unsigned, io::OutputStream&);
}

#endif
