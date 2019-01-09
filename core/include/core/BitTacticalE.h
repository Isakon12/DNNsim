#ifndef DNNSIM_BITTACTICAL_E_H
#define DNNSIM_BITTACTICAL_E_H

#include "BitTactical.h"

#define ZERO_COUNT // Count zeroes as 1 cycle
#define BOOTH_ENCODING // Activate booth-like encoding

namespace core {

    template <typename T>
    class BitTacticalE : public BitTactical<T> {

    private:

        /* Compute number of one bit multiplications given a weights and an activation
         * @param act               Activation
         * @param wgt               Weight
         * @return                  Number of one bit multiplications
         */
        uint8_t computeTacticalEBitsPE(uint16_t act, uint16_t wgt);

        /* Compute number of cycles for pragmatic style PE
         * @param act       Activation
         * @return          Number of cycles
         */
        uint8_t computeTacticalEPE(uint16_t act);

        /* Compute cycles for BitTacticalE column
         * @param batch             Current number of batch
         * @param act_x             X position for the input window
         * @param act_y             Y position for the input window
         * @param init_filter       Starting index for the filter
         * @param stride            Stride of the current layer
         * @param padded_act        Set of padded input activations
         * @param wgt               Set of weights
         * @param max_filter        Maximum number of filters
         * @param dense_schedule    Data structure containing the weights
         * @return                  Number of cycles
         */
        uint8_t computeTacticalEColumn(int batch, int act_x, int act_y, int init_filter, int stride,
                const cnpy::Array<T> &padded_act, const cnpy::Array<T> &wgt, int max_filter,
                const std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> &dense_schedule);

        /* Compute cycles for BitTacticalE tile
         * @param batch             Current number of batch
         * @param list_act_x        X position for the set of input windows
         * @param list_act_y        Y position for the set of input windows
         * @param init_filter       Starting index for the filter
         * @param stride            Stride of the current layer
         * @param padded_act        Set of padded input activations
         * @param wgt               Set of weights
         * @param max_filter        Maximum number of filters
         * @param dense_schedule    Data structure containing the weights
         * @return                  Number of cycles
         */
        uint8_t computeTacticalETile(int batch, const std::vector<int> &list_act_x,
                const std::vector<int> &list_act_y, int init_filter, int stride, const cnpy::Array<T> &padded_act,
                const cnpy::Array<T> &wgt, int max_filter,
                const std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> &dense_schedule);

        /* Compute the timing for a convolutional layer
         * @param layer     Layer for which we want to calculate the outputs
         * @param stats     Statistics to fill
         */
        void computeConvolution(const Layer<T> &layer, sys::Statistics::Stats &stats) override;

        /* Compute the timing for a fully-connected layer
         * @param layer     Layer for which we want to calculate the outputs
         * @param stats     Statistics to fill
         */
        void computeInnerProduct(const Layer<T> &layer, sys::Statistics::Stats &stats) override;

        /* Compute the potentials for a convolutional layer
         * @param layer     Layer for which we want to calculate potentials
         * @param stats     Statistics to fill
         */
        void computePotentialsConvolution(const core::Layer<T> &layer, sys::Statistics::Stats &stats) override;

        /* Compute the potentials for a inner product layer
         * @param layer     Layer for which we want to calculate potentials
         * @param stats     Statistics to fill
         */
        void computePotentialsInnerProduct(const core::Layer<T> &layer, sys::Statistics::Stats &stats) override;

    public:

        /* Constructor
         * @param _N_COLUMNS            Number of columns
         * @param _N_ROWS               Number of rows
         */
        BitTacticalE(int _N_COLUMNS, int _N_ROWS) : BitTactical<T>(_N_COLUMNS,_N_ROWS) {}

        /* Run the timing simulator of the architecture
         * @param network   Network we want to simulate
         */
        void run(const Network<T> &network) override;

        /* Calculate work reduction for the given network
         * @param network   Network we want to calculate work reduction
         */
        void potentials(const Network<T> &network) override;

    };

}

#endif //DNNSIM_BITTACTICAL_E_H
