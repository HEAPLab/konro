#ifndef GPUPOWER_H
#define GPUPOWER_H

#include <iostream>

namespace rmcommon {
/*!
 * \class encapsulates power information about the machine
 */
class GpuPower {
    /* Current power in mW */
    std::vector<int> gpu_;
public:

    void addGpuPower(int val) {
        gpu_.push_back(val);
    }

    int getGpuPower(int index) {
        return gpu_[index];
    }

    friend std::ostream &operator << (std::ostream &os, const GpuPower &pp) {
        bool first;
        os << '{'
           << "\"gpus\":";
        os << '[';
        first = true;
        for (const auto &gpu: pp.gpu_) {
            if (first)
                first = false;
            else
                os << ',';
            os << gpu;
        }
        os << ']';
        os << '}';
        return os;
    }
};

}   // namespace rmcommon

#endif // GPUPOWER_H
