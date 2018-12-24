#ifndef DNNSIM_BITPRAGMATIC_H
#define DNNSIM_BITPRAGMATIC_H

#include "Simulator.h"

namespace core {

    template <typename T>
    class BitPragmatic : public Simulator<T> {

    private:


        /* Compute cycles for one column of pragmatic
         * @param batch         Current number of batch
         * @param act_x         X position in the input activations
         * @param act_y         Y position in the input activations
         * @param kernel_x      X position in the kernel window
         * @param kernel_y      Y position in the kernel window
         * @param init_channel  Starting index for the channel
         * @param stride        Stride of the current layer
         * @param padded_act    Set of padded input activations
         * @param max_channel   Maximum number of channels
         * @return              Number of cycles
         */
        uint8_t computePragmaticColumn(int batch, int act_x, int act_y, int kernel_x, int kernel_y, int init_channel,
                int stride, const cnpy::Array<T> &padded_act, int max_channel);

        /* Compute cycles for pragmatic tile
         * @param batch         Current number of batch
         * @param list_act_x    X position for the set of input windows
         * @param list_act_y    Y position for the set of input windows
         * @param kernel_x      X position in the kernel window
         * @param kernel_y      Y position in the kernel window
         * @param init_channel  Starting index for the channel
         * @param stride        Stride of the current layer
         * @param padded_act    Set of padded input activations
         * @param max_channel   Maximum number of channels
         * @return              Number of cycles
         */
        uint8_t computePragmaticTile(int batch, std::vector<int> &list_act_x, std::vector<int> &list_act_y, int kernel_x,
                int kernel_y, int init_channel, int stride, const cnpy::Array<T> &padded_act, int max_channel);

        /* Compute the timing for a convolutional layer
         * @param layer     Layer for which we want to calculate the outputs
         * @param stats     Statistics to fill
         */
        void computeConvolution(const Layer<T> &layer, sys::Statistics::Stats &stats);

    public:

        /* Calculate the number of memory accesses
         * @param network   Network we want to simulate
         */
        void memoryAccesses(const Network<T> &network);

        /* Run the timing simulator of the architecture
         * @param network   Network we want to simulate
         */
        void run(const Network<T> &network);

    };

}

#endif //DNNSIM_BITPRAGMATIC_H
