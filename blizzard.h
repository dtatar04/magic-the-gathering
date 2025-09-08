#ifndef BLIZZARD
#define BLIZZARD
#include "spell.h"
using namespace std;

// Deals 2 damage to all minions
class Blizzard: public Spell {
public:
        Blizzard(string name, Player& p);
        void playCard(Player& ap, Player& np, int i) override;
};

#endif
