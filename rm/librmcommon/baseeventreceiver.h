#ifndef BASEEVENTRECEIVER_H
#define BASEEVENTRECEIVER_H

#include "ieventreceiver.h"
#include "threadsafequeue.h"
#include "events/baseevent.h"
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

namespace rmcommon {

/*!
 * Base class for all classes which receive and process
 * events in a separate thread.
 * \p
 * BaseEventReceiver receives BaseEvents via addEvent
 * and puts them in a queue. For each event, the processEvent
 * function is called.
 * \p
 * The thread stops when the stop() function is called or when
 * the processEvent() function returns false.
 * \p
 * Derived classes must override the function
 * \code
 * bool processEvent(.....);
 * \endcode
 */
class BaseEventReceiver : public IEventReceiver {
    rmcommon::ThreadsafeQueue<std::shared_ptr<rmcommon::BaseEvent>> queue_;
    const std::chrono::milliseconds WAIT_POP_TIMEOUT_MILLIS = std::chrono::milliseconds(5000);
    std::string threadName_;
    std::thread receiverThread_;
    std::atomic_bool stop_;

    /*! The thread function */
    void run();

protected:
    BaseEventReceiver(const char *threadName);
public:

    /*!
     * \brief Adds an event to the tread safe queue
     * \param event the event to add
     */
    virtual void addEvent(std::shared_ptr<BaseEvent> event) override;

    virtual void start();
    virtual void stop();
    virtual void join();

    /*!
     * Processes a generic event by calling the appropriate handler function.
     * \param event the event to process
     */
    virtual bool processEvent(std::shared_ptr<rmcommon::BaseEvent> event) = 0;
};

}   // namespace rmcommon

#endif // BASEEVENTRECEIVER_H