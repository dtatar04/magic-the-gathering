#include "subject.h"
#include "minion.h"

#include <algorithm>

using namespace std;

// Add new observer to observers
void Subject::attach(shared_ptr<Observer> o) {
    observers.push_back(o);
}

// notify all relevant observers of the game state change
void Subject::notifyObservers(State gState) {            
    for (auto &ob : observers) {
        auto observerMin = dynamic_pointer_cast<Minion>(ob);
        // observerMin needs to have a triggered ability, be on board (N/A)
        //   and match state to notify
        if (observerMin && observerMin->getBTA() && // observerMin->getOnBoard() &&
            observerMin->getTriggeredAbility()->getCond() == gState) {
            observerMin->notify(gState);
        }
    }
}


// Get number of observers
int Subject::getNumofObservers() {
    return observers.size();
}

