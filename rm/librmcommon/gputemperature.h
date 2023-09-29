#ifndef GPUTEMLERATURE_H
#define GPUTEMLERATURE_H

#include <iostream>

namespace rmcommon {
/*!
 * \class encapsulates temperature information about the machine
 */
    class GpuTemperature {
        /* Current power in mW */
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
            return os;
        }
    };

}   // namespace rmcommon

#endif // GPUTEMLERATURE_H
