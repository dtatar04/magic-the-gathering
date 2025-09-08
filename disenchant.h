#ifndef DISENCHANT
#define DISENCHANT

#include "spell.h"
using namespace std;

// Destroys the top enchantment on target minion
class Disenchant: public Spell {
    public:
    Disenchant(string name, Player& p);
    void playCard(Player& ap, Player& tp, Minion& m, int i, int t) override;
};

#endif
