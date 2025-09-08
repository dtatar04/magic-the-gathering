#ifndef SPELL
#define SPELL

#include "card.h"
#include "minion.h"
#include "player.h"
// #include "ritual.h"
using namespace std;

// Spell is-a Card which changes the game in some way
//   and is then removed from the game.

class Spell: public Card {
    string description;

protected:
    // Set description
    void setDescription(string description);
        
public:
    // Spell Ctor
    Spell(string name, Player& p);

    // Copy Ctor
    Spell(Spell& other);

    // Spell Dtor
    virtual ~Spell() = default;

    // for command: play i
    // overriden by: Minion, Rituals, Recharge, RaiseDead, Blizzard
    virtual void playCard(Player& ap, Player& np, int i) override;

    // for command: play i p t, where t = 1,2,3,4,5
    // overriden by: Enchantments, Banish, Unsummon, Disenchant
    virtual void playCard(Player& ap, Player& tp, Minion& m, int i, int t) override;

    // for command: play i pt, where t = r
    // overriden by: Banish
    virtual void playCard(Player& p, Ritual& r) override;

    string getDescription() override; // overides Card's getDescription()
};

#endif

