#ifndef BINARY_IO_H
#define BINARY_IO_H

#include "util.h"
#include "io/streams.h"


namespace io {
	u64 read_bits(u64, io::InputStream&);
	void write_bits(u64, u64, io::OutputStream&);
}

#endif