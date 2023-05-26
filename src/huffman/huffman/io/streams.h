#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

#include "util.h"


namespace io {
    struct InputStream {
        virtual ~InputStream() { }

        virtual Datum read() = 0;
        virtual bool end_reached() const = 0;
    };

    struct OutputStream {
        virtual ~OutputStream() { }

        virtual void write(Datum value) = 0; // Waarom schrijft deze een hele uint64_t naar de output stream? Zo is elke "bit" toch 64 bits lang?
    };
}

#endif