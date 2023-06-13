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

        // Waarom schrijft deze een hele uint64_t naar de output stream? Zo is elke "bit" toch 64 bits lang?
        // Achter de schermen wordt deze Datum gecast naar een integer type met 8, 16, 32 of 64 bits afhankelijk van de domain size.
        // Zie memory-buffer.h.
        virtual void write(Datum value) = 0;
    };
}

#endif