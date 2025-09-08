#ifndef CARD
#define CARD

// #include "player.h"
#include <string>
#include <iostream>
#include <memory>
using namespace std;

class Player;
class Minion;
class Ritual;

class Card {
    // private fields 
    string name;
    int cost;
    string type;
    Player& owner;
    
    public:
        // Card ctor
        Card(string n, string t, Player& p); 

        // Card dtor
        virtual ~Card() = default; 

        // Copy ctor
        Card(Card& other);

        void setName(string n); // sets Card's name
        void setCost(int c); // sets Card's cost
        string getName(); // gets Card's name
        int getCost(); // gets Card's cost
        Player& getOwner(); 
        
        // for command: play i
        // overriden by: Minion, Rituals, Recharge, RaiseDead, Blizzard

        virtual void playCard(Player& ap, Player& np, int i);
        // for command: play i p t, where t = 1,2,3,4,5
        // overriden by: Enchantments, Banish, Unsummon, Disenchant
        virtual void playCard(Player& ap, Player& tp, Minion& m, int i, int t);

        // for command: play i pt, where t = r
        // overriden by: Banish
        virtual void playCard(Player& p, Ritual& r);

        // Gets the card's description 
        virtual string getDescription() = 0;

        // Gets the card type
        string getType();

};

#endif
