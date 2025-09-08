#include "unsummon.h"
using namespace std;

// Unsummon Ctor: sets cost, description, name, owner
Unsummon::Unsummon(string name, Player& p): Spell{name, p} {
    setCost(1);
    setDescription("Return target minion to its owner's hand");
}

// Plays the spell Unsummon:
//   Returns target minion to its owner's hand
void Unsummon::playCard(Player& ap, Player& tp, Minion& m, int i, int t) {
    // If hand is full, destroys Minion, and person is not playing the card on themself
    if (tp.getSizeHand() >= FULL_HAND && ap.getPlayerNum() != tp.getPlayerNum()) {
        // Outputs a message to inform Player their action has failed
        cout << this->getName() << " failed. Hand is full. " << m.getName() << " is destroyed." << endl;
        tp.switchLocation("minions played", "destroy", t);
    
    } else if (t >= tp.getSizeMinionsPlayed()) {
        cout << "Unsummon failed. Cannot play spell on minion that does not exist." << endl;
    // Otherwise, proceed as normal
    } else {
        // Switches minion i from cards played to hand
        tp.switchLocation("minions played", "hand", t);
    }
}
