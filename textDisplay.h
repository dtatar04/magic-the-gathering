#ifndef TEXT_DISPLAY
#define TEXT_DISPLAY

#include <iostream>
#include <string>
#include <vector>
#include "displays.h"
#include "player.h"
#include "card.h"
#include "spell.h"
#include "minion.h"
#include "enchantment.h"
#include "ritual.h"

using namespace std;

class TextDisplay: public Displays {
	const int CARD_WIDTH = 33;
	const int CARD_HEIGHT = 11;
	const int MAX_CARD_ROW = 5;

	// prints the top/bottom border of game board 
	string printBorder (bool isTop);

	// prints the side border of game board
	string printSide ();
	
	// displays game name on game board
	vector <string> displayTitle();

	// displays player card
	vector <string> displayPlayer(int playerNum);
	
    // displays card
    vector<string> displayCard(shared_ptr <Card> &c);

    // displays spell
    vector<string> displaySpell(shared_ptr <Spell> &s);

    // displays minion
    vector<string> displayMinion(shared_ptr <Minion> &m);

    // displays enchantment
    vector<string> displayEnchantment(shared_ptr <Enchantment> &e);

    // displays ritual
    vector<string> displayRitual(shared_ptr <Ritual> &r);

	// displays player's minions that have been played
	vector <vector<string>> displayMinionsPlayed (int playerNum);

	// displays graveyard, player, and ritual cards
	vector <vector <string>> displayEdgeCards (int playerNum);


public:
	TextDisplay (shared_ptr <Player> p1, shared_ptr <Player> p2);
	
	virtual ~TextDisplay ();

    // Prints row of cards on game board
    void printRow(vector<vector<string>> row);

	// Displays player's hand
	void displayHand(int playerNum) override;

	// Displays minion i in player's hand
	void displayMinionInfo(int playerNum, int minionNum) override;

	// Displays game board
	void displayBoard() override;

	// Updates board
	void notifyDisplay(int playerNum) override;
};

#endif
