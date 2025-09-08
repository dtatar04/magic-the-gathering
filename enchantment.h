#ifndef ENCHANTMENT
#define ENCHANTMENT

#include "card.h"

using namespace std;

class Enchantment: public Card {
    string enDesc;
    char atkOp;
    char defOp;
    int modValue;

public:
    // Enchantment Ctor
    Enchantment(string name, Player& p);

    // Enchantment Copy Ctor
    Enchantment(Enchantment& other);

    // Enchantment Dtor
    virtual ~Enchantment() = default;
    
    // Overrides Card's getDescription
    string getDescription() override;
    
    // get private field info
    char getAtkOp();
    char getDefOp();
    int getModValue();

};

#endif
