#ifndef EXECEVENT_H
#define EXECEVENT_H

#include "baseevent.h"
#include <vector>
#include <cstdint>

namespace rmcommon {

/*!
 * \class exec event generated by the ProcListener
 */
class ExecEvent : public BaseEvent {
public:
    std::vector<std::uint8_t> data_;

    ExecEvent(const uint8_t *data, size_t len) :
        BaseEvent("ExecEvent"),
        data_(data, data+len)
    {
    }

    virtual void printOnOstream(std::ostream &os) const {
        os << "{}";
    }
};

}   // namespace rmcommon

#endif // EXECEVENT_H
