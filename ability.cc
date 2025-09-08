#include "ability.h"
using namespace std;

// Ctor for Ability
Ability::Ability(string name): cardName{name} {}

Ability::Ability(const Ability& other): cardName{other.cardName}, description{other.description} {}

// gets description of ability
string Ability::getDescription() {
    return description;
}

// Sets description of ability
void Ability::setDescription(string description) {
    this->description = description;
}

// gets the card's name who has this ability
string Ability::getCardName() {
    return cardName;
}

// sets the card's name who has this ability
void Ability::setCardName(string name) {
    cardName = name;
}

