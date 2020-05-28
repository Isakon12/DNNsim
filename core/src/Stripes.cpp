
#include <core/Stripes.h>

namespace core {

    /* AUXILIARY FUNCTIONS */

    template <typename T>
    uint64_t Stripes<T>::getCycles() const {
        return this->linear ? sys::get_max(this->compute_cycles) : this->cycles;
    }

    template <typename T>
    std::string Stripes<T>::name() {
        return "Stripes";
    }

    /* CYCLES */

    template <typename T>
    bool Stripes<T>::diffy() {
        return false;
    }

    template <typename T>
    bool Stripes<T>::schedule() {
        return false;
    }

    template <typename T>
    void Stripes<T>::process_tiles(const std::vector<TileData<T>> &tiles_data) {

        if (this->linear) {

            if (this->cycles < this->compute_cycles[this->column_index])
                this->cycles = this->compute_cycles[this->column_index];

            this->compute_cycles[this->column_index] = this->cycles + this->act_prec;
            this->cycles++;

            this->column_cycles[this->column_index] = *this->global_cycle + this->act_prec;
            this->column_index = (this->column_index + 1) % this->column_cycles.size();

            this->done_cycle = *this->global_cycle + this->act_prec;
            this->ready_cycle = this->column_cycles[this->column_index];

        } else {

            this->done_cycle = *this->global_cycle + this->act_prec;
            this->ready_cycle = *this->global_cycle + this->act_prec;
            this->cycles += this->act_prec;

        }

        for (const auto &tile_data : tiles_data) {

            if (!tile_data.valid)
                continue;

            if (this->linear) {
                this->scheduled_pe += tile_data.filters.size();
                this->idle_pe += this->ROWS - tile_data.filters.size();
            } else {
                this->scheduled_pe += tile_data.windows.size() * tile_data.filters.size();
                this->idle_pe += (this->COLUMNS * this->ROWS - tile_data.windows.size() * tile_data.filters.size());
            }

        }

    }

    /* POTENTIALS */

    template <typename T>
    std::string Stripes<T>::filename_pot() {
        return "";
    }

    template <typename T>
    std::string Stripes<T>::header_pot() {
        return "";
    }

    template <typename T>
    uint16_t Stripes<T>::computeBits(T act, T wgt) {
        return this->act_prec * this->network_width;
    }

    template class Stripes<uint16_t>;

}
