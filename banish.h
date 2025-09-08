#ifndef BANISH
#define BANISH
#include "spell.h"
using namespace std;

// Destroys target minion or ritual
class Banish: public Spell {
    public:
    Banish(string name, Player& p);
    void playCard(Player& ap, Player& tp, Minion& m, int i, int t) override; // Banishes a Minion
    void playCard(Player& p, Ritual& r) override; // Banishes a Ritual
};

#endif
