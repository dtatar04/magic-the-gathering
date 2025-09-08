#include "recharge.h"
using namespace std;

// Ctor
Recharge::Recharge(string name, Player& p): Spell{name, p} {
    setCost(1);
    setDescription("Your ritual gains 3 charges");
}

// Plays Recharge
void Recharge::playCard(Player& ap, Player& np, int i) {
    if (ap.getSizeRitual() >= 1) {
        ap.getRitual()->modCharges(3);
    } else {
        cout << "Cannot play Recharge on a player with no ritual" << endl;
    }
}


