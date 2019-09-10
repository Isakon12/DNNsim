#ifndef DNNSIM_STRIPES_H
#define DNNSIM_STRIPES_H

#include "Simulator.h"

#define FC_MULTIPLEX_COLUMNS // Execute each mult-add in a different column

namespace core {

    template <typename T>
    class Stripes : public Simulator<T> {

    private:

        /* Number of concurrent multiplications per PE */
        const uint32_t N_LANES;

        /* Number of columns */
        const uint32_t N_COLUMNS;

        /* Number of rows */
        const uint32_t N_ROWS;

        /* Bits per PE */
        const uint32_t BITS_PE;

        /* Compute number of one bit multiplications
         * @param layer_prec    Layer precision
         * @param network_bits  Max bits network
         * @return              Number of one bit multiplications
         */
        inline uint16_t computeStripesBitsPE(uint8_t layer_prec, int network_bits);

    public:

        /* Constructor
         * @param _N_LANES      Number of concurrent multiplications per PE
         * @param _N_COLUMNS    Number of columns
         * @param _N_ROWS       Number of rows
         * @param _BITS_PE      Number of bits per PE
         * @param _N_THREADS    Number of parallel threads for multi-threading execution
         * @param _FAST_MODE    Enable fast mode to simulate only one image
         * @param _QUIET        Avoid std::out messages
         */
        Stripes(uint32_t _N_LANES, uint32_t _N_COLUMNS, uint32_t _N_ROWS, uint32_t _BITS_PE, uint8_t _N_THREADS,
                bool _FAST_MODE, bool _QUIET) : Simulator<T>(_N_THREADS,_FAST_MODE,_QUIET), N_LANES(_N_LANES),
                N_COLUMNS(_N_COLUMNS), N_ROWS(_N_ROWS), BITS_PE(_BITS_PE) {}

        /* Run the timing simulator of the architecture
         * @param network   Network we want to simulate
         */
        void run(const base::Network<T> &network);

        /* Calculate potentials for the given network
         * @param network   Network we want to calculate work reduction
         */
        void potentials(const base::Network<T> &network);

    };

}

#endif //DNNSIM_STRIPES_H
