#ifndef PLAYER
#define PLAYER

#include <string>
#include <vector>
#include <memory>
#include "card.h"
#include "minion.h"
#include "ritual.h"
#include "subject.h"
#include "ability.h"
#include "enchantment.h"

using namespace std;

const int LIFE_START = 20;
const int MAGIC_START = 3;
const int FULL_HAND = 5;
const int FULL_MPLAYED = 5;

class Card;
class Minion;
class Ritual;

class Player {
    int playerNum;      // Player 1 or 2
    string name;        // Player's name 
    int life;           // Player's life count
    int magic;          // Player's magic count

    bool testingMode;

    vector <shared_ptr <Card>> deck;            // Player's deck
    vector <shared_ptr <Card>> hand;            // Player's hand of cards
    vector <shared_ptr <Minion>> graveyard;     // Player's graveyard of minions
    vector <shared_ptr <Minion>> minionsPlayed; // Player's minions on board
    vector <shared_ptr <Ritual>> ritual;        // Player's ritual

public:
    // Ctor
    Player (int num, string name, string deckFile, int magic, bool testing);

    // For command: attack i j
    void minionAttacksMinion(int i, int j, Player& np); 

    // For command: attack i
    void minionAttacksPlayer(int i, Player &np);

    // For command: play i p t
    void playCardWithTarget(Player& tp, int i, int t);

    // For command: play i p r (FOR RITUALS)
    void playRitual(Player& ap, Ritual& r, int i);

    // For command: play i
    void playCardNoTarget(Player& ap, Player& np, int i);

    // For command: use i p t
    void useCardWithTarget(Player& ap, Player& tp, int i, int t);

    // For command: use i
    void useCardNoTarget(Player& ap, Player& np, int i); 
    
    // Returns player number
    int getPlayerNum();

    // Returns player name
    string getName();
    
    // Returns life field
    int getLife();

    // Sets life field
    void setLife();
    
    // Modifies life field
    void modLife(int i);

    // Returns magic field
    int getMagic();
    
    // Modifies magic field
    void modMagic(int i);

    // Returns number of cards in deck
    int getSizeDeck();

    // Returns number of cards in hand
    int getSizeHand();

    // Returns number of minions in graveyard
    int getSizeGraveyard();

    // Returns number of minions on player's board
    int getSizeMinionsPlayed();

    // Returns number of rituals on player's board; 
    //      Should always be either 0 or 1
    int getSizeRitual();

    // Sets player's ritual, and gets rid of the old ritual
    void setRitual(shared_ptr<Ritual> &r);

    // Returns pointer to card i in deck
    shared_ptr <Card> getCardDeck (int i);

    // Returns pointer to card i in hand
    shared_ptr <Card> getCardHand (int i);

    // Returns pointer to minion i in graveyard
    shared_ptr <Minion> getMinionGraveyard (int i);

    // Returns pointer to minion i in minionsPlayed
    shared_ptr <Minion> getMinionPlayed (int i);

    // Returns pointer to Ritual on board
    shared_ptr <Ritual> getRitual();

    // Returns card i and removes it from v (deck or hand)
    shared_ptr <Card> popCard (vector <shared_ptr <Card>> &v, int i);

    // Returns minion i and removes it from v (graveyard or minionsPlayed)
    shared_ptr <Card> popCard (vector <shared_ptr <Minion>> &v, int i);

    // Adds card c to end of v
    void pushCard (vector <shared_ptr <Card>> &v, shared_ptr <Card> &c);

    // Adds minion m to end of v
    void pushCard (vector <shared_ptr <Minion>> &v, shared_ptr <Minion> &m);

    // Adds minion m to top of graveyard
    void pushToGraveyard (shared_ptr <Minion> &m);

    // Adds minion m to end of minionsPlayed
    void pushToMinionsPlayed (shared_ptr <Minion> &m);

    // moves card i from location 1 to location 2
    void switchLocation (string cs1, string cs2, int i);

    // Initializes deck
    void initDeck (string deckFile, bool testing);

    // Helper function to shuffle deck
    void shuffleDeck ();

	// Draws top card from deck to hand
	void draw();

	// Discards the ith card in the player’s hand, simply removing it from 
	//		their hand (the card does not go to the graveyard, trigger leave 
	// 		play effects or anything else)
	void discard(int i);

    // Restores minion actions at start of turn
    void restoreActionsToMinions();

    // Observer methods
    void notifyMinions(shared_ptr<Subject> whoNotified, State gState);
    void attachObserversToMinion(shared_ptr<Card>  newMinion);
    void attachMinionToObservers(shared_ptr<Card>  newMinion);
};

#endif
