#include "activatedAbility.h"
using namespace std;

// Ctor
ActivatedAbility::ActivatedAbility(string name): Ability{name} {
    if (name == "Novice Pyromancer") {
        actCost = 1;
        setDescription("Deal 1 damage to target minion");
    } else if (name == "Apprentice Summoner") {
        actCost = 1;
        setDescription("Summon a 1/1 air elemental");
    } else if (name == "Master Summoner") {
        actCost = 2;
        setDescription("Summon up to three 1/1 air elementals");
    } else {
        cout << "Something has gone wrong in activated abilities ctor" << endl;
    }
}

// Copy ctor
ActivatedAbility::ActivatedAbility(const ActivatedAbility& other): Ability{other}, actCost{other.actCost} {}

int ActivatedAbility::getActCost() {
    return actCost;
}

void ActivatedAbility::setActCost(int cost) {
    actCost = cost;
}

// use is made to express the effects of activated abililties of Minions
// called when Minion is already on the board and is used (with a use command)
// ap is the attacking player, tp is the targeted player
void ActivatedAbility::use(Player& ap, Player &tp, int i, int t) {
    // Minion has no action points
    if (ap.getMinionPlayed(i)->getActions() < 1) {
        cout << "Cannot use activated ability. Minion has no actions left." << endl;
        return;
    
    // Minion is silenced
    } else if (ap.getMinionPlayed(i)->getSilenced()) {
        cout << "Cannot use activated ability. Minion is Silenced." << endl;
        return;
    }

    if (ap.getMagic() >= getActCost()) {
        ap.modMagic(-getActCost()); // reduces ap's magic by activation cost

    // Player does not have enough magic to use activated ability
    } else {
        cout << "Cannot use activated ability. Player does not have sufficient magic." << endl;
        return;
    }

    // We can now apply the activated ability:

    // Novice Pyromancer:
    if (getCardName() == "Novice Pyromancer") {
        // Deals 1 damage to target minion
        // If affected Minion dies from this:
        if (!(tp.getMinionPlayed(t)->modDefense(-1))) {
            tp.switchLocation("minions played", "graveyard", t);
        }

    // Apprentice Summoner
    } else if (getCardName() == "Apprentice Summoner") {
        // Summons a 1/1 air elemental

        // Cannot use activated ability if ap already has 5 minions on their board
        if (ap.getSizeMinionsPlayed() >= FULL_MPLAYED) {
            cout << "Cannot use activated ability. Player already has 5 Minions on Board." << endl;
            ap.modMagic(getActCost()); // Gives ap back the activation cost
            return;
        } else {
            // Create and attach new Air Elemental to ap
            shared_ptr <Minion> airElem = make_shared <Minion> ("Air Elemental", ap);
            ap.pushToMinionsPlayed(airElem); // adding minion to board

            // Attach observer relationships 
            ap.attachObserversToMinion(airElem);
            ap.attachMinionToObservers(airElem);
            tp.attachObserversToMinion(airElem);
            tp.attachMinionToObservers(airElem);

            // If ap has Ritual Standstil active, destroy airElem
            if (ap.getSizeRitual() == 1 && ap.getRitual()->getName() == "Standstill") {
                ap.getRitual()->use(ap, tp, 0);
            } else {
                auto whoNotified = static_pointer_cast<Minion>(airElem);
                ap.notifyMinions(whoNotified, State::MinionEntersPlay);
            }
        }

    // Master Summoner
    } else if (getCardName() == "Master Summoner") {
        // Summon up to three 1/1 air elementals (given how many board slots are left)

        // Cannot use activated ability if ap already has 5 minions on their board
        if (ap.getSizeMinionsPlayed() >= FULL_MPLAYED) {
            cout << "Cannot use activated ability. Player already has 5 Minions on Board." << endl;
            ap.modMagic(getActCost()); // Gives ap back the activation cost
            return;
        } else {
            int spotsLeft = ap.getSizeMinionsPlayed();
            int inc = 0;
            for (int spot = spotsLeft; spot <= FULL_MPLAYED; spot++) {
                // Create and attach new Air Elemental to ap
                shared_ptr <Minion> airElem = make_shared <Minion> ("Air Elemental", ap);
                ap.pushToMinionsPlayed(airElem); // adding minion to board
                // Attach observer relationships 
                ap.attachObserversToMinion(airElem);
                ap.attachMinionToObservers(airElem);
                tp.attachObserversToMinion(airElem);
                tp.attachMinionToObservers(airElem);

                // If ap has Ritual Standstil active, destroy airElem
                if (ap.getSizeRitual() == 1 && ap.getRitual()->getName() == "Standstill") {
                    ap.getRitual()->use(ap, tp, 0);
                } else {
                    auto whoNotified = static_pointer_cast<Minion>(airElem);
                    ap.notifyMinions(whoNotified, State::MinionEntersPlay);
                }

               inc++;
               // Can only summon max 3 air elementals
               if (inc == 3) {
                break;
               }
            }
        }
    }

}

