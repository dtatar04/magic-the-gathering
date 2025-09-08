#ifndef ACTIVATEDABILITY
#define ACTIVATEDABILITY

#include "ability.h"
#include "player.h"

class Minion;

// This is the ActivatedAbility class, which is-a(n) Ability
//   Activated abilities cost magic and an action point to use

class ActivatedAbility: public Ability {
    int actCost; 

public:
    // Ctors for ActivatedAbility
    ActivatedAbility(string name);
//    ActivatedAbility(string name, int cost);

    // Copy ctor
    ActivatedAbility(const ActivatedAbility& other);

    // Dtor for ActivatedAbility
    virtual ~ActivatedAbility() = default;

    int getActCost(); // sets activation cost
    void setActCost(int cost); // gets activation cost

    // use applies the activated ability from the active player (ap) on the target player's (tp) 
    //   card at position t
    void use(Player& ap, Player &tp, int i, int t);
};

#endif
