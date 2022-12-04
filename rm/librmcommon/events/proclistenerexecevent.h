#ifndef PROCLISTENEREXECEVENT_H
#define PROCLISTENEREXECEVENT_H

#include "baseevent.h"
#include <vector>
#include <cstdint>

namespace rmcommon {

/*!
 * \class Exec event generated by the ProcListener
 */
class ProcListenerExecEvent : public BaseEvent {
public:
    std::vector<std::uint8_t> data_;

    ProcListenerExecEvent(const uint8_t *data, size_t len) :
        BaseEvent("ProcListenerExecEvent"),
        data_(data, data+len)
    {
    }

    virtual void printOnOstream(std::ostream &os) const {
        os << "{}";
    }
};

}   // namespace rmcommon

#endif // PROCLISTENEREXECEVENT_H
