#ifndef GPUMONITOR_H
#define GPUMONITOR_H

#include "eventbus.h"
#include "basethread.h"
#include "gpuload.h"
#include "gpupower.h"
#include "gputemperature.h"
#include <nvml.h>
#include <thread>
#include <atomic>
#include <memory>
#include <string>
#include <log4cpp/Category.hh>

/*!
 * \class periodically samples information about the platform status.
 * This information is then encapsulated in a MonitorEvent and published
 * to the EventBus.
 * GpuMonitor runs in a dedicated thread.
 */
class GpuMonitor : public rmcommon::BaseThread {
    struct GpuMonitorImpl;
    std::unique_ptr<GpuMonitorImpl> pimpl_;
    log4cpp::Category &cat_;
    int monitorPeriod_;
    rmcommon::EventBus &bus_;

    virtual void run() override;
public:
    GpuMonitor(rmcommon::EventBus &eventBus, int monitorPeriod);
    ~GpuMonitor();

//    /*!
//     * \brief Sets the vector of possible CPU chip names.
//     * \param names the comma separated list of module names
//     */
//    void setCpuModuleNames(const std::string &names);
//
//    /*!
//     * \brief Sets the vector of possible battery module names.
//     * \param names the comma separated list of module names
//     */
//    void setBatteryModuleNames(const std::string &names);
};

#endif  // #ifndef GPUMONITOR_H


