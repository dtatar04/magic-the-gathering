#include "graphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay (shared_ptr <Player> p1, shared_ptr <Player> p2): Displays {p1, p2} {
	displayBoard();
}

GraphicsDisplay::~GraphicsDisplay() {}

vector<string> GraphicsDisplay::descToPrint (string desc) {
	vector<string> newDesc;
	string line;

	// Determines number of rows
	int descLength = desc.length();
	int numRows = descLength / MAX_STRING_LENGTH;
	int remainder = descLength % MAX_STRING_LENGTH;
	int index = 0;

	// Create each full row
	for (int row = 0; row < numRows; row++) {
		// Create each row
		for (int i = 0; i < MAX_STRING_LENGTH; i++) {
			line += desc[index];
			index++;
		}

		// Add row to vector
		newDesc.emplace_back(line);

		// Clear row
		line = "";
	}

	// Creates remaining desc
    for (int i = 0; i < remainder; i++) {
		line += desc[index];
		index++;
    }

	// Add remaining desc to vector
	newDesc.emplace_back(line);

	return newDesc;
}

void GraphicsDisplay::drawEmptyCard(int x, int y, int c) {
	gameBoard.fillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT, 0);
	gameBoard.fillRectangle(x + 2, y + 2, CARD_WIDTH - 4, CARD_HEIGHT - 4, c);
}

void GraphicsDisplay::drawEmptySlot(int x, int y) {
	gameBoard.fillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT, 1);
}

// void GraphicsDisplay::drawTitle() {};

void GraphicsDisplay::drawPlayer(int playerNum) {
	if (playerNum == 1) {
		gameBoard.fillRectangle(205, 5, CARD_WIDTH, CARD_HEIGHT, 4);
		gameBoard.drawString (210, 15, "PLAYER 1");
		gameBoard.drawString (210, 15 + LINE_HEIGHT, p1->getName());
		gameBoard.drawString (210, 15 + LINE_HEIGHT*2, "LIFE: " + to_string(p1->getLife()));
		gameBoard.drawString (210, 15 + LINE_HEIGHT*3, "MAGIC: " + to_string(p1->getMagic()));
	} else {
		gameBoard.fillRectangle(205, 320, CARD_WIDTH, CARD_HEIGHT, 3);
		gameBoard.drawString (210, 330, "PLAYER 2");
		gameBoard.drawString (210, 330 + LINE_HEIGHT, p2->getName());
		gameBoard.drawString (210, 330 + LINE_HEIGHT*2, "LIFE: " + to_string(p2->getLife()));
		gameBoard.drawString (210, 330 + LINE_HEIGHT*3, "MAGIC: " + to_string(p2->getMagic()));
	}
}

void GraphicsDisplay::drawCard(int x, int y, shared_ptr <Card> &c) {
	// Displays Spell
	if (c->getType() == "Spell") {
		shared_ptr <Spell> s = dynamic_pointer_cast <Spell> (c);
		drawSpell(x, y, s);

	// Displays Minion
	} else if (c->getType() == "Minion") {
		shared_ptr <Minion> m = dynamic_pointer_cast <Minion> (c);
		drawMinion (x, y, m);
		
	// Displays Enchantment
	} else if (c->getType() == "Enchantment") {
		shared_ptr <Enchantment> e = dynamic_pointer_cast <Enchantment> (c);
		drawEnchantment (x, y, e);
		
	// Displays Ritual
	} else {
		shared_ptr <Ritual> r = dynamic_pointer_cast <Ritual> (c);
		drawRitual (x, y, r);
	}
}

void GraphicsDisplay::drawSpell(int x, int y, shared_ptr <Spell> &s) {
	int textX = x + 5;
	int textY = y + 10;

	// Display card 
	gameBoard.fillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT, 0);

	// Display text
	gameBoard.drawString (textX, textY, "SPELL");
	gameBoard.drawString (textX, textY + LINE_HEIGHT, s->getName());
	gameBoard.drawString (textX, textY + LINE_HEIGHT*2, "COST: " + to_string(s->getCost()));
	int descY = textY + LINE_HEIGHT*3;
	vector<string> desc = descToPrint (s->getDescription());
	for (int i = 0; i < static_cast <int> (desc.size()); i++) {
		gameBoard.drawString (textX, descY + LINE_HEIGHT*i, desc[i]);
	}
}

