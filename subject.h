#ifndef SUBJECT
#define SUBJECT

#include "state.h"
#include "observer.h"
#include <vector>
#include <memory>

using namespace std;

class Observer;

// This is the Subject class

class Subject {
    vector<shared_ptr<Observer>> observers;

    public:

    // Add new observer to observers
    void attach(shared_ptr<Observer> o);

    // Notify observers based on the game state
    void notifyObservers(State gState);

    // Get number of observers
    int getNumofObservers();

    virtual ~Subject() = default;

};

#endif 
