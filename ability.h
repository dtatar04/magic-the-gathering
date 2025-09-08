#ifndef ABILITY
#define ABILITY

#include "state.h"
#include <string>
using namespace std;

class Card; 
// This is the base Ability class, which has derived classes
//   ActivatedAbility and TriggeredAbility

class Ability {
    string description;
    string cardName;

    public:
    // Ctor for Ability
    Ability(string name);

    // Copy ctor
    Ability(const Ability& other);

    // Dtor for Ability
    virtual ~Ability() = default;

    string getDescription(); // gets Ability's descrition
    void setDescription(string description); // sets Ability's description
    string getCardName(); // gets the card's name who has this ability
    void setCardName(string name); // sets the card's name who has this ability

};


#endif