void GraphicsDisplay::drawMinion(int x, int y, shared_ptr <Minion> &m) {
	int textX = x + 5;
	int textY = y + 10;

	// Display card 
	gameBoard.fillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT, 0);

	// Display text
	int descY;
	gameBoard.drawString (textX, textY, "MINION");
	gameBoard.drawString (textX, textY + LINE_HEIGHT, m->getName());
	gameBoard.drawString (textX, textY + LINE_HEIGHT*2, "COST: " + to_string(m->getCost()));
	gameBoard.drawString (textX, textY + LINE_HEIGHT*3, "A/D: " + to_string(m->getAttack()) + "/" + to_string(m->getDefense()));
	if (m->getBAA()) {
		gameBoard.drawString (textX, textY + LINE_HEIGHT*4, "A_COST: " + to_string(m->getActivatedAbility()->getActCost()));
		descY = textY + LINE_HEIGHT*5;
	} else if (m->getBTA()) {
		descY = textY + LINE_HEIGHT*4;
	}
	vector<string> desc = descToPrint (m->getDescription());
	for (int i = 0; i < static_cast <int> (desc.size()); i++) {
		gameBoard.drawString (textX, descY + LINE_HEIGHT*i, desc[i]);
	}
}

void GraphicsDisplay::drawEnchantment(int x, int y, shared_ptr <Enchantment> &e) {
	int textX = x + 5;
	int textY = y + 10;

	// Display card 
	gameBoard.fillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT, 0);

	// Display text
	int descY;
	gameBoard.drawString (textX, textY, "ENCHANTMENT");
	gameBoard.drawString (textX, textY + LINE_HEIGHT, e->getName());
	gameBoard.drawString (textX, textY + LINE_HEIGHT*2, "COST: " + to_string(e->getCost()));
	if (e->getDescription() == "") {
		gameBoard.drawString (textX, textY + LINE_HEIGHT*3, "MODs: " + e->getAtkOp() + to_string(e->getModValue()) + "/" + e->getDefOp() + to_string(e->getModValue()));
	} else {
		descY = textY + LINE_HEIGHT*3;
		vector<string> desc = descToPrint (e->getDescription());
		for (int i = 0; i < static_cast <int> (desc.size()); i++) {
			gameBoard.drawString (textX, descY + LINE_HEIGHT*i, desc[i]);
		}
	}
}

void GraphicsDisplay::drawRitual(int x, int y, shared_ptr <Ritual> &r) {
	int textX = x + 5;
	int textY = y + 10;

	// Display card 
	gameBoard.fillRectangle(x, y, CARD_WIDTH, CARD_HEIGHT, 0);

	// Display text
	int descY;
	gameBoard.drawString (textX, textY, "RITUAL");
	gameBoard.drawString (textX, textY + LINE_HEIGHT, r->getName());
	gameBoard.drawString (textX, textY + LINE_HEIGHT*2, "COST: " + to_string((r->getCost())));
	gameBoard.drawString (textX, textY + LINE_HEIGHT*3, "A_COST: " + to_string(r->getActCost()));
	gameBoard.drawString (textX, textY + LINE_HEIGHT*4, "CHARGES: " + to_string(r->getCharges()));
	descY = textY + LINE_HEIGHT*5;
	vector<string> desc = descToPrint (r->getDescription());
	for (int i = 0; i < static_cast <int> (desc.size()); i++) {
		gameBoard.drawString (textX, descY + LINE_HEIGHT*i, desc[i]);
	}
}

