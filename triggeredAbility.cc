#include "triggeredAbility.h"

using namespace std;

// TriggeredAbility Ctor, takes in only a name
//   Sets gState and description as well
TriggeredAbility::TriggeredAbility(string name): Ability{name} {
     if (name == "Bone Golem") {
        setDescription("Gain +1/+1 whenever a minion leaves play.");
        cond = State::MinionExitsPlay;
    } else if (name == "Potion Seller") {
        setDescription("At the end of your turn, all your minions gain +0/+1.");
        cond = State::EndOfTurn;
    } else if (name == "Fire Elemental") {
        setDescription("Whenever an opponent's minion enters play, deal 1 damage to it.");
        cond = State::MinionEntersPlay;
    } 
}

// TriggeredAbility Ctor, takes in a name and game state condition
//   Sets description as well
TriggeredAbility::TriggeredAbility(string name, State cond): Ability{name}, cond{cond} {
    if (name == "Bone Golem") {
        setDescription("Gain +1/+1 whenever a minion leaves play.");

    } else if (name == "Potion Seller") {
        setDescription("At the end of your turn, all your minions gain +0/+1.");
    
    } else if (name == "Fire Elemental") {
        setDescription("Whenever an opponent's minion enters play, deal 1 damage to it.");
    
    } else if (name == "Dark Ritual") {
        setDescription("At the start of your turn, gain 1 mana");
    
    } else if (name == "Aura of Power") {
        setDescription("Whenever a minion enters play under your control, it gains +1/+1");
    
    } else if (name == "Standstill") {
        setDescription("Whenever a minion enters play, destroy it");
    }
}

// Copy ctor
TriggeredAbility::TriggeredAbility(const TriggeredAbility& other) : Ability{other},
    cond{other.cond} {}

// Gets state condition
State TriggeredAbility::getCond() {
    return cond;
}

// Sets state condition
void TriggeredAbility::setCond(State cond) {
    this->cond = cond;
}

// Use a triggered ability, given the game state condition on card is correct
void TriggeredAbility::use(Player &ap, Player &tp, int i) {
    // Minion is silenced
     if (ap.getMinionPlayed(i)->getSilenced()) {
        cout << "Minion's ability was triggered but cannot be used due to Silence." << endl;
        return;
    }

    // Bone Golem implementation
    if (this->getCardName() == "Bone Golem") {
        // Bone Golem gains +1/+1
        ap.getMinionPlayed(i)->modAttack(1);
        ap.getMinionPlayed(i)->modDefense(1);
        
    // Potion Seller Implementation
    } else if (this->getCardName() == "Potion Seller") {
        // All minions gain +0/+1
        for (int j = 0; j <ap.getSizeMinionsPlayed(); j ++) {
            ap.getMinionPlayed(j)->modDefense(1);
        }

    // Fire Elemental Implementation
    } else if (this->getCardName() == "Fire Elemental") {
        // Deal 1 damage whenever an opponent's minion enters play
         if (tp.getSizeMinionsPlayed() > 0) {
            int lastMPos = tp.getSizeMinionsPlayed() - 1;
            bool stillAlive = tp.getMinionPlayed(lastMPos)->modDefense(-1);
            // If minion died, remove it from board to graveyard
            if (!stillAlive) {
                tp.switchLocation("minions played", "graveyard", lastMPos);
            }
         } else {
            cout << "Error in Fire elemental" << endl;
         }
    }
}
