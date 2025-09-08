#include "ritual.h"

// Ritual Ctor. Takes in a name and the Player* owning it
//   Sets Card cost, charges, activation cost, and TriggerdAbility
Ritual::Ritual(string name, Player& p): Card{name, "Ritual", p} {
    // Constructs the Ritual "Dark Ritual"
    if (name == "Dark Ritual") {
        setCost(0);
        charges = 5;
        actCost = 1;
        ta = make_unique<TriggeredAbility>(name, State::StartOfTurn);

    // Constructs the Ritual "Aura of Power"
    } else if (name == "Aura of Power") {
        setCost(1);
        charges = 4;
        actCost = 1;
        ta = make_unique<TriggeredAbility>(name, State::MinionEntersPlay);

    // Constructs the Ritual "Standstill"
    } else if (name == "Standstill") {
        setCost(3);
        charges = 4;
        actCost = 2;
        ta = make_unique<TriggeredAbility>(name, State::MinionEntersPlay);
    }
}

// Copy ctor
Ritual::Ritual(Ritual& other) : Card{other.getName(), other.getType(), other.getOwner()},
    charges{other.getCharges()}, actCost{other.getActCost()} {
        ta = make_unique<TriggeredAbility>(*other.ta);
}

// Gets description of the Ritual's TriggeredAbility
string Ritual::getDescription() {
    return ta->getDescription();
}

// Gets charges
int Ritual::getCharges() {
    return charges;
}

// Adds i to charges
void Ritual::modCharges(int i) {
    charges += 3;
}

// Gets the TriggeredAbility
unique_ptr <TriggeredAbility>& Ritual::getTA() {
    return ta;
}

// Gets activation cost
int Ritual::getActCost() {
    return actCost;
}

// Active player(ap) uses a Ritual at position i
void Ritual::use(Player &ap, Player& tp, int i) {
    int affectedMinion = ap.getSizeMinionsPlayed() - 1; // Most recently played minion pos

    // Determine if Ritual has enough charges left
    if (actCost > charges) {
        cout << "A ritual was triggered, but it did not have enough charges to activate" << endl;
        return;
    } else {
        charges -=actCost;
    }

    // We can now determine which Ritual to play

    // Dark Ritual: At the start of your turn, gain 1 mana
    if (getName() == "Dark Ritual") {
        ap.modMagic(1);

    // Aura of Power: Whenever a minion enters play under your control, it gains +1/+1
    } else if (getName() == "Aura of Power") {
        ap.getMinionPlayed(affectedMinion)->modAttack(1);
        ap.getMinionPlayed(affectedMinion)->modDefense(1);

    // Standstill: Whenever a minion enters play, destroy it
    } else if (getName() == "Standstill") {
        ap.switchLocation("minions played", "destroy", affectedMinion);
    } else {
        cout << "The ritual played does not exist" << endl;
    }
}