void GraphicsDisplay::drawMinionsPlayed (int playerNum) {
	int numEmptySlots;
	int xPos = 5;

	if (playerNum == 1) {
		// Display minions
		for (int i = 0; i < p1->getSizeMinionsPlayed(); i++) {
			shared_ptr <Minion> m = p1->getMinionPlayed(i);
			drawMinion(xPos, 85, m);
			xPos += 100;
		}

		numEmptySlots = MAX_CARD_ROW - p1->getSizeMinionsPlayed();

		// Display empty slots
		for (int i = 0; i < numEmptySlots; i++) {
			drawEmptyCard (xPos, 85, 1);
			xPos += 100;
		}
	} else {
		// Display minions
		for (int i = 0; i < p2->getSizeMinionsPlayed(); i++) {
			shared_ptr <Minion> m = p2->getMinionPlayed(i);
			drawMinion(xPos, 240, m);
			xPos += 100;
		}

		numEmptySlots = MAX_CARD_ROW - p2->getSizeMinionsPlayed();

		// Display empty slots
		for (int i = 0; i < numEmptySlots; i++) {
			drawEmptyCard (xPos, 240, 1);
			xPos += 100;
		}
	}
}

void GraphicsDisplay::drawEdgeCards (int playerNum) {
    // Player 1
    if (playerNum == 1) {
		// Player 1's ritual on board
		if (p1->getSizeRitual() == 0) {
			drawEmptyCard(5, 5, 1);
		} else  {
			shared_ptr <Card> c = p1->getRitual();
			drawCard(5, 5, c);
		}

		// Empty slot
		drawEmptySlot(105, 5);

		// Player card
		drawPlayer(1);

		// Empty slot
		drawEmptySlot(305, 5);

		// Player 1's graveyard
		if (p1->getSizeGraveyard() == 0) {
			drawEmptyCard(405, 5, 1);
		} else {
			shared_ptr <Card> c = p1->getMinionGraveyard(0);
			drawCard(405, 5, c);
		}

    // Player 2
    } else {
		// Player 2's ritual on board
		if (p2->getSizeRitual() == 0) {
			drawEmptyCard(5, 320, 1);
		} else  {
			shared_ptr <Card> c = p2->getRitual();
			drawCard(5, 320, c);
		}

		// Empty slot
		drawEmptySlot(105, 320);

		// Player card
		drawPlayer(2);

		// Empty slot
		drawEmptySlot(305, 320);

		// Player 1's graveyard
		if (p1->getSizeGraveyard() == 0) {
			drawEmptyCard(405, 320, 1);
		} else {
			shared_ptr <Card> c = p2->getMinionGraveyard(0);
			drawCard(405, 320, c);
		}
    }
}

void GraphicsDisplay::drawActiveHand (int playerNum) {
	int numEmptySlots;
	int xPos = 5;
	int colour;


	if (playerNum == 1) {
		colour = 4;
		gameBoard.fillRectangle(0, 400, 500, 100, 4);
		gameBoard.drawString (15, 415, "Player 1's Hand: ");
		// Display cards
		for (int i = 0; i < p1->getSizeHand(); i++) {
			shared_ptr <Card> c = p1->getCardHand(i);
			drawCard(xPos, 420, c);
			xPos += 100;
		}

		numEmptySlots = MAX_CARD_ROW - p1->getSizeHand();
	} else {
		colour = 3;
		gameBoard.fillRectangle(0, 400, 500, 100, 3);
		gameBoard.drawString (15, 415, "Player 2's Hand: ");
		// Display cards
		for (int i = 0; i < p2->getSizeHand(); i++) {
			shared_ptr <Card> c = p2->getCardHand(i);
			drawCard(xPos, 420, c);
			xPos += 100;
		}

		numEmptySlots = MAX_CARD_ROW - p2->getSizeHand();
	}

	// Display empty slots
	for (int i = 0; i < numEmptySlots; i++) {
		drawEmptyCard (xPos, 420, colour);
		xPos += 100;
	}
}

