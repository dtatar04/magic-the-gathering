#include "blizzard.h"
using namespace std;

// Ctor
Blizzard::Blizzard(string name, Player& p): Spell{name, p} {
    setCost(3);
    setDescription("Deal 2 damage to all minions");
}

// Plays Blizzard
void Blizzard::playCard(Player& ap, Player& np, int i) {
    // Decreases all of active player's minions defense by 2
    for(int i = 0; i < ap.getSizeMinionsPlayed(); i++) {
        if (!ap.getMinionPlayed(i)->modDefense(-2)) {
            ap.switchLocation("minions played", "graveyard", i);
        }
        
    }
    // Decreases all of non-active player's minions defense by 2
    for(int i = 0; i < np.getSizeMinionsPlayed(); i++) {
        if (!np.getMinionPlayed(i)->modDefense(-2)) {
            np.switchLocation("minions played", "graveyard", i);
        }
    }
}

