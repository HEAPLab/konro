#include "cpubasedpolicy.h"
#include <vector>
#include <algorithm>

namespace rp {

CpuBasedPolicy::CpuBasedPolicy(const AppMappingSet &apps, PlatformDescription pd) :
    apps_(apps),
    platformDescription_(pd),
    hasLastPlatformLoad(false)
{
}

/*! Returns the PU that currently has lower usage. */
int CpuBasedPolicy::getLowerUsagePU()
{
    if (hasLastPlatformLoad) {
        const std::vector<int> &pus = lastPlatformLoad.getPUs();
        if (!pus.empty()) {
            std::vector<int>::const_iterator minElem = std::min_element(pus.begin(), pus.end());
            return std::distance(pus.begin(), minElem);
        }
    }
    return 0;
}

/*! Picks which PU is the most appropriate to execute a new process.
    Initially, every process is always assigned to a single PU.
    The range of PUs available for a process can later be expanded. */
int CpuBasedPolicy::pickInitialCpu()
{
    return getLowerUsagePU();
}

void CpuBasedPolicy::addApp(AppMappingPtr appMapping)
{
    pid_t pid = appMapping->getPid();
    try {
        short initialPU = pickInitialCpu();
        appMapping->setPuVector({{initialPU, initialPU}});
    } catch (exception &e) {
        // An exception can happen for a short lived process; the
        // process has died and the Workload Manager has already
        // removed the cgroup directory for the process, but the
        // Resource Policicy Manager has not yet received the
        // corresponding RemoveEvent from the WorkloadManager
        log4cpp::Category::getRoot().error("CPUBASEDPOLICY addApp PID %ld: EXCEPTION %s",
                                           (long)pid, e.what());
    }
}

void CpuBasedPolicy::removeApp(AppMappingPtr appMapping)
{

}

void CpuBasedPolicy::timer()
{
    // no action required
}

void CpuBasedPolicy::monitor(std::shared_ptr<const rmcommon::MonitorEvent> event)
{
    lastPlatformLoad = event->getPlatformLoad();
    hasLastPlatformLoad = true;
}

void CpuBasedPolicy::feedback(AppMappingPtr appMapping, int feedback)
{
    appMapping->setLastFeedback(feedback);
}

}   // namespace rp