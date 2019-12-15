
#include <sys/Batch.h>

namespace sys {

    bool ReadProtoFromTextFile(const char* filename, google::protobuf::Message* proto) {
        int fd = open(filename, O_RDONLY);
        auto input = new google::protobuf::io::FileInputStream(fd);
        bool success = google::protobuf::TextFormat::Parse(input, proto);
        delete input;
        close(fd);
        return success;
    }

    Batch::Simulate Batch::read_training_simulation(const protobuf::Batch_Simulate &simulate_proto) {
        Batch::Simulate simulate;
        std::string value;
        simulate.network = simulate_proto.network();
        simulate.batch = simulate_proto.batch();
        simulate.epochs = simulate_proto.epochs() < 1 ? 1 : simulate_proto.epochs();
        simulate.network_bits = 16;
		simulate.training = simulate_proto.training();
        simulate.only_forward = simulate_proto.only_forward();
        simulate.only_backward = simulate_proto.only_backward();

        value = simulate_proto.model();
        if(value != "Trace")
            throw std::runtime_error("Training input type configuration for network " + simulate.network +
                                     " must be <Trace>.");
        else
            simulate.model = simulate_proto.model();

        if(simulate_proto.data_type() != "BFloat16")
            throw std::runtime_error("Training input data type configuration for network " + simulate.network +
                                     " must be <BFloat16>.");
        else
            simulate.data_type = simulate_proto.data_type();

        if (simulate.data_type == "BFloat16") {
            for(const auto &experiment_proto : simulate_proto.experiment()) {

                Batch::Simulate::Experiment experiment;
                if(experiment_proto.architecture() == "TensorDash") {
                    experiment.n_lanes = experiment_proto.n_lanes() < 1 ? 16 : experiment_proto.n_lanes();
                    experiment.n_columns = experiment_proto.n_columns() < 1 ? 16 : experiment_proto.n_columns();
                    experiment.n_rows = experiment_proto.n_rows() < 1 ? 16 : experiment_proto.n_rows();
                    experiment.n_tiles = experiment_proto.n_tiles() < 1 ? 1 : experiment_proto.n_tiles();
                    experiment.lookahead_h = experiment_proto.lookahead_h() < 1 ? 2 : experiment_proto.lookahead_h();
                    experiment.lookaside_d = experiment_proto.lookaside_d() < 1 ? 5 : experiment_proto.lookaside_d();
                    experiment.search_shape = experiment_proto.search_shape().empty() ? 'T' :
                            experiment_proto.search_shape().c_str()[0];
                    experiment.banks = experiment_proto.banks() < 1 ? 16 : experiment_proto.banks();

                    value = experiment.search_shape;
                    if(value != "L" && value != "T")
                        throw std::runtime_error("BitTactical search shape for network " + simulate.network +
                                                 " must be <L|T>.");

                } else throw std::runtime_error("Training architecture for network " + simulate.network +
                                                " in BFloat16 must be <TensorDash>.");

                value = experiment_proto.task();
                if(value != "Cycles" && value != "Potentials")
                    throw std::runtime_error("Training task for network " + simulate.network +
                                             " in BFloat16 must be <Cycles|Potentials>.");

                experiment.architecture = experiment_proto.architecture();
                experiment.task = experiment_proto.task();
                simulate.experiments.emplace_back(experiment);

            }
        }

        return simulate;
    }

    Batch::Simulate Batch::read_inference_simulation(const protobuf::Batch_Simulate &simulate_proto) {
        Batch::Simulate simulate;
        std::string value;
        simulate.network = simulate_proto.network();
        simulate.batch = simulate_proto.batch();
        simulate.tensorflow_8b = simulate_proto.tensorflow_8b();
        simulate.intel_inq = simulate_proto.intel_inq();
        simulate.network_bits = simulate_proto.network_bits() < 1 ? 16 : simulate_proto.network_bits();
		simulate.training = simulate_proto.training();
        if(simulate.tensorflow_8b) simulate.network_bits = 8;

        value = simulate_proto.model();
        if(value  != "Caffe" && value != "Trace" && value != "CParams" && value != "Protobuf")
            throw std::runtime_error("Model configuration for network " + simulate.network +
                                     " must be <Caffe|Trace|CParams|Protobuf>.");
        else
            simulate.model = simulate_proto.model();

        value = simulate_proto.data_type();
        if(value  != "Float32" && value != "Fixed16")
            throw std::runtime_error("Input data type configuration for network " + simulate.network +
                                     " must be <Float32|Fixed16>.");
        else
            simulate.data_type = simulate_proto.data_type();

        if (simulate.data_type == "Fixed16") {
            for(const auto &experiment_proto : simulate_proto.experiment()) {

                Batch::Simulate::Experiment experiment;
                if (experiment_proto.architecture() == "SCNN") {
                    experiment.Wt = experiment_proto.wt() < 1 ? 8 : experiment_proto.wt();
                    experiment.Ht = experiment_proto.ht() < 1 ? 8 : experiment_proto.ht();
                    experiment.I = experiment_proto.i() < 1 ? 4 : experiment_proto.i();
                    experiment.F = experiment_proto.f() < 1 ? 4 : experiment_proto.f();
                    experiment.out_acc_size = experiment_proto.out_acc_size() < 1 ?
                            6144 : experiment_proto.out_acc_size();
                    experiment.banks = experiment_proto.banks() < 1 ? 32 : experiment_proto.banks();

                    if(experiment.banks > 32)
                        throw std::runtime_error("Banks for SCNN in network " + simulate.network +
                                                 " must be from 1 to 32");

                } else {

                    experiment.n_lanes = experiment_proto.n_lanes() < 1 ? 16 : experiment_proto.n_lanes();
                    experiment.n_columns = experiment_proto.n_columns() < 1 ? 16 : experiment_proto.n_columns();
                    experiment.n_rows = experiment_proto.n_rows() < 1 ? 16 : experiment_proto.n_rows();
                    experiment.n_tiles = experiment_proto.n_tiles() < 1 ? 16 : experiment_proto.n_tiles();
                    experiment.bits_pe = experiment_proto.bits_pe() < 1 ? 16 : experiment_proto.bits_pe();

                    // Bit Tactical
                    experiment.lookahead_h = experiment_proto.lookahead_h() < 1 ? 2 : experiment_proto.lookahead_h();
                    experiment.lookaside_d = experiment_proto.lookaside_d() < 1 ? 5 : experiment_proto.lookaside_d();
                    experiment.search_shape = experiment_proto.search_shape().empty() ? 'L' :
                                              experiment_proto.search_shape().c_str()[0];
                    experiment.read_schedule = experiment_proto.read_schedule();

                    value = experiment.search_shape;
                    if(value != "L" && value != "T")
                        throw std::runtime_error("BitTactical search shape for network " + simulate.network +
                                                 " must be <L|T>.");
                    if(value == "T" && (experiment.lookahead_h != 2 || experiment.lookaside_d != 5))
                        throw std::runtime_error("BitTactical search T-shape for network " + simulate.network +
                                                 " must be lookahead of 2, and lookaside of 5.");

                    if(experiment_proto.architecture() == "BitPragmatic") {
                        experiment.column_registers = experiment_proto.column_registers();
                        experiment.bits_first_stage = experiment_proto.bits_first_stage();
                        experiment.diffy = experiment_proto.diffy();

                    } else if(experiment_proto.architecture() == "Stripes") {

                    } else if(experiment_proto.architecture() == "ShapeShifter") {
                        experiment.column_registers = experiment_proto.column_registers();
                        experiment.precision_granularity = experiment_proto.precision_granularity() < 1 ? 256 :
                                                           experiment_proto.precision_granularity();
                        experiment.minor_bit = experiment_proto.minor_bit();
                        experiment.diffy = experiment_proto.diffy();

                        if(experiment.precision_granularity % experiment.n_columns != 0 ||
                           (((experiment.n_columns * 16) % experiment.precision_granularity) != 0))
                            throw std::runtime_error("ShapeShifter precision granularity for network " +
                                    simulate.network + " must be multiple of 16 and divisible by the columns.");

                    } else if(experiment_proto.architecture() == "Loom") {
                        experiment.precision_granularity = experiment_proto.precision_granularity() < 1 ? 256 :
                                                           experiment_proto.precision_granularity();
                        experiment.pe_serial_bits = experiment_proto.pe_serial_bits() < 1 ? 1 :
                                                    experiment_proto.pe_serial_bits();
                        experiment.minor_bit = experiment_proto.minor_bit();
                        experiment.dynamic_weights = experiment_proto.dynamic_weights();

                        if(experiment.precision_granularity % experiment.n_columns != 0 ||
                           (((experiment.n_columns * 16) % experiment.precision_granularity) != 0))
                            throw std::runtime_error("Loom precision granularity for network " + simulate.network
                                                     + " must be multiple of 16 and divisible by the columns.");
                        if(experiment.precision_granularity % experiment.n_columns != 0 ||
                           (((experiment.n_rows * 16) % experiment.precision_granularity) != 0))
                            throw std::runtime_error("Loom precision granularity for network " + simulate.network
                                                     + " must be multiple of 16 and divisible by the rows.");

                    } else if (experiment_proto.architecture() == "Laconic") {

                    } else if (experiment_proto.architecture() == "BitTactical") {

                    } else throw std::runtime_error("Architecture for network " + simulate.network +
                                                    " in Fixed16 must be <BitPragmatic|Stripes|ShapeShifter|Laconic|"
                                                    "Loom|BitTactical|SCNN>.");

                }

                value = experiment_proto.task();
                if(value  != "Cycles" && value != "Potentials" && value != "Schedule")
                    throw std::runtime_error("Task for network " + simulate.network +
                                             " in Fixed16 must be <Cycles|Potentials|Schedule>.");

                if(experiment_proto.architecture() != "BitTactical" && experiment_proto.task() == "Schedule")
                    throw std::runtime_error("Task \"Schedule\" for network " + simulate.network +
                                             " in Fixed16 is only allowed for BitTactial.");

                experiment.architecture = experiment_proto.architecture();
                experiment.task = experiment_proto.task();
                simulate.experiments.emplace_back(experiment);

            }
        } else if (simulate.data_type == "Float32") {
            for(const auto &experiment_proto : simulate_proto.experiment()) {

                Batch::Simulate::Experiment experiment;
                if (experiment_proto.architecture() == "SCNN") {
                    experiment.Wt = experiment_proto.wt() < 1 ? 8 : experiment_proto.wt();
                    experiment.Ht = experiment_proto.ht() < 1 ? 8 : experiment_proto.ht();
                    experiment.I = experiment_proto.i() < 1 ? 4 : experiment_proto.i();
                    experiment.F = experiment_proto.f() < 1 ? 4 : experiment_proto.f();
                    experiment.out_acc_size = experiment_proto.out_acc_size() < 1 ?
                            1024 : experiment_proto.out_acc_size();
                    experiment.banks = experiment_proto.banks() < 1 ? 32 : experiment_proto.banks();
                    if(experiment.banks > 32)
                        throw std::runtime_error("Banks for SCNN in network " + simulate.network +
                                                 " must be from 1 to 32");

                } else throw std::runtime_error("Architecture for network " + simulate.network +
                                                " in Float32 must be <SCNN>.");

                value = experiment_proto.task();
                if(value  != "Cycles" && value != "Potentials")
                    throw std::runtime_error("Task for network " + simulate.network +
                                             " in Float32 must be <Cycles|Potentials>.");

                experiment.architecture = experiment_proto.architecture();
                experiment.task = experiment_proto.task();
                simulate.experiments.emplace_back(experiment);

            }
        }

        return simulate;
    }

    void Batch::read_batch() {
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        protobuf::Batch batch;

        if (!ReadProtoFromTextFile(this->path.c_str(),&batch)) {
            throw std::runtime_error("Failed to read prototxt");
        }

        for(const auto &simulate : batch.simulate()) {
            try {
                this->simulations.emplace_back(simulate.training() ? 
					read_training_simulation(simulate) : read_inference_simulation(simulate));
            } catch (std::exception &exception) {
                std::cerr << "Prototxt simulation error: " << exception.what() << std::endl;
                #ifdef STOP_AFTER_ERROR
                exit(1);
                #endif
            }
        }

    }

    /* Getters */
    const std::vector<Batch::Simulate> &Batch::getSimulations() const { return simulations; }

}
