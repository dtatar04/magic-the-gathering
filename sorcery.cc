#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include "board.h"

using namespace std;

int main(int argc, char * argv[]) {
	bool graphicsMode = false;			// Graphics mode: displays graphics 
	bool testingMode = false;			// Testing mode: enables discard and draw commands
	bool initMode = false;				// Initialization mode: reads input commands from provided file
	string deck0File = "default.deck";	// Default deck if deckFile is not provided
	string deck1File = "default.deck";	// Default deck if deckFile is not provided
	string initFile;					// Initialization file for initialization mode
	bool first = true;					// Determines if it's the first command of the game 

	// COMMAND-LINE ARGUMENTS
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		string fileName;

		// Specifies Player 1's deck file
		if (arg == "-deck1") {
			fileName = argv [i + 1];
			deck0File = fileName;
			i++;

		// Specifies Player 2's deck file
		} else if (arg == "-deck2") {
			fileName = argv [i + 1];
			deck1File = fileName;
			i++;

		// Specifies initialization file
		} else if (arg == "-init") {
			initMode = true;
			fileName = argv [i + 1];
			initFile = fileName;
			cout << "Init File: " << initFile << endl;
			i++;

		// Turns on -testing mode
		} else if (arg == "-testing") {
			testingMode = true;
			cout << "Testing On" << endl;

		// Turns on -graphics mode
		} else if (arg == "-graphics") {
			graphicsMode = true;
			cout << "Graphics On" << endl;
		}
	}

	// INITIALIZING PLAYER NAMES	
	string p0, p1;
	ifstream ifs;
	// Initialization via file
	if (initMode) {
		ifs = ifstream {initFile};		
		ifs >> p0;
		ifs >> p1;

		// Check if EOF
		if (ifs.peek() == EOF) {		
			initMode = false;
		}
	
	// Inititalization via user input
	} else {
		cout << "Enter Player 1's name: " << endl;
		cin >> p0;
		cout << "Enter Player 2's name: " << endl;
		cin >> p1; 
	}

	// Initializes game with players
	Board b {p0, p1, deck0File, deck1File, graphicsMode, testingMode};

	// Begin Player 1's turn
	cout << "Player 1's turn." << endl;
	b.startTurn();

	// PLAYER COMMANDS
	while (true) {
		string line;
		string cmd;
		int i, j, p;
		char t;

		// Commands via file
		if (initMode) {
			getline (ifs, line);

			// Check if EOF
			if (ifs.peek() == EOF) {
				initMode = false;
			}
		
		// Commands via user input
		} else {
			if (!first) {
				cout << "Enter a command: " << endl;
			}
			getline (cin, line);
		}

		istringstream iss {line};
		iss >> cmd;

		// help - displays a message describing the commands and their formats
		if (cmd == "help") {
			cout << "Commands:" << endl;
			cout << "\thelp -- Display this message." << endl;
			cout << "\tend -- End the current player's turn." << endl;
			cout << "\tquit -- End the game." << endl;
			cout << "\tdraw -- Draws a card from the deck and adds it to hand if deck is not empty and hand is not full. Only available in -testing mode" << endl;
			cout << "\tdiscard [card] -- Discards card from hand. Only available in -testing mode" << endl;
			cout << "\tattack [minion] -- Orders minion to attack the opponent." << endl;
			cout << "\tattack [minion] [other-minion] -- Orders minion to attack other-minion." << endl;
			cout << "\tplay [card] -- Play card on no target." << endl;
			cout << "\tplay [card] [target-player] [target-card] -- Play card, targeting target-card owned by target-player." << endl;
			cout << "\tuse [minion] -- Use minion's ability on no target." << endl;
			cout << "\tuse [minion] [target-player] [target-card] -- Use minion's special ability, targeting target-card owned by target-player." << endl;
			cout << "\tinspect [minion] -- View a minion's card and all enchantments on that minion." << endl;
			cout << "\thand -- Describe all cards in your hand." << endl;
			cout << "\tboard -- Describe all cards on the board." <<endl;

		// end - ends the current player’s turn
		} else if (cmd == "end") {
			b.endTurn();
			b.switchAP();
			if (!b.getAP()) {
				cout << "Player 1's turn." << endl;
			} else {
				cout << "Player 2's turn." << endl;
			}
			b.startTurn();
			b.notifyDisplay();
		
		// quit - ends the game immediately with no winner
		} else if (cmd == "quit") {
			return 0;
		
		// draw - draws a card, similar to the effect if the player just started their 
		//		  turn. This command is only available in -testing mode
		} else if (cmd == "draw") {
			if (testingMode) {
				if (b.getSizeHand() < 5 && b.getSizeDeck()) {
					b.draw();
				} else if (b.getSizeHand() >= 5) {
					cout << "Hand is full. Cannot draw." << endl;
				} else if (b.getSizeDeck() >= 5)	{
					cout << "Deck is empty. Cannot draw." << endl;
				}
				b.notifyDisplay();
			} else {
				cout << "Command 'draw' can only be used in -testing mode." << endl;
			}

		// discard [i] - discards the ith card in the player’s hand, simply removing 
		//				 it from their hand (the card does not go to the graveyard, 
		//				 trigger leave play effects or anything else). This command 
		//				 is only available in -testing mode
		} else if (cmd == "discard") {
			iss >> i;
			if (testingMode) {
				if (i >= 1 && i <= b.getSizeHand()) {	
					b.discard(i - 1);
				} else {
					cout << "[discard i] Invalid index. Re-enter command." << endl;  
				}
				b.notifyDisplay();
			} else {
				cout << "Command 'discard' can only be used in -testing mode." << endl;
			}
		
		// attack [i] - orders minion i to attack the opposing player, where 1 is the 
		//				leftmost minion and 5 is the rightmost minion
		// attack [i] [j] - orders the active player’s minion i to attack the inactive 
		//					player’s minion j
		} else if (cmd == "attack") {
			iss >> i;

			// attack [i] [j]
			if (iss >> j) {
				if (i >= 1 && i <= b.getSizeMinionsPlayed() && j >= 1 && j <= b.getOppSizeMinionsPlayed()) {	
					b.attack(i - 1, j - 1);	
				} else {
					cout << "[attack i j] Invalid index. Re-enter command." << endl;  
				}

			// attack [i]
			} else {
				if (i >= 1 && i <= b.getSizeMinionsPlayed()) {	
					b.attack(i - 1);	
				} else {
					cout << "[attack i] Invalid index. Re-enter command." << endl;  
				}
			}
			b.notifyDisplay();
		
		// play [i] - plays the ith card in the active player’s hand with no target
		// play [i] [p] [t] - plays the ith card in the active player’s hand on card 
		//					  t owned by player p. p may be equal to 1 or 2 to represent 
		//					  player 1 or 2 respectively. t is either 1, 2, 3, 4, 5 (the 
		//					  ith minion owned by player p) or r (the ritual owned by 
		//					  player p). This can be used to play enchantments and spells 
		//					  with targets.
		} else if (cmd == "play") {
			iss >> i;

			// play [i] [p] [t]
			if (iss >> p >> t) {
				if (i >= 1 && i <= b.getSizeHand() 
					&& (p == 1 || p == 2) 
					&& ((t >= '1' && t <= '5') || t == 'r')) {	
					b.play(i - 1, p - 1, t);
				} else {
					cout << "[play i p t] Invalid index. Re-enter command." << endl;  
				}
			
			// play [i]
			} else {
				if (i >= 1 && i <= b.getSizeHand()) {	
					b.play(i - 1);	
				} else {
					cout << "[play i] Invalid index. Re-enter command." << endl;  
				}
			}
			b.notifyDisplay();
		
		// use [i] - uses the ith minion in the active player’s hand with no target
		// use [i] [p] [t] - plays the ith minion in the active player’s hand on target 
		//					 t owned by player p. The command orders minion i to use its 
		//					 activated ability on the provided target (or on no target)
		} else if (cmd == "use") {
			int tInt;
			bool playSelf = false;
			iss >> i;

			// use [i] [p] [t]
			if (iss >> p >> tInt) {

				// Check if player is using minion on themself
				if ((p == 1 && !b.getAP()) || (p == 2 && b.getAP())) {
					playSelf = true;
				}

				if ((i >= 1 && i <= b.getSizeHand()) && 
					(p == 1 || p == 2) &&
					((playSelf && tInt >= 1 && tInt <= b.getSizeMinionsPlayed()) ||
					(!playSelf && tInt >= 1 && tInt <= b.getOppSizeMinionsPlayed()))) {
					b.use(i - 1, playSelf, tInt - 1);
				} else {
					cout << "[use i p t] Invalid index. Re-enter command." << endl;  
				}
			
			// use [i]
			} else {
				if (i >= 1 && i <= b.getSizeHand()) {	
					b.use(i - 1);
				} else {
					cout << "[use i] Invalid index. Re-enter command." << endl;  
				}
			}
			b.notifyDisplay();
		
		// inspect [i] - inspects the ith minion owned by the active player
		} else if (cmd == "inspect") {
			iss >> i;
			if (i >= 1 && i <= b.getSizeMinionsPlayed()) {	
				b.displayMinionInfo (i - 1);
			} else {
				cout << "[inspect i] Invalid index. Re-enter command." << endl;  
			}

		// hand - displays the active player’s hand
		} else if (cmd == "hand") {
			b.displayHand();

		// board - displays the board, 
		} else if (cmd == "board") {
			b.displayBoard();

		// Invalid command
		} else {
			if (!first) {
				cout << "Unrecognized command: '" << cmd << "'. Enter 'help' for a list of commands." << endl;
			} else {
				first = false;
			}
		}

		if (b.isWon()) {
			cout << b.getWinner() << " has won!!" << endl;
			return 0;
		}
	} 
}
