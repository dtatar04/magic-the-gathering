#ifndef RITUAL
#define RITUAL

#include "card.h"
#include "triggeredAbility.h"

// This is the Ritual Class. A Ritual is-a Card.
//   Rituals have a triggered ability, and activation cost,
//   and a number of charges.
//   A Player may only have one Ritual on the board at a time.

class Ritual: public Card {
    int charges;
    int actCost;
    unique_ptr <TriggeredAbility> ta;

public:
    // Ctor for Ritual
    Ritual(string name, Player& p);

    // Copy ctor
    Ritual(Ritual& other);

    // Dtor for Ritual
    virtual ~Ritual() = default;

    // Gets charges
    int getCharges();

    // Gets TriggeredAbility
    unique_ptr <TriggeredAbility>& getTA();

    // Gets activation cost
    int getActCost();

    // Adds i to charges
    void modCharges(int i);

    // Active player(ap) uses a Ritual at position i
    void use(Player &ap, Player& tp, int i);

    // Overrides Card's getDescription()
    string getDescription() override;

};

#endif

