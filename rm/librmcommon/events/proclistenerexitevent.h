#ifndef PROCLISTENEREXITEVENT_H
#define PROCLISTENEREXITEVENT_H

#include "baseevent.h"
#include <vector>
#include <cstdint>

namespace rmcommon {

/*!
 * \class Exit event generated by the ProcListener
 */
class ProcListenerExitEvent : public BaseEvent {
public:
    std::vector<std::uint8_t> data_;

    ProcListenerExitEvent(const uint8_t *data, size_t len) :
        BaseEvent("ProcListenerExitEvent"),
        data_(data, data+len)
    {
    }

    virtual void printOnOstream(std::ostream &os) const {
        os << "{}";
    }
};

}   // namespace rmcommon

#endif // PROCLISTENEREXITEVENT_H
