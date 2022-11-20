#ifndef APPMAPPING_H
#define APPMAPPING_H

#include <app.h>
#include <memory>
#include "cpucontrol.h"
#include "cpusetcontrol.h"
#include "numericvalue.h"
#include "memorycontrol.h"


/*!
 * \class encapsulates an application and adds information about
 * the resources it can use.
 * This class acts as a cache with respect to cgroup files. Whenever
 * a new value is written to a cgroup file, this class stores the value
 * in a variable. Subsequent reads to this value can access the variable
 * instead of reading the file.
 * The setter methods of this class can be used to simultaneosuly write
 * the desired value to a cgroup file and store it in a variable.
 */
class AppMapping {
    std::shared_ptr<rmcommon::App> app_;
    /*! Processing Units that can execute the app */
    rmcommon::CpusetVector puVec_;
    /*! maximum cpu bandwidth limit */
    rmcommon::NumericValue cpuMax_;
    /*! memory nodes that can be used by the app */
    rmcommon::CpusetVector memNodes_;
    /*! minimum amount of memory the app must always retain */
    int minMemory_;
    /*! memory usage hard limit for the app */
    int maxMemory_;

public:
    typedef std::shared_ptr<AppMapping> AppMappingPtr;

    AppMapping(std::shared_ptr<rmcommon::App> app) :
        app_(app),
        minMemory_(-1),
        maxMemory_(-1)
    {
    }
    ~AppMapping() = default;

    /*!
     * \brief Gets the pid of the application
     * \returns the pid of the application
     */
    pid_t getPid() const {
        return app_->getPid();
    }

    std::shared_ptr<rmcommon::App> getApp() const {
        return app_;
    }

    rmcommon::CpusetVector getPuVector() {
        if (puVec_.empty()) {
            puVec_ = pc::CpusetControl::instance().getCpusEffective(app_);
        }
        return puVec_;
    }

    void setPuVector(rmcommon::CpusetVector puVec) {
        pc::CpusetControl::instance().setCpus(puVec, app_);
        puVec_ = puVec;
    }

    rmcommon::NumericValue getCpuMax() {
        if (cpuMax_.isInvalid()) {
            cpuMax_ = pc::CpuControl::instance().getMax(app_);
        }
        return cpuMax_;
    }

    void setCpuMax(rmcommon::NumericValue cpuMax) {
        pc::CpuControl::instance().setMax(cpuMax, app_);
        cpuMax_ = cpuMax;
    }

    rmcommon::CpusetVector getMemNodes() {
        if (memNodes_.empty()) {
            memNodes_ = pc::CpusetControl::instance().getMemsEffective(app_);
        }
        return memNodes_;
    }

    void setMemNodes(rmcommon::CpusetVector memNodes) {
        pc::CpusetControl::instance().setMems(memNodes, app_);
        memNodes_ = memNodes;
    }

    /*! total amount of memory currently used by the app */
    int getCurrentMemory() {
        return pc::MemoryControl::instance().getCurrent(app_);
    }

    int getMinMemory() {
        if (minMemory_ == -1) {
            minMemory_ = pc::MemoryControl::instance().getMin(app_);
        }
        return minMemory_;
    }

    void setMinMemory(int minMemory) {
        minMemory_ = minMemory;
    }

    int getMaxMemory() {
        if (maxMemory_ == -1) {
            maxMemory_ = pc::MemoryControl::instance().getMax(app_);
        }
        return maxMemory_;
    }

    void setMaxMemory(int maxMemory) {
        maxMemory_ = maxMemory;
    }
};

#endif // APPMAPPING_H
