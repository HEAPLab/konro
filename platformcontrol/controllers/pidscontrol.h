#ifndef PIDSCONTROL_H
#define PIDSCONTROL_H

#include "../cgroupcontrol.h"
#include "../numericvalue.h"
#include <string>
#include <map>

namespace pc {
/*!
 * \class a class for interacting with the cgroup PIDs controller
 */
class PidsControl : public CGroupControl {
public:
    enum ControllerFile {
        MAX,        // read-write
        CURRENT     // read-only
    };
    const int MAX_NUM_PIDS = -1;
private:
    static const char *controllerName_;
    static const std::map<ControllerFile, const char *> fileNamesMap_;
public:

    /*!
     * Limits the number of processes that may be forked by the specified application.
     * To remove all limits, the caller must pass the MAX_NUM_PIDS constant as parameter.
     * \param numPids the maximum number of processes that can be forked
     * \param app the application to limit
     */
    void setPidsMax(NumericValue numPids, App app);

    /*!
     * Gets the maximum number of processes that may be forked by the application.
     * The function returns the MAX_NUM_PIDS constant if no upper bound is set.
     * \param app the application of interest
     * \returns the maximum number of processes that can be froked by the application
     */
    NumericValue getPidsMax(App app);

    /*!
     * Gets the number of processes in the cgroup where the application is located and
     * all its descendants.
     * \param app the application of interest
     * \returns the number of processes in the app's cgroup and descendants
     */
    NumericValue getPidsCurrent(App app);
};

}   // namespace pc
#endif // PIDSCONTROL_H
