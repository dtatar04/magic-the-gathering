#include "raiseDead.h"
using namespace std;

// Ctor
RaiseDead::RaiseDead(string name, Player& p): Spell{name, p} {
    setCost(1);
    setDescription("Resurrect the top minion in your graveyard and set its defence to 1");
}

// Resurrects the top minion in Player's graveyard and sets 
//   its defense to 1
//   Resurrect moves the Minion from graveyard to Hand
void RaiseDead::playCard(Player& ap, Player& np, int i) {
    // If there is no minion in Player's graveyard, do nothing
    if (ap.getSizeGraveyard() == 0){
        // Outputs a message to inform Player their action has failed 
        cout << this->getName() << " failed. There are no minions in player's graveyard." << endl;

    // If hand is full, destroys Minion
    } else if (ap.getSizeHand() >= FULL_HAND) {
        // Outputs a message to inform Player their action has failed
        cout << this->getName() << " failed. Hand is full. " << ap.getMinionGraveyard(0)->getName() << " is destroyed." << endl;
        ap.switchLocation("graveyard", "destroy", 0); // Destroys top Minion
    
    // Otherwise, proceed as normal
    } else {
        // Switches top minion in graveyard to hand
        ap.getMinionGraveyard(0)->setDefense(1);
        ap.switchLocation("graveyard", "hand", 0); 
    }  
}

