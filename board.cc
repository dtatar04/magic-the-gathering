#include <string>
#include <memory>
#include "board.h"
#include "textDisplay.h"
#include "graphicsDisplay.h"

class TextDisplay;
class GraphicsDisplay;

using namespace std;

Board::Board(string p0Name, string p1Name, string p0deckFile, string p1deckFile, bool graphicsMode, bool testing):
	p0 {make_shared <Player> (1, p0Name, p0deckFile, 3, testing)}, p1 {make_shared <Player> (2,p1Name, p1deckFile, 3, testing)}, activePlayer {0} {
	
	// Initializes text interface
	unique_ptr <TextDisplay> td = make_unique <TextDisplay> (p0,p1);
	addDisplay(move(td));

	// Initializes graphics interface
	if (graphicsMode) {
		unique_ptr <GraphicsDisplay> gd = make_unique <GraphicsDisplay> (p0,p1);
		addDisplay(move(gd));
	}
}

void Board::addDisplay (unique_ptr <Displays> d) {
	displays.emplace_back(move(d));
}

void Board::notifyDisplay() {
	for (int i = 0; i < static_cast <int> (displays.size()); i++) {
		if (!activePlayer) {
			displays[i]->notifyDisplay(1);
		} else {
			displays[i]->notifyDisplay(2);
		}
	}
}

int Board::getNumDisplays() {
	return static_cast <int> (displays.size());
}

bool Board::isWon() {
	if (p0->getLife() <= 0) {
		winner = p1->getName();
		return true;
	} else if (p1->getLife() <= 0) {
		winner = p0->getName();
		return true;
	} else {
		return false;
	}
}

string Board::getWinner() {
	return winner;
}

bool Board::getAP() {
	return activePlayer;
}
		
void Board::switchAP() {
	activePlayer = !activePlayer;
}

int Board::getSizeDeck() {
	if (!activePlayer) {
		return p0->getSizeDeck();
	} else {
		return p1->getSizeDeck();
	}
}

int Board::getSizeHand() {
	if (!activePlayer) {
		return p0->getSizeHand();
	} else {
		return p1->getSizeHand();
	}
}

int Board::getSizeGraveyard() {
	if (!activePlayer) {
		return p0->getSizeGraveyard();
	} else {
		return p1->getSizeGraveyard();
	}
}

int Board::getSizeMinionsPlayed() {
	if (!activePlayer) {
		return p0->getSizeMinionsPlayed();
	} else {
		return p1->getSizeMinionsPlayed();
	}
}

int Board::getOppSizeDeck() {
	if (activePlayer) {
		return p0->getSizeDeck();
	} else {
		return p1->getSizeDeck();
	}
}

int Board::getOppSizeHand() {
	if (activePlayer) {
		return p0->getSizeHand();
	} else {
		return p1->getSizeHand();
	}
}

int Board::getOppSizeGraveyard() {
	if (activePlayer) {
		return p0->getSizeGraveyard();
	} else {
		return p1->getSizeGraveyard();
	}
}

int Board::getOppSizeMinionsPlayed() {
	if (activePlayer) {
		return p0->getSizeMinionsPlayed();
	} else {
		return p1->getSizeMinionsPlayed();
	}	
}

void Board::startTurn() {
	draw();
	
	for (int i = 0; i < getNumDisplays(); i++) {
		displays[i]->notifyActivePlayer(activePlayer);
	}

	if (activePlayer) {
		p1->modMagic(1);

		// Applies start of turn Ritual if applicable
		if (p1->getSizeRitual() == 1) {
			if (p1->getRitual()->getName() == "Dark Ritual") {
				p1->getRitual()->use(*p1, *p0, 0);
			}
		}
		p1->restoreActionsToMinions();
	} else {
		p0->modMagic(1);

		// Applies start of turn Ritual if applicable
		if (p0->getSizeRitual() == 1) {
			if (p0->getRitual()->getName() == "Dark Ritual") {
				p0->getRitual()->use(*p0, *p1, 0);
			}
		}
		p0->restoreActionsToMinions();
	}
}

void Board::endTurn() {
	if (activePlayer) {
		p1->notifyMinions(nullptr, State::EndOfTurn);
	} else {
		p0->notifyMinions(nullptr, State::EndOfTurn);
	}
	
	for (int i = 0; i < static_cast <int> (displays.size()); i++) {
		displays[i]->notifyActivePlayer(activePlayer);
	}

}

void Board::draw() {
	if (activePlayer) {
		p1->draw();
	} else {
		p0->draw();
	}
}

void Board::discard(int i) {
	if (activePlayer) {
		p1->discard(i);
	} else {
		p0->discard(i);
	}
}

void Board::attack(int i) {
	if (!activePlayer) {
		p0->minionAttacksPlayer(i, *p1);
	} else {
		p1->minionAttacksPlayer(i, *p0);
	}
}

void Board::attack(int i, int j) {
	if (!activePlayer) {
		p0->minionAttacksMinion(i, j, *p1);
	} else {
		p1->minionAttacksMinion(i, j, *p0);
	}
}

void Board::play(int i) {	
	if (!activePlayer) {
		p0->playCardNoTarget(*p0, *p1, i);
	} else {
		p1->playCardNoTarget(*p1, *p0, i);
	}
}

void Board::play(int i, int p, char t) {
	bool playSelf;
	if ((!activePlayer && p == 0) || 
		(activePlayer && p == 1)) {
		playSelf = true;
	} else {
		playSelf = false;
	}
	
	if (!activePlayer) {
		if (t == 'r') {
			if (playSelf) {
				p0->playRitual(*p0, *(p0->getRitual()), i);
			} else {
				p0->playRitual(*p1, *(p1->getRitual()), i);
			}
		} else {
			int index = t - '0' - 1;
			if (playSelf) {
				p0->playCardWithTarget(*p0, i, index);
			} else {
				p0->playCardWithTarget(*p1, i, index);

			}
		}
	} else {
		if (t == 'r') {
			if (playSelf) {
				p1->playRitual(*p1, *(p1->getRitual()), i);
			} else {
				p1->playRitual(*p0, *(p0->getRitual()), i);
			}
		} else {
			int index = t - '0' - 1;
			if (playSelf) {
				p1->playCardWithTarget(*p1, i, index);
			} else {
				p1->playCardWithTarget(*p0, i, index);
			}
		}
	}
}

void Board::use(int i) {	
	if (!activePlayer) {
		p0->useCardNoTarget(*p0, *p1, i);
	} else {
		p1->useCardNoTarget(*p1, *p0, i);
	}
}

void Board::use(int i, bool playSelf, int t) {	
	if (!activePlayer) {
		if (playSelf) {
			p0->useCardWithTarget(*p0, *p0, i, t);
		} else {
			p0->useCardWithTarget(*p0, *p1, i, t);
		}
	} else {
		if (playSelf) {
			p1->useCardWithTarget(*p1, *p1, i, t);
		} else {
			p1->useCardWithTarget(*p1, *p0, i, t);
		}
	}
}

void Board::displayMinionInfo (int i) {
	for (int j = 0; j < getNumDisplays(); j++) {
		if (!activePlayer) {
			displays[j]->displayMinionInfo(1,i);
		} else {
			displays[j]->displayMinionInfo(2,i);
		}
	}
}

void Board::displayHand() {
	for (int i = 0; i < getNumDisplays(); i++) {
		if (!activePlayer) {
			displays[i]->displayHand(1);
		} else {
			displays[i]->displayHand(2);
		}
	}
}

void Board::displayBoard() {
	for (int i = 0; i < getNumDisplays(); i++) {
		displays[i]->displayBoard();
	}
}
