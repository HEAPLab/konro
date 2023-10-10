#ifndef GPUTEMPERATURE_H
#define GPUTEMPERATURE_H

#include <iostream>

namespace rmcommon {
/*!
 * \class encapsulates temperature information about the GPU
 */
    class GpuTemperature {
        /* Current temperature in CELSIUS */
        std::vector<int> gpu_;
    public:

        void addGpuTemperature(int val) {
            gpu_.push_back(val);
        }

        int getGpuTemperature(int index) {
            return gpu_[index];
        }

        friend std::ostream &operator << (std::ostream &os, const GpuTemperature &pp) {
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

#endif // GPUTEMPERATURE_H
