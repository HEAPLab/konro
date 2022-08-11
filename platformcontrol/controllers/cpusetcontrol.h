#ifndef CPUSETCONTROL_H
#define CPUSETCONTROL_H

#include "../cgroupcontrol.h"
#include <string>
#include <map>

namespace pc {
/*!
 * \class a class for interacting with the cgroup cpuset controller
 */
class CpusetControl : public CGroupControl {
public:
    enum ControllerFile {
        CPUS,               // read-write
        CPUS_EFFECTIVE,     // read-only
        MEMS,               // read-write
        MEMS_EFFECTIVE      // read-only
    };
private:
    static const char *controllerName_;
    static const std::map<ControllerFile, const char *> fileNamesMap_;
public:

    /*!
     * Requests the use of a set of cpus by the application.
     *
     * The cpu numbers are expressed through a comma-separated list of numbers
     * or ranges, where ranges are represented through dashes.
     * An empty value indicates that the cgroup is using the same setting as the
     * nearest cgroup ancestor with a non-empty cpuset.cpus or all the available
     * cpus if none is found.
     *
     * \example 0-2,16 represents CPUs 0, 1, 2 and 16
     *
     * \param cpus the list of requested cpus
     * \param app the application to limit
     */
    void setCpusetCpus(std::string cpus, App app);

    /*!
     * Returns the list of cpus that are granted to the specified application
     * \param app the application of interest
     * \returns the cpus available for use by the application
     */
    std::string getCpusetCpusEffective(App app);

    /*!
     * Requests the use of a set of memory nodes by the application.
     *
     * The memory node numbers are expressed through a comma-separated list of numbers
     * or ranges, where ranges are represented through dashes.
     * An empty value indicates that the cgroup is using the same setting as the
     * nearest cgroup ancestor with a non-empty cpuset.mems or all the available
     * memory nodes if none is found.
     *
     * \example 0-2,16 represents memory nodes 0, 1, 2 and 16
     *
     * \note Setting a non-empty value to cpuset.mems causes memory of tasks within
     * the cgroup to be migrated to the designated nodes if they are currently using
     * memory outside of the designated nodes.
     *
     * \param memNodes the list of requested memory nodes
     * \param app the application to limit
     */
    void setCpusetMems(std::string memNodes, App app);

    /*!
     * Returns the list of memory nodes that are granted to the specified application
     * \param app the application of interest
     * \returns the memory nodes available for use by the application
     */
    std::string getCpusetMemsEffective(App app);

};

}   // namespace pc
#endif // CPUSETCONTROL_H
