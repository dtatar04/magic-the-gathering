#ifndef GRAPHICS_DISPLAY
#define GRAPHICS_DISPLAY

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
#include "window.h"

using namespace std;

/*	
	GraphicsDisplay: Displays graphics interface
	
	Class Relationships: is-a Displays, owns-a Xwindow
*/

class GraphicsDisplay: public Displays {
	const int CARD_WIDTH = 90;
	const int CARD_HEIGHT = 75;
	const int MAX_CARD_ROW = 5;
	const int MAX_STRING_LENGTH = 14;
	const int LINE_HEIGHT = 11;
	Xwindow gameBoard;

	// Resizes description
	vector<string> descToPrint (string desc);

	// Displays an empty card at (x,y)
	void drawEmptyCard(int x, int y, int c);

	// Displays a empty slot at (x,y)
	void drawEmptySlot(int x, int y);

	// Displays player card
	void drawPlayer(int playerNum);

	// Displays card
	void drawCard (int x, int y, shared_ptr <Card> &c);

	// Displays spell
	void drawSpell (int x, int y, shared_ptr <Spell> &s);

	// Displays minion
	void drawMinion (int x, int y, shared_ptr <Minion> &m);
	
	// Displays enchantment
	void drawEnchantment (int x, int y, shared_ptr <Enchantment> &e);

	// Displays ritual
	void drawRitual (int x, int y, shared_ptr <Ritual> &r);

	// Displays player's minions that have been played
	void drawMinionsPlayed (int player);
	
	// Displays graveyard, player, and ritual cards
	void drawEdgeCards (int playerNum);

	// Display's hand of active player
	void drawActiveHand (int playerNum);

public:
	GraphicsDisplay (shared_ptr <Player> p1, shared_ptr <Player> p2);
	
	virtual ~GraphicsDisplay ();

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
