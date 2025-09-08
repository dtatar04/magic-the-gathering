#ifndef UNSUMMON
#define UNSUMMON

#include "spell.h"
using namespace std;

// Unsummon is-a Spell
//   which returns target minion to its owner's hand

class Unsummon: public Spell {
    public:
    // Unsummon Ctor
    Unsummon(string name, Player& p);

    // Plays the Spell Unsummon
    void playCard(Player& ap, Player& tp, Minion& m, int i, int t) override;

};

#endif
