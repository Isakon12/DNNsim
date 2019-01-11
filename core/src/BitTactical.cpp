
#include <core/BitTactical.h>

namespace core {

    /* AUXILIARY FUNCTIONS */

    template <typename T>
    std::vector<std::vector<std::vector<std::tuple<int,int,int>>>> naive_scheduler(const cnpy::Array<T> &wgt,
            int act_channels) {

        const auto &wgt_shape = wgt.getShape();

        int num_filters = wgt_shape[0];
        int wgt_channels = wgt_shape[1];
        int Kx = wgt_shape[2];
        int Ky = wgt_shape[3];

        int groups = act_channels / wgt_channels;
        int it_per_group = num_filters / groups;

        std::vector<std::vector<std::vector<std::tuple<int,int,int>>>> naive_schedule =
                std::vector<std::vector<std::vector<std::tuple<int,int,int>>>>((unsigned)num_filters,
                std::vector<std::vector<std::tuple<int,int,int>>>(WEIGHT_LANES,std::vector<std::tuple<int,int,int>>()));

        int current_group = 0, group_m = 0, start_group = 0;
        for(int m=0; m<num_filters; m++) {
            int index = 0;
            for (int i = 0; i < Kx; i++) {
                for (int j = 0; j < Ky; j++) {
                    for (int k = start_group; k < wgt_channels + start_group; k+=WEIGHT_LANES) {
                        for(int channel = k; channel < std::min(k + WEIGHT_LANES,act_channels); channel++) {
                            naive_schedule[m][index].push_back(std::make_tuple(channel,i,j));
                            index++;
                            if(index == WEIGHT_LANES) index = 0;
                        }
                    }
                }
            }

            // Ensure all the queue are equal in length
            while (index != 0) {
                naive_schedule[m][index].push_back(std::make_tuple(0,0,0));
                index++;
                if(index == WEIGHT_LANES) index = 0;
            }

            group_m++;
            if(group_m >= it_per_group) {
                group_m = 0;
                current_group++;
                start_group = wgt_channels*current_group;
            }

        }

        return naive_schedule;
    }

    void promote_weight() {}

    void weight_schedule_L_shape(std::vector<std::vector<std::vector<std::tuple<int,int,int>>>> &naive_schedule,
            int filter, int time) {

    }

    std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> dense_scheduler_L_shape(
            std::vector<std::vector<std::vector<std::tuple<int,int,int>>>> &naive_schedule) {

        std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> dense_schedule =
                std::vector<std::vector<std::queue<std::tuple<int,int,int>>>>(naive_schedule.size(),
                std::vector<std::queue<std::tuple<int,int,int>>>(WEIGHT_LANES,std::queue<std::tuple<int,int,int>>()));

        for(int m=0; m<naive_schedule.size(); m++) {
            for(int time=0; time<naive_schedule.front().front().size(); time++) {

            }
        }

        return dense_schedule;
    }

    std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> dense_scheduler_T_shape(
            std::vector<std::vector<std::vector<std::tuple<int,int,int>>>> &naive_schedule) {

        std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> dense_schedule =
                std::vector<std::vector<std::queue<std::tuple<int,int,int>>>>(naive_schedule.size(),
                std::vector<std::queue<std::tuple<int,int,int>>>(WEIGHT_LANES,std::queue<std::tuple<int,int,int>>()));

        return dense_schedule;
    }

    template <typename T>
    std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> BitTactical<T>::scheduler(const cnpy::Array<T> &wgt,
            int act_channels) {

        auto naive_schedule = naive_scheduler(wgt,act_channels);

        return SEARCH_SHAPE == 'L' ? dense_scheduler_L_shape(naive_schedule) : dense_scheduler_T_shape(naive_schedule);

    }

    template <typename T>
    bool BitTactical<T>::check_schedule(const std::vector<std::vector<std::queue<std::tuple<int,int,int>>>>
        &dense_schedule, int init_filter, int max_filter) {

        for (int filter = init_filter; filter < std::min(init_filter + this->N_ROWS, max_filter); filter++) {
            for(int i = 0; i < WEIGHT_LANES; i++) {
                if(!dense_schedule[filter][i].empty())
                    return true;
            }
        }
        return false;
    }

    template <typename T>
    void BitTactical<T>::update_schedule(std::vector<std::vector<std::queue<std::tuple<int,int,int>>>> &dense_schedule
            ,int init_filter, int max_filter) {

        for (int filter = init_filter; filter < std::min(init_filter + this->N_ROWS, max_filter); filter++) {
            for(int i = 0; i < WEIGHT_LANES; i++) {
                dense_schedule[filter][i].pop();
            }
        }
    }

    /* MEMORY ACCESSES */

    template <typename T>
    void BitTactical<T>::computeMemAccessesConvolution(const core::Layer<T> &layer, sys::Statistics::Stats &stats) {

    }

    template <typename T>
    void BitTactical<T>::memoryAccesses(const core::Network<T> &network) {

    }

    template class BitTactical<uint16_t>;

}