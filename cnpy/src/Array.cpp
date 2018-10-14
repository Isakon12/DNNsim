
#include <cnpy/Array.h>

namespace cnpy {

    void Array::set_values(const std::string &path) {
        cnpy::NpyArray data_npy;
        cnpy::npy_load(path, data_npy, this->shape);
        unsigned long long max_index = 1;
        for(size_t length : shape)
            max_index *= (int)length;
        for(unsigned long long i = 0; i < max_index; i++)
            this->data.push_back(data_npy.data<float>()[i]);
    }

    void Array::set_values(const std::vector<float> &_data, const std::vector<size_t> &_shape) {
        Array::data = _data;
        Array::shape = _shape;
    }


    float Array::get (int i, int j, int k, int l) const {
        unsigned long long index = shape[1]*shape[2]*shape[3]*i + shape[2]*shape[3]*j + shape[3]*k + l;
        if(index >= this->data.size())
            exit(1);
        return this->data[index];
    }

    float Array::get (int i, int j) const {
        unsigned long long index = shape[1]*i + j;
        if(index >= this->data.size()) {
            std::cerr << "Array out of index" << std::endl;
            exit(1);
        }
        return this->data[index];
    }

    float Array::get(unsigned long long index) const {
        if(index >= this->data.size()) {
            std::cerr << "Array out of index" << std::endl;
            exit(1);
        }
        return this->data[index];
    }

    unsigned long Array::getDimensions() const {
        return shape.size();
    }

    /* Getters */
    const std::vector<size_t> &Array::getShape() const { return shape; }
    unsigned long long int Array::getMax_index() const { return data.size(); }

}