void GraphicsDisplay::displayHand (int playerNum) {
	// Covers game board
	gameBoard.fillRectangle(0, 0, 500, 500, 0);

	int numCards;
	int xPos = 5;

	// Player 1's hand
	if (playerNum == 1) {
		numCards = p1->getSizeHand();

		// Empty hand
		if (numCards == 0) {
			for (int i = 0; i < MAX_CARD_ROW; i++) {
				drawEmptyCard(xPos, 212, 0);
				xPos += 100;
			}
		} else {
			// Player 1's hand
			for (int i = 0; i < numCards; i++) {
				shared_ptr <Card> c = p1->getCardHand(i);
				drawCard (xPos, 212, c);
				xPos += 100;
			}
			
			// Empty slots in Player 1's hand
			for (int i = 0; i < (MAX_CARD_ROW - numCards); i++) {
				drawEmptyCard(xPos, 212, 0);
				xPos += 100;
			}
		}
	
	// Player 2's hand
	} else {
		numCards = p2->getSizeHand();

		// Empty hand
		if (numCards == 0) {
			for (int i = 0; i < MAX_CARD_ROW; i++) {
				drawEmptyCard(xPos, 212, 0);
				xPos += 100;
			}
		} else {
			// Player 2's hand
			for (int i = 0; i < numCards; i++) {
				shared_ptr <Card> c = p2->getCardHand(i);
				drawCard (xPos, 212, c);
				xPos += 100;
			}
			
			// Empty slots in Player 2's hand
			for (int i = 0; i < (MAX_CARD_ROW - numCards); i++) {
				drawEmptyCard(xPos, 212, 0);
				xPos += 100;
			}
		}
	}
}

void GraphicsDisplay::displayMinionInfo(int playerNum, int minionNum) {
	// Covers game board
	gameBoard.fillRectangle(0, 0, 500, 500, 0);

	shared_ptr <Minion> m;
	int xPos = 5;
	int yPos = 5;
	
	if (playerNum == 1) {
		m = p1->getMinionPlayed(minionNum);
	} else {
		m = p2->getMinionPlayed(minionNum);
	}

    // Print minion i
	drawMinion(xPos, yPos, m);
	yPos += 80;
    
	// Number of enchantments
	int eCount = m->getEnchantmentCount();

    // Determine number of rows
    int numRows = eCount / MAX_CARD_ROW;
    int remainder = eCount % MAX_CARD_ROW;

    int index = 0;

    // Display each full row of enchantments
	for (int row = 0; row < numRows; row++) {
        // Add to vector to be printed
        for (int i = 0; i < MAX_CARD_ROW; i++) {
			drawEnchantment(xPos, yPos, m->getMinionEnchantments()[index]);
			xPos += 100;
            index++;
        }

		// Set new coordinates 
		xPos = 5;
		yPos += 80;
    }

    // Display remaining enchantments to be printed
    for (int i = 0; i < remainder; i++) {
		drawEnchantment(xPos, yPos, m->getMinionEnchantments()[index]);
		xPos += 100;
        index++;
    }
}

void GraphicsDisplay::displayBoard() {
	// Covers game board
	gameBoard.fillRectangle(0, 0, 500, 400, 1);

	// Displays cards
	drawEdgeCards(1);
	drawMinionsPlayed(1);
	//drawTitle();
	drawMinionsPlayed(2);
	drawEdgeCards(2);

	// Display active player's hand
	drawActiveHand(activePlayer);
}

void GraphicsDisplay::notifyDisplay(int playerNum) {
	// Displays game board 
	displayBoard();
	
	/*// Displays active player's hand
	if (playerNum == 1) {
		gameBoard.fillRectangle(0, 400, 500, 100, 5);
		gameBoard.drawString (10, 410, "Player 1's Hand: ");
		drawActiveHand(1);
	} else {
		gameBoard.fillRectangle(0, 400, 500, 100, 5);
		gameBoard.drawString (10, 410, "Player 2's Hand: ");
		drawActiveHand(2);
	}*/
}
