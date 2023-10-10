#ifndef NVIDIAMONITOR_H
#define NVIDIAMONITOR_H

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
 * NvidiaMonitor runs in a dedicated thread.
 */
class NvidiaMonitor : public rmcommon::BaseThread {
    struct NvidiaMonitorImpl;
    std::unique_ptr <NvidiaMonitorImpl> pimpl_;
    log4cpp::Category &cat_;
    int monitorPeriod_;
    rmcommon::EventBus &bus_;

    virtual void run() override;

public:
    NvidiaMonitor(rmcommon::EventBus &eventBus, int monitorPeriod);

    ~NvidiaMonitor();

};

#endif  // NVIDIAMONITOR_H