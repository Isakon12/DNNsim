#ifndef DNNSIM_BATCH_H
#define DNNSIM_BATCH_H

#include <sys/common.h>
#include <batch.pb.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/text_format.h>

namespace sys {

    class Batch {

    public:

        /* Struct for the Transform instructions */
        struct Transform {
            int batch = 0;
            std::string inputType = "";
            std::string inputDataType = "";
            std::string outputType = "";
            std::string outputDataType = "";
            std::string network = "";
            bool bias_and_out_act = false;
            bool tensorflow_8b = false;
        };

        /* Struct for the Simulate instructions */
        struct Simulate {

            struct Experiment {
                std::string architecture = "";
                std::string task = "";
                int n_columns = 0;
                int n_rows = 0;
                int bits_pe = 0;
                int column_registers = 0;
                int precision_granularity = 0;
                bool leading_bit = false;
                bool minor_bit = false;
                int bits_first_stage = 0;
                int lookahead_h = 0;
                int lookaside_d = 0;
                char search_shape = 'X';
                bool read_schedule_from_proto = false;
                int Wt = 0;
                int Ht = 0;
                int I = 0;
                int F = 0;
                int out_acc_size = 0;
                int banks = 0;
                int pe_serial_bits = 0;
                int M = 0;
                int N = 0;
                int pmax = 0;
                int pmin = 0;
                bool diffy = false;
                bool dynamic_weights = false;
            };

            int batch = 0;
            int epochs = 0;
            std::string inputType = "";
            std::string inputDataType = "";
            std::string network = "";
            int network_bits = 0;
            bool bias_and_out_act = false;
            bool tensorflow_8b = false;
            bool training = false;
            bool only_forward = false;
            bool only_backward = false;
            int decoder_states = 0;
            std::vector<Experiment> experiments;
        };

    private:

        /* Path to the batch file */
        std::string path;

        /* Transformations */
        std::vector<Transform> transformations;

        /* Simulations */
        std::vector<Simulate> simulations;

        /* Return the transformation parsed from the prototxt file
         * @param transform_proto   prototxt transformation
         */
        Transform read_transformation(const protobuf::Batch_Transform &transform_proto);

        /* Return the training simulation parsed from the prototxt file
         * @param simulate_proto   prototxt simulation
         */
        Simulate read_training_simulation(const protobuf::Batch_Simulate &simulate_proto);

        /* Return the inference simulation parsed from the prototxt file
         * @param simulate_proto   prototxt simulation
         */
        Simulate read_inference_simulation(const protobuf::Batch_Simulate &simulate_proto);

    public:

        /* Constructor
         * @param _path     Path to the batch file
         */
        explicit Batch(const std::string &_path){ this->path = _path; }

        /* Parse the batch file into memory */
        void read_batch();

        /* Getters */
        const std::vector<Transform> &getTransformations() const;
        const std::vector<Simulate> &getSimulations() const;

    };

}


#endif //DNNSIM_BATCH_H
