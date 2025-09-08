#include "enchantment.h"

// Ctor
Enchantment::Enchantment(string name, Player& p): Card{name, "Enchantment", p}, atkOp{' '}, defOp{' '}, modValue{0} {
    // Constructs a Giant Strength Enchantment
    if (name == "Giant Strength") {
        setCost(1);
        enDesc = "";
        atkOp = '+';
        defOp = '+';
        modValue = 2;

    // Constructs an Enrage Enchantment
    } else if (name == "Enrage") {
        setCost(2);
        enDesc = "";
        atkOp = '*';
        defOp = '*';
        modValue = 2;

    // Constructs a Haste Enchantment
    } else if (name == "Haste") {
        setCost(1);
        enDesc = "Enchanted minion gains +1 action each turn";

    // Constructs a Magic Fatigue Enchantment
    } else if (name == "Magic Fatigue") {
        setCost(0);
        enDesc = "Enchanted minion's activated ability costs 2 more";

    // Constructs a Silence Enchantment
    } else if (name == "Silence") {
        setCost(1);
        enDesc = "Enchanted minion cannot use abilities";
    }
}


// Copy ctor
Enchantment::Enchantment(Enchantment& other): Card{other.getName(), other.getType(), other.getOwner()}, 
    enDesc{other.getDescription()}, atkOp{other.getAtkOp()}, defOp{other.getDefOp()},
    modValue{other.getModValue()} {}



string Enchantment::getDescription() {
    return enDesc;
}
    
char Enchantment::getAtkOp() {
    return atkOp;
}

char Enchantment::getDefOp() {
    return defOp;
}

int Enchantment::getModValue() {
    return modValue;
}
