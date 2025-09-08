#ifndef DISPLAYS
#define DISPLAYS
#include <memory>
#include "player.h"

/*	
	Displays: Abstract Display (observer) class
	
	Class Relationships: has-a Player
*/

class Displays {
protected:
	shared_ptr <Player> p1;
	shared_ptr <Player> p2;
	int activePlayer = 1;

public:
	// Ctor
	Displays (shared_ptr <Player> p1, shared_ptr <Player> p2);

	// Dtor
	virtual ~Displays() = default;
	
	// Displays player's hand
	virtual void displayHand(int playerNum) = 0;

	// Displays minion i in player's hand
	virtual void displayMinionInfo(int playerNum, int minionNum) = 0;

	// Displays game board
	virtual void displayBoard() = 0;

	// Updates board
	virtual void notifyDisplay(int playerNum) = 0;

	// Updates active player
	void notifyActivePlayer(bool ap);
};

#endif
