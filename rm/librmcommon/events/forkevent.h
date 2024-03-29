#ifndef FORKEVENT_H
#define FORKEVENT_H

#include "baseevent.h"
#include <string>
#include <vector>
#include <cstdint>

namespace rmcommon {

/*!
 * \class fork event generated by the ProcListener
 */
class ForkEvent : public BaseEvent {
public:
    std::vector<std::uint8_t> data_;

    ForkEvent(const uint8_t *data, size_t len) :
        BaseEvent("ForkEvent"),
        data_(data, data+len)
    {
    }

    virtual void printOnOstream(std::ostream &os) const {
        os << "{}";
    }
};

}   // namespace rmcommon

#endif // FORKEVENT_H
