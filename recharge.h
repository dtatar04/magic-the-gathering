#ifndef RECHARGE
#define RECHARGE

#include "spell.h"
using namespace std;

// Player's ritual gains 3 charges
class Recharge: public Spell {
    public:
    Recharge(string name, Player& p);
    void playCard(Player& ap, Player& np, int i) override;
};

#endif
