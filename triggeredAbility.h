#ifndef TRIGGEREDABILITY
#define TRIGGEREDABILITY

#include "ability.h"
#include "player.h"

using namespace std;

// This is the TriggeredAbility class. TriggeredAbility is-a(n) Ability
//   Triggered Abilities are activated for free when a certain condition is met

class TriggeredAbility: public Ability {
    State cond; 

public:
    // TriggeredAbility Ctors
    TriggeredAbility(string name);
    TriggeredAbility(string name, State cond);

    TriggeredAbility(const TriggeredAbility& other);

    // TriggeredAbility Dtor
    virtual ~TriggeredAbility() = default;

    State getCond(); // gets state cond
    void setCond(State cond); // sets state cond

    Minion& getMinion();

    // Use a triggered ability, given the game state condition on card is correct
    void use(Player &ap, Player &tp, int i);

};


#endif 
