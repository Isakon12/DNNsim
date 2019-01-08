#ifndef DNNSIM_STRIPES_H
#define DNNSIM_STRIPES_H

#include "Simulator.h"

#define NM_WIDTH 256 // Width of the neuron memory row in bits

namespace core {

    template <typename T>
    class Stripes : public Simulator<T> {

    private:

        /* Number of columns */
        const int N_COLUMNS;

        /* Number of rows */
        const int N_ROWS;

        /* Compute cycles for stripes column
         * @param act_x         X position in the input activations
         * @param act_y         Y position in the input activations
         * @param kernel_x      X position in the kernel window
         * @param kernel_y      Y position in the kernel window
         * @param layer_prec    Activations precision per layer
         * @param init_channel  Starting index for the channel
         * @param max_channel   Maximum number of channels
         * @param rowMap        3D mapping of each value in their corresponding row
         */
        uint8_t computeStripesColumn(int act_x, int act_y, int kernel_x, int kernel_y, int layer_prec, int init_channel,
                int max_channel, const std::vector<std::vector<std::vector<int>>> &rowMap);

        /* Compute cycles for stripes tile
         * @param list_act_x    X position for the set of input windows
         * @param list_act_y    Y position for the set of input windows
         * @param kernel_x      X position in the kernel window
         * @param kernel_y      Y position in the kernel window
         * @param layer_prec    Activations precision per layer
         * @param init_channel  Starting index for the channel
         * @param max_channel   Maximum number of channels
         * @param rowMap        3D mapping of each value in their corresponding row
         */
        uint8_t computeStripesTile(const std::vector<int> &list_act_x, const std::vector<int> &list_act_y, int kernel_x,
                int kernel_y, int layer_prec, int init_channel, int max_channel,
                const std::vector<std::vector<std::vector<int>>> &rowMap);

        /* Compute the timing for a convolutional layer
         * @param layer     Layer for which we want to calculate the outputs
         * @param stats     Statistics to fill
         */
        void computeConvolution(const Layer<T> &layer, sys::Statistics::Stats &stats);

        /* Compute the timing for a fully-connected layer
         * @param layer     Layer for which we want to calculate the outputs
         * @param stats     Statistics to fill
         */
        void computeInnerProduct(const Layer<T> &layer, sys::Statistics::Stats &stats);

        /* Compute the potentials for a convolutional layer
         * @param layer     Layer for which we want to calculate potentials
         * @param stats     Statistics to fill
         */
        void computePotentialsConvolution(const core::Layer<T> &layer, sys::Statistics::Stats &stats);

        /* Compute the potentials for a inner product layer
         * @param layer     Layer for which we want to calculate potentials
         * @param stats     Statistics to fill
         */
        void computePotentialsInnerProduct(const core::Layer<T> &layer, sys::Statistics::Stats &stats);

        /* Compute the potentials for a convolutional layer
         * @param layer     Layer for which we want to calculate potentials
         * @param stats     Statistics to fill
         */
        void computeMemAccessesConvolution(const core::Layer<T> &layer, sys::Statistics::Stats &stats);

    public:

        /* Constructor
         * @param _N_COLUMNS            Number of columns
         * @param _N_ROWS               Number of rows
         */
        Stripes(int _N_COLUMNS, int _N_ROWS) : N_COLUMNS(_N_COLUMNS), N_ROWS(_N_ROWS) {}

        /* Run the timing simulator of the architecture
         * @param network   Network we want to simulate
         */
        void run(const Network<T> &network);

        /* Calculate work reduction for the given network
         * @param network   Network we want to calculate work reduction
         */
        void potentials(const Network<T> &network);

        /* Calculate the number of memory accesses
         * @param network   Network we want to simulate
         */
        void memoryAccesses(const Network<T> &network);

    };

}

#endif //DNNSIM_STRIPES_H
