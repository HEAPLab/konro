#include "policymanager.h"
#include "policies/nopolicy.h"
#include "policies/randpolicy.h"
#include "threadname.h"
#include "eventbus.h"
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

namespace rp {

/*!
 * Compares two AppMapping ("less" function) handled
 * by shared pointers
 *
 * \param lhs the first app to compare
 * \param rhs the second app to compare
 * \return true if pid of lsh is < than pid of rhs
 */
static bool appMappingComp(const shared_ptr<AppMapping> &lhs, const shared_ptr<AppMapping> &rhs)
{
    return lhs->getPid() < rhs->getPid();
}

PolicyManager::PolicyManager(rmcommon::EventBus &bus, PlatformDescription pd, Policy policy) :
    rmcommon::BaseEventReceiver("POLICYMANAGER"),
    cat_(log4cpp::Category::getRoot()),
    bus_(bus),
    platformDescription_(pd),
    apps_(appMappingComp)
{
    subscribeToEvents();
    policy_ = makePolicy(policy);
}

std::unique_ptr<IBasePolicy> PolicyManager::makePolicy(Policy policy)
{
    switch (policy) {
    case Policy::RandPolicy:
        return make_unique<RandPolicy>(platformDescription_);
    case Policy::NoPolicy:
    default:
        return make_unique<NoPolicy>();
        break;
    }
}

PolicyManager::Policy PolicyManager::getPolicyByName(const std::string &policyName)
{
    if (policyName == "RandPolicy")
        return Policy::RandPolicy;
    else
        return Policy::NoPolicy;
}

void PolicyManager::subscribeToEvents()
{
    using namespace rmcommon;

    bus_.subscribe<PolicyManager, AddEvent, BaseEvent>(this, &PolicyManager::addEvent);
    bus_.subscribe<PolicyManager, RemoveEvent, BaseEvent>(this, &PolicyManager::addEvent);
    bus_.subscribe<PolicyManager, TimerEvent, BaseEvent>(this, &PolicyManager::addEvent);
    bus_.subscribe<PolicyManager, FeedbackEvent, BaseEvent>(this, &PolicyManager::addEvent);
    bus_.subscribe<PolicyManager, MonitorEvent, BaseEvent>(this, &PolicyManager::addEvent);
}

bool PolicyManager::processEvent(std::shared_ptr<const rmcommon::BaseEvent> event)
{
    using namespace rmcommon;

#if 1
    ostringstream os;
    os << "POLICYMANAGER received message => " << *event;
    cat_.debug(os.str());
#endif

    if (const AddEvent *e = dynamic_cast<const AddEvent *>(event.get())) {
        processAddEvent(static_pointer_cast<const AddEvent>(event));
    } else if (const RemoveEvent *e = dynamic_cast<const RemoveEvent *>(event.get())) {
        processRemoveEvent(static_pointer_cast<const RemoveEvent>(event));
    } else if (const TimerEvent *e = dynamic_cast<const TimerEvent *>(event.get())) {
        processTimerEvent(static_pointer_cast<const TimerEvent>(event));
    } else if (const MonitorEvent *e = dynamic_cast<const MonitorEvent *>(event.get())) {
        processMonitorEvent(static_pointer_cast<const MonitorEvent>(event));
    } else if (const FeedbackEvent *e = dynamic_cast<const FeedbackEvent *>(event.get())) {
        processFeedbackEvent(static_pointer_cast<const FeedbackEvent>(event));
    }
    return true;        // continue processing
}

void PolicyManager::processAddEvent(std::shared_ptr<const rmcommon::AddEvent> event)
{
    cat_.debug("POLICYMANAGER AddEvent received");
    shared_ptr<AppMapping> appMapping = make_shared<AppMapping>(event->getApp());
    apps_.insert(appMapping);
    dumpApps();
    policy_->addApp(appMapping);
}

void PolicyManager::processRemoveEvent(std::shared_ptr<const rmcommon::RemoveEvent> event)
{
    cat_.debug("POLICYMANAGER RemoveProc event received");
    // search target
    shared_ptr<AppMapping> appMapping = make_shared<AppMapping>(event->getApp());
    auto it = apps_.find(appMapping);
    if (it != end(apps_)) {
        policy_->removeApp(*it);
        apps_.erase(it);
    }
    dumpApps();
}

void PolicyManager::processTimerEvent(std::shared_ptr<const rmcommon::TimerEvent> event)
{
    cat_.debug("POLICYMANAGER timer event received");
    policy_->timer();
}

void PolicyManager::processMonitorEvent(std::shared_ptr<const rmcommon::MonitorEvent> event)
{
    cat_.debug("POLICYMANAGER monitor event received");
    policy_->monitor(event);
}

void PolicyManager::processFeedbackEvent(std::shared_ptr<const rmcommon::FeedbackEvent> event)
{
    cat_.debug("POLICYMANAGER feedback event received");
    policy_->feedback(event);
}

void PolicyManager::dumpApps() const
{
    std::ostringstream os;
    os << "POLICYMANAGER handling PIDS [";
    bool first = true;
    for (auto &app: apps_) {
        if (first)
            first = false;
        else
            os << ",";
        os << app->getPid();
    }
    os << "]";
    cat_.info(os.str());
}

}   // namespace rp
