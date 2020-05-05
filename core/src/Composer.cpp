
#include <core/Composer.h>

namespace core {

    template <typename T>
    uint32_t Composer<T>::calculate_delay(const std::vector<TileData<T>> &tiles_data) {
        uint64_t max_delay = 0;
        for (const auto &tile_data : tiles_data) {

            if (!tile_data.write)
                continue;

            auto input_step = ceil(tile_data.out_banks.size() / (double)INPUTS);
            auto delay = input_step * DELAY;
            if (delay > max_delay)
                max_delay = delay;

        }
        return max_delay;
    }

    INITIALISE_DATA_TYPES(Composer);

}
