#ifndef GPULOAD_H
#define GPULOAD_H

#include <vector>
#include <iostream>

namespace rmcommon {
/*!
 * \class encapsulates cpu utilization information
 */
class GpuLoad {
    /*! Usage of each CPU */
    std::vector<int> gpu_;

public:
    void addGpuLoad(int load) {
        gpu_.push_back(load);
    }

    int getGpuLoad(int idx) {
        return gpu_[idx];
    }


    friend std::ostream &operator << (std::ostream &os, const GpuLoad &pt) {
        bool first;
        os << '{'
           << "\"gpus\":";
        os << '[';
        first = true;
        for (const auto &gpu: pt.gpu_) {
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

#endif // GPULOAD_H
