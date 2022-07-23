#ifndef MEMORYCONTROL_H
#define MEMORYCONTROL_H

#include <string>
#include <map>
#include "cgroupcontrol.h"

namespace pc {
/*!
 * \class a class for interacting with the cgroup memory controller
 */
class MemoryControl : public CGroupControl {
public:
    enum ControllerFile {
        CURRENT,
        MIN,
        MAX,
        EVENTS,
        STAT
    };
private:
    static const char *controllerName_;
    static const std::map<ControllerFile, const char *> fileNamesMap_;
public:

    template<typename T>
    void setValue(ControllerFile controllerFile, T value, App app) const {
        CGroupControl::setValue(controllerName_, fileNamesMap_.at(controllerFile), value, app);
    }

    void getValue(ControllerFile controllerFile, const std::string &value, App app) const {
        CGroupControl::getValue(fileNamesMap_.at(controllerFile), app);
    }

    void getValueAsInt(ControllerFile controllerFile, const std::string &value, App app) const {
        CGroupControl::getValueAsInt(fileNamesMap_.at(controllerFile), app);
    }
};

}   // namespace pc

#endif // MEMORYCONTROL_H
