
#include <core/LocalBuffer.h>

namespace core {

    template <typename T>
    std::string LocalBuffer<T>::header() {
        std::string header = "Number of memory rows: " + std::to_string(ROWS) + "\n";
        if (READ_DELAY != NULL_DELAY) header += "Read delay: " + std::to_string(READ_DELAY) + "\n";
        if (WRITE_DELAY != NULL_DELAY) header += "Write delay: " + std::to_string(WRITE_DELAY) + "\n";
        return header;
    }

    template <typename T>
    void LocalBuffer<T>::configure_layer() {
        idx = 0;
        ready_cycle = std::vector<uint64_t>(ROWS, 0);
        done_cycle = std::vector<uint64_t>(ROWS, 0);
    }

    template <typename T>
    uint64_t LocalBuffer<T>::getFifoReadyCycle() const {
        return ready_cycle[idx];
    }

    template <typename T>
    uint64_t LocalBuffer<T>::getFifoDoneCycle() const {
        return done_cycle[idx];
    }

    template <typename T>
    void LocalBuffer<T>::update_fifo() {
        idx = (idx + 1) % ROWS;
    }

    template <typename T>
    bool LocalBuffer<T>::data_ready() {
        return ready_cycle[idx] <= *this->global_cycle;
    }

    template <typename T>
    void LocalBuffer<T>::read_request(uint64_t global_buffer_ready_cycle) {
        ready_cycle[idx] = global_buffer_ready_cycle + READ_DELAY;
    }

    template <typename T>
    void LocalBuffer<T>::evict_data() {
        done_cycle[idx] = *this->global_cycle;
    }

    template <typename T>
    bool LocalBuffer<T>::write_ready() {
        return done_cycle[idx] <= *this->global_cycle;
    }

    template <typename T>
    void LocalBuffer<T>::write_request() {
        ready_cycle[idx] = *this->global_cycle + WRITE_DELAY;
    }

    template <typename T>
    void LocalBuffer<T>::update_done_cycle(uint64_t global_buffer_ready_cycle) {
        done_cycle[idx] = global_buffer_ready_cycle;
    }


    INITIALISE_DATA_TYPES(LocalBuffer);

}
