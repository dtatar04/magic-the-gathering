#ifndef RAISEDEAD
#define RAISEDEAD

#include "spell.h"
using namespace std;

// Resurrects the top minion in Player's graveyard and sets 
//   its defense to 1
class RaiseDead: public Spell {
    public:
    RaiseDead(string name, Player& p);
    void playCard(Player& ap, Player& np, int i) override;
};

#endif
