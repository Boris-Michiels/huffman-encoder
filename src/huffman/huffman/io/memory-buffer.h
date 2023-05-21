#ifndef MEMORY_DATA_SOURCE_H
#define MEMORY_DATA_SOURCE_H

#include "data-endpoints.h"
#include <assert.h>
#include <utility>
#include <vector>
#include <limits>


namespace io {
    template<typename T>
    class MemoryInputStream : public InputStream {
    private:
        std::shared_ptr<const std::vector<T>> m_contents;
        size_t m_index;

    public:
        MemoryInputStream(std::shared_ptr<const std::vector<T>> contents) : m_contents(contents), m_index(0) {
            // NOP
        }

        Datum read() override {
            assert(m_index < m_contents->size());

            return (*m_contents)[m_index++];
        }

        bool end_reached() const override {
            return m_index == m_contents->size();
        }
    };

    template<typename T>
    class MemoryOutputStream : public OutputStream {
    private:
        std::shared_ptr<std::vector<T>> m_contents;

    public:
        MemoryOutputStream(std::shared_ptr<std::vector<T>> contents) : m_contents(contents) {
            // NOP
        }

        void write(Datum value) {
            assert(value <= std::numeric_limits<T>::max());

            m_contents->push_back(static_cast<T>(value));
        }
    };

    template<typename T>
    std::unique_ptr<InputStream> create_memory_input_stream(std::shared_ptr<std::vector<T>> buffer) {
        return std::make_unique<MemoryInputStream<T>>(buffer);
    }

    template<typename T>
    std::unique_ptr<OutputStream> create_memory_output_stream(std::shared_ptr<std::vector<T>> buffer) {
        return std::make_unique<MemoryOutputStream<T>>(buffer);
    }

    template<typename T>
    class MemoryDataSourceImplementation : public DataSourceImplementation {
    private:
        std::shared_ptr<std::vector<T>> m_contents;

    public:
        MemoryDataSourceImplementation(std::shared_ptr<std::vector<T>> contents) : m_contents(contents) {
            // NOP
        }

        std::unique_ptr<InputStream> create_input_stream() override {
            return create_memory_input_stream<T>(m_contents);
        }
    };

    template<typename T>
    class MemoryDataDestinationImplementation : public DataDestinationImplementation {
    private:
        std::shared_ptr<std::vector<T>> m_contents;

    public:
        MemoryDataDestinationImplementation(std::shared_ptr<std::vector<T>> contents) : m_contents(contents) {
            // NOP
        }

        std::unique_ptr<OutputStream> create_output_stream() override {
            return create_memory_output_stream<T>(m_contents);
        }
    };

    template<u64 N, typename T>
    DataSource<N> create_memory_data_source(std::shared_ptr<std::vector<T>> buffer) {
        static_assert(N - 1 <= std::numeric_limits<T>::max(), "T too small");

        return DataSource<N>(std::make_shared<MemoryDataSourceImplementation<T>>(buffer));
    }

    template<u64 N, typename T>
    DataDestination<N> create_memory_data_destination(std::shared_ptr<std::vector<T>> buffer) {
        static_assert(N - 1 <= std::numeric_limits<T>::max(), "T too small");

        return DataDestination<N>(std::make_shared<MemoryDataDestinationImplementation<T>>(buffer));
    }

    template<u64 N, typename T = typename SelectIntegerTypeByDomainSize<N>::type>
    class MemoryBuffer {
    private:
        std::shared_ptr<std::vector<T>> m_data;

    public:
        MemoryBuffer() : MemoryBuffer(std::make_shared<std::vector<T>>()) {
            static_assert(N - 1 <= std::numeric_limits<T>::max(), "T too small");
        }

        MemoryBuffer(std::shared_ptr<std::vector<T>> data) : m_data(data) {
            // NOP
        }

        MemoryBuffer(const std::vector<T>& data) : MemoryBuffer(std::make_shared<std::vector<T>>(data)) {
            static_assert(N - 1 <= std::numeric_limits<T>::max(), "T too small");
        }

        DataSource<N> source() {
            return create_memory_data_source<N>(m_data);
        }

        DataDestination<N> destination() {
            return create_memory_data_destination<N>(m_data);
        }

        std::shared_ptr<std::vector<T>> data() {
            return m_data;
        }
    };
}

#endif
