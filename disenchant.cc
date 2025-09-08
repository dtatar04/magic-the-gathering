#include "disenchant.h"
using namespace std;

// Ctor
Disenchant::Disenchant(string name, Player& p): Spell{name, p} {
    setCost(1);
    setDescription("Destroy the top enchantment on target minion");
}

// Plays Disenchant
void Disenchant::playCard(Player& ap, Player& tp, Minion& m, int i, int t) {
    m.removeEnchantment();
}

