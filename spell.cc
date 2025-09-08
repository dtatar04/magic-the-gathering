#include "spell.h"
using namespace std;

// Spell Ctor
Spell::Spell(string name, Player& p): Card{name, "Spell", p} {}

Spell::Spell(Spell& other): Card{other.getName(), other.getType(), other.getOwner()},
    description{other.getDescription()} {}

// Sets description
void Spell::setDescription(string desc) {
    description = desc;
}

// Gets description
string Spell::getDescription() {
    return description;
}


// Dummy playCard(). Should not be called
void Spell::playCard(Player& ap, Player& np, int i) {
    cout << "Bad. Wrong caller of playCard. From Spell" << endl;
}

// Dummy playCard(). Should not be called
void Spell::playCard(Player& ap, Player& tp, Minion& m, int i, int t) {
    cout << "Bad. Wrong caller of playCard. From Spell" << endl;
}

void Spell::playCard(Player& p, Ritual& r) {
    cout << "You did smt wrong :()" << endl;
}
