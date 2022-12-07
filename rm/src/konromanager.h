#ifndef KONROMANAGER_H
#define KONROMANAGER_H

#include <string>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

namespace wm {
    class ProcListener;
    class WorkloadManager;
}

namespace http {
    class KonroHttp;
}

namespace rp {
    class PolicyManager;
}

class KonroManager {
    log4cpp::Category &cat_;
    wm::ProcListener *procListener_;
    wm::WorkloadManager *workloadManager_;
    http::KonroHttp *http_;
    rp::PolicyManager *policyManager_;

    std::string cfgPolicyName_;
    int cfgTimerSeconds_;       // 0 means "no timer"
    int cfgMonitorPeriod_;
    std::string cfgCpuModuleNames_;
    std::string cfgBatteryModuleNames_;

    std::string configFilePath();
    void setupLogging();
    void loadConfiguration();
public:
    KonroManager();

    /*!
     * Configures and starts the ProcListener in the main thread
     * \param pidToMonitor
     */
    void run(long pidToMonitor);

    /*!
     * Stops the ProcListener
     */
    void stop();

    void testPlatformDescription();
};

#endif // KONROMANAGER_H