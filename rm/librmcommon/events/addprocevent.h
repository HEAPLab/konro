#ifndef ADDPROCEVENT_H
#define ADDPROCEVENT_H

#include "baseevent.h"
#include "../app.h"
#include <memory>
#include <iostream>

namespace rmcommon {

/*!
 * \class event generated by the addition of a new process to Konro
 */
class AddProcEvent : public BaseEvent {

    std::shared_ptr<rmcommon::App> app_;

public:

    AddProcEvent(std::shared_ptr<rmcommon::App> app) : app_(app) {}

    std::shared_ptr<rmcommon::App> getApp() const {
        return app_;
    }

    void printOnOstream(std::ostream &os) const override {
        os << "{\"pid\":" << app_->getPid() << "}";
    }
};

}   // namespace rmcommon

#endif // ADDPROCEVENT_H
