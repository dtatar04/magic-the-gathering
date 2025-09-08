#ifndef BOARD
#define BOARD

#include <string>
#include <vector>
#include "player.h"
#include "displays.h"
using namespace std;

/*	
	Board: Manages game data (ex. Players, active player, displays) and actions
		between players
	
	Class Relationships: owns-a Player, has-a Displays 
*/

const int MAX_HAND = 5;

class Board {
	shared_ptr <Player> p0;		// Player 1
	shared_ptr <Player> p1;		// Player 2
	bool activePlayer;			// Current active player (false = Player 1, true = Player 2)
	string winner = "tie";		// Game winner
	vector <unique_ptr <Displays>> displays;	// Display interfaces

public:
	// Dtor
	virtual ~Board() = default;

	// Ctor
	Board(string p0Name, string p1Name, string p0deckFile, string p1deckFile, bool graphicsMode, bool testing);

	// Adds display observer
	void addDisplay (unique_ptr <Displays> d);

	// Notifies displays when board changes state
	void notifyDisplay ();

	// Returns number of display observers
	int getNumDisplays ();

	// Returns whether game has been won
	bool isWon();

	// Returns winner
	string getWinner();
	
	// Returns activePlayer
	bool getAP();

	// Switches activePlayer
	void switchAP();

	// Returns number of cards in active player's deck
	int getSizeDeck();

	// Returns number of cards in active player's hand
	int getSizeHand();

	// Returns number of minions in active player's graveyard
	int getSizeGraveyard();

	// Returns number of minions on active player's board
	int getSizeMinionsPlayed();

	// Returns number of cards in non-active player's deck
	int getOppSizeDeck();

	// Returns number of cards in non-active player's hand
	int getOppSizeHand();

	// Returns number of minions in non-active player's graveyard
	int getOppSizeGraveyard();

	// Returns number of minions on non-active player's board
	int getOppSizeMinionsPlayed();

    // Start of turn process
    void startTurn();

    // End of turn process
    void endTurn();

	// Draws a card from players deck to hand
	void draw();

	// Discards the ith card in the player’s hand, simply removing it from 
	//		their hand (the card does not go to the graveyard, trigger leave 
	// 		play effects or anything else)
	void discard(int i);

	// Orders minion i to attack the opposing player, where 1 
	//		is the leftmost minion and 5 is the rightmost minion
	void attack(int i);					

	// Orders the active player’s minion i to attack the inactive 
	//		player’s minion j
	void attack(int i, int j);

	// Plays the ith card in the active player’s hand with no target
	void play(int i);

	// Plays the ith card in the active player’s hand on card t owned by 
	//		player p. p may be equal to 1 or 2 to represent player 1 or 2 
	//		respectively. t is either 1, 2, 3, 4, 5 (the ith minion owned 
	//		by player p) or r (the ritual owned by player p). This can be 
	//		used to play enchantments and spells with targets
	void play(int i, int p, char t);

	// Orders the ith minion on player's board to use its activated ability
	//		with no target
	void use(int i);

	// Orders the ith minion on player's board to use its activated ability
	//		on the provided target
	void use(int i, bool playSelf, int t);

	// Displays minion i from active player's board 
	void displayMinionInfo(int i);

	// Displays active player's hand 
	void displayHand();

	// Displays board
	void displayBoard();
};

#endif
