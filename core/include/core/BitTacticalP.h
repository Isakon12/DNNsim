#ifndef DNNSIM_BITTACTICAL_P_H
#define DNNSIM_BITTACTICAL_P_H

#include "BitTactical.h"

#define ZERO_COUNT // Count zeroes as 1 cycle
#define FC_MULTIPLEX_COLUMNS // Execute each mult-add in a different column

namespace core {

    template <typename T>
    class BitTacticalP : public BitTactical<T> {

    private:

        /* Number of activations per group: Tile, SIP */
        std::string PRECISION_GRANULARITY;

        /* Compute number of one bit multiplications given a weights and an activation
         * @param wgt               Weight
         * @param act_layer_rec     Layer precision
         * @return                  Number of one bit multiplications
         */
        uint8_t computeTacticalPBitsPE(uint16_t wgt, uint8_t act_layer_prec);

        /* Compute cycles for Bit-Tactical P column
         * @param batch             Current number of batch
         * @param act_x             X position for the input window
         * @param act_y             Y position for the input window
         * @param stride            Stride of the current layer
         * @param padded_act        Set of padded input activations
         * @param dense_schedule    Data structure containing the weights
         * @param schedule_time     Time index for the scheduler
         * @return                  Number of cycles
         */
        uint8_t computeTacticalPColumn(int batch, int act_x, int act_y, int stride, const cnpy::Array<T> &padded_act,
                const schedule &dense_schedule, int schedule_time);

        /* Compute cycles for Bit-Tactical P tile
         * @param batch             Current number of batch
         * @param list_act_x        X position for the set of input windows
         * @param list_act_y        Y position for the set of input windows
         * @param stride            Stride of the current layer
         * @param padded_act        Set of padded input activations
         * @param dense_schedule    Data structure containing the weights
         * @param schedule_time     Time index for the scheduler
         * @return                  Number of cycles
         */
        uint8_t computeTacticalPTile(int batch, const std::vector<int> &list_act_x, const std::vector<int>
                &list_act_y, int stride, const cnpy::Array<T> &padded_act, const schedule &dense_schedule,
                int schedule_time);

        /* Compute the timing for a convolutional layer
         * @param layer                 Layer for which we want to calculate the outputs
         * @param stats                 Statistics to fill
         * @param proto_dense_schedule  Schedule read from protobuf file
         */
        void computeConvolution(const Layer<T> &layer, sys::Statistics::Stats &stats,
                                const schedule &proto_dense_schedule) override;

        /* Compute the timing for a fully-connected layer
         * @param layer                 Layer for which we want to calculate the outputs
         * @param stats                 Statistics to fill
         * @param proto_dense_schedule  Schedule read from protobuf file
         */
        void computeInnerProduct(const Layer<T> &layer, sys::Statistics::Stats &stats,
                                 const schedule &proto_dense_schedule) override;

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
         * @param _N_COLUMNS                Number of columns
         * @param _N_ROWS                   Number of rows
         * @param _PRECISION_GRANULARITY    Granularity for dynamic precisions
         * @param _COLUMN_REGISTERS         Number of registers per SIP
         * @param _LOOKAHEAD_D              Value for scheduler lookahead
         * @param _LOOKASIDE_H              Value for scheduler lookaside
         * @param _SEARCH_SHAPE             Type of search
         * @param _N_THREADS                Number of parallel threads for multi-threading execution
         * @param _FAST_MODE                Enable fast mode to simulate only one image
         */
        BitTacticalP(int _N_COLUMNS, int _N_ROWS, const std::string &_PRECISION_GRANULARITY, int _COLUMN_REGISTERS,
                int _LOOKAHEAD_H, int _LOOKASIDE_D, const char _SEARCH_SHAPE, uint8_t _N_THREADS, bool _FAST_MODE) :
                BitTactical<T>(_N_COLUMNS, _N_ROWS,_COLUMN_REGISTERS,_LOOKAHEAD_H,_LOOKASIDE_D,_SEARCH_SHAPE,_N_THREADS,
                _FAST_MODE) {
            PRECISION_GRANULARITY = _PRECISION_GRANULARITY;
        }

        /* Run the timing simulator of the architecture
         * @param network   Network we want to simulate
         */
        void run(const Network<T> &network) override;

        /* Run the timing simulator of the architecture
         * @param network   Network we want to simulate
         * @param schedules Dense schedules for the layer we want to simulate
         */
        void run(const Network<T> &network, const std::vector<schedule> &schedules) override;

        /* Calculate potentials for the given network
         * @param network   Network we want to calculate work reduction
         */
        void potentials(const Network<T> &network) override;

    };

}

#endif //DNNSIM_BITTACTICAL_P_H
