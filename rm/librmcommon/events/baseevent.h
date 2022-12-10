#ifndef BASEEVENT_H
#define BASEEVENT_H

#include <iostream>
#include <string>

namespace rmcommon {

/*!
 * \class a generic event related to a process managed by Konro
 */
class BaseEvent {
    std::string name_;
public:
    BaseEvent(const char *name) : name_(name) {
    }

    std::string getName() const {
        return name_;
    }

    virtual void printOnOstream(std::ostream &os) const {
        os << "{}";
    }

    friend std::ostream &operator <<(std::ostream &os, const BaseEvent &event) {
        event.printOnOstream(os);
        return os;
    }
};

}   // namespace rmcommon


#endif // BASEEVENT_H
