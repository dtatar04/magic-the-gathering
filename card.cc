#include "card.h"
using namespace std;

Card::Card(string n, string t, Player& p): name{n}, type{t}, owner{p} {}

Player& Card::getOwner() {
    return owner;
}

// copy ctor
Card::Card (Card & other) : name{other.name}, owner{other.owner},
    type{other.type}, cost{other.cost} {}


// sets Card's name
void Card::setName(string n) { name = n; }

// sets Card's cost
void Card::setCost(int c) { cost = c; }

// gets Card's name
string Card::getName() { return name; }

// gets Card's cost
int Card::getCost() { return cost; }

void Card::playCard(Player& ap, Player& np, int i) {
    cout << "Bad. Wrong caller of playCard" << endl;
}

void Card::playCard(Player& ap, Player& tp, Minion& m, int i, int t) {
    cout << "Bad." << endl;
}

void Card::playCard(Player& p, Ritual& r) {
    cout << "You're not Banish. Can't play";
}

string Card::getType() {
    return type;
}
