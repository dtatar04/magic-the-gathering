#ifndef OBSERVER
#define OBSERVER

#include "state.h"
#include <memory>

class Subject;

// This is the Observer class. 
// Observers can notify based on game state.

class Observer {
    public:
    // Observers can notify
    virtual void notify(std::shared_ptr<Subject> whoNotified, State gState) = 0;

    // Dtor for Observer
    virtual ~Observer() = default;
};

#endif
