
#include <core/PPU.h>

namespace core {

    template <typename T>
    std::string PPU<T>::header() {
        std::string header = "Number of inputs in parallel: " + std::to_string(INPUTS) + "\n";
        header += "Delay: " + std::to_string(DELAY) + "\n";
        return header;
    }

    template <typename T>
    void PPU<T>::calculate_delay(const std::vector<core::TileData<T>> &tiles_data) {
        uint64_t inputs = 0;
        for (const auto &tile_data : tiles_data) {

            if (!tile_data.ppu)
                continue;

            inputs += tile_data.out_banks.size();

        }
        auto input_steps = ceil(inputs / (double)INPUTS);
        auto delay = input_steps * DELAY;
        *global_cycle += delay;
    }

    INITIALISE_DATA_TYPES(PPU);

}
