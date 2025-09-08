#include "textDisplay.h"
#include "ascii_graphics.h"

TextDisplay::TextDisplay (shared_ptr <Player> p1, shared_ptr <Player> p2): Displays {p1, p2} {}

TextDisplay::~TextDisplay() {}

string TextDisplay::printBorder (bool isTop) {
	string border;
	// Left corner
	if (isTop) {
		border = EXTERNAL_BORDER_CHAR_TOP_LEFT;
	} else {
		border = EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;
	}

	// Line
	for (int i = 0; i < CARD_WIDTH * MAX_CARD_ROW; i++) {
		border += EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	}

	// Right Corner
	if (isTop) {
		border += EXTERNAL_BORDER_CHAR_TOP_RIGHT;
	} else {
		border += EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT;
	}

	return border;
}

string TextDisplay::printSide() {
	return EXTERNAL_BORDER_CHAR_UP_DOWN;
} 

vector <string> TextDisplay::displayTitle() {
	return CENTRE_GRAPHIC;
}

vector <string> TextDisplay::displayPlayer (int playerNum) {
	if (playerNum == 1) {
    	return display_player_card(playerNum, p1->getName(), p1->getLife(), p1->getMagic());
	} else {
    	return display_player_card(playerNum, p2->getName(), p2->getLife(), p2->getMagic());
	}
}
	
vector <string> TextDisplay::displayCard (shared_ptr <Card> &c) {
	// Displays Spell
	if (c->getType() == "Spell") {
		shared_ptr <Spell> s = dynamic_pointer_cast <Spell> (c);
		return displaySpell (s);

	// Displays Minion
	} else if (c->getType() == "Minion") {
		shared_ptr <Minion> m = dynamic_pointer_cast <Minion> (c);
		return displayMinion (m);
		
	// Displays Enchantment
	} else if (c->getType() == "Enchantment") {
		shared_ptr <Enchantment> e = dynamic_pointer_cast <Enchantment> (c);
		return displayEnchantment (e);
		
	// Displays Ritual
	} else {
		shared_ptr <Ritual> r = dynamic_pointer_cast <Ritual> (c);
		return displayRitual (r);
	}
}

vector <string> TextDisplay::displaySpell (shared_ptr <Spell> &s) {
	return display_spell(s->getName(), s->getCost(), s->getDescription());
}

vector <string> TextDisplay::displayMinion (shared_ptr <Minion> &m) {
	// General Minion
	if (!m->getBTA() && !m->getBAA()) {
		return display_minion_no_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefense());

	// Triggered Minion
	} else if (m->getBTA() && !m->getBAA()) {
		return display_minion_triggered_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefense(), m->getDescription());
	
	// Activated Minion
	} else {
		return display_minion_activated_ability(m->getName(), m->getCost(), m->getAttack(), m->getDefense(), m->getActivatedAbility()->getActCost(), m->getDescription());
	}
}

vector <string> TextDisplay::displayEnchantment (shared_ptr <Enchantment> &e) {
	// Enchantements with numerical values
	if (e->getDescription() == "") {

		// Set output as string
		string attackField = e->getAtkOp() + to_string(e->getModValue());
		string defenseField = e->getDefOp() + to_string(e->getModValue());

		return display_enchantment_attack_defence(e->getName(), e->getCost(), e->getDescription(), attackField, defenseField);
	
	// Enchantments with descriptions
	} else {
		return display_enchantment(e->getName(), e->getCost(), e->getDescription());
	}
}

vector <string> TextDisplay::displayRitual (shared_ptr <Ritual> &r) {
	return display_ritual(r->getName(), r->getCost(), r->getActCost(), r->getDescription(), r->getCharges());
}

vector <vector<string>> TextDisplay::displayEdgeCards (int playerNum) {
    vector <vector<string>> row;
    vector <string> emptyCard = CARD_TEMPLATE_BORDER;
    vector <string> emptySlot = CARD_TEMPLATE_EMPTY;

    // Player 1
    if (playerNum == 1) {
		// Player 1's ritual on board
		if (p1->getSizeRitual() == 0) {
			row.emplace_back(emptyCard);
		} else  {
			shared_ptr <Card> c = p1->getRitual();
			row.emplace_back(displayCard(c));
		}

		// Empty slot
		row.emplace_back(emptySlot);

		// Player card
        row.emplace_back(displayPlayer(1));

		// Empty slot
		row.emplace_back(emptySlot);

		// Player 1's graveyard
		if (p1->getSizeGraveyard() == 0) {
			row.emplace_back(emptyCard);
		} else {
			shared_ptr <Card> c = p1->getMinionGraveyard(0);
			row.emplace_back(displayCard(c));
		}

    // Player 2
    } else {
		// Player 2's ritual on board
		if (p2->getSizeRitual() == 0) {
			row.emplace_back(emptyCard);
		} else  {
			shared_ptr <Card> c = p2->getRitual();
			row.emplace_back(displayCard(c));
		}

		// Empty slot
		row.emplace_back(emptySlot);

		// Player card
        row.emplace_back(displayPlayer(2));

		// Empty slot
		row.emplace_back(emptySlot);

		// Player 2's graveyard
		if (p2->getSizeGraveyard() == 0) {
			row.emplace_back(emptyCard);
		} else {
			shared_ptr <Card> c = p2->getMinionGraveyard(0);
			row.emplace_back(displayCard(c));
		}
    }
    
    return row;
}
	
vector <vector<string>> TextDisplay::displayMinionsPlayed (int playerNum) {
    vector <vector<std::string>> row;
    vector <string> emptyCard = CARD_TEMPLATE_BORDER;
	int numEmptySlots;
    
	if (playerNum == 1) {
		// Display minions
		for (int i = 0; i < p1->getSizeMinionsPlayed(); i++) {
			shared_ptr <Minion> m = p1->getMinionPlayed(i);
			row.emplace_back(displayMinion(m));
		}

		numEmptySlots = MAX_CARD_ROW - p1->getSizeMinionsPlayed();
	} else {
		// Display minions
		for (int i = 0; i < p2->getSizeMinionsPlayed(); i++) {
			shared_ptr <Minion> m = p2->getMinionPlayed(i);
			row.emplace_back(displayMinion(m));
		}

		numEmptySlots = MAX_CARD_ROW - p2->getSizeMinionsPlayed();
	}

	// Display empty slots
	for (int i = 0; i < numEmptySlots; i++) {
		row.emplace_back(emptyCard);
	}

    return row;
}

void TextDisplay::displayHand (int playerNum) {
    std::vector<std::vector<std::string>> row;
	int numCards;

	// Player 1's hand
	if (playerNum == 1) {
		numCards = p1->getSizeHand();

		if (numCards == 0) {
        	cout << "Hand is empty." << endl;
		} else {
			for (int i = 0; i < numCards; i++) {
				shared_ptr <Card> c = p1->getCardHand(i);
				row.emplace_back(displayCard(c));
			}
		}
	
	// Player 2's hand
	} else {
		numCards = p2->getSizeHand();

		if (numCards == 0) {
        	cout << "Hand is empty." << endl;
		} else {
			for (int i = 0; i < numCards; i++) {
				shared_ptr <Card> c = p2->getCardHand(i);
				row.emplace_back(displayCard(c));
			}
		}
	}

    // Print hand
    for (int i = 0; i < CARD_HEIGHT; i++){
        for (int j = 0; j < numCards; j++){
            cout << row[j][i];
        }
        cout << endl;
    }
}

void TextDisplay::printRow (vector<vector<string>> row) {
    for (int i = 0; i < CARD_HEIGHT; i++){
        // Left border
        cout << printSide();

        // Cards
        for (int j = 0; j < MAX_CARD_ROW; j++){
            cout << row[j][i];
        }

        // Right border
        cout << printSide() << endl;
    }
}

void TextDisplay::displayMinionInfo (int playerNum, int minionNum) {
	shared_ptr <Minion> m;
    vector<vector<string>> output;
	
	if (playerNum == 1) {
		m = p1->getMinionPlayed(minionNum);
	} else {
		m = p2->getMinionPlayed(minionNum);
	}

    // Print minion i
    for (int i = 0; i < CARD_HEIGHT; i++){
        cout << displayMinion(m)[i] << endl;
    }
    
	// Number of enchantments
	int eCount = m->getEnchantmentCount();

    // If no enchantments
    if (eCount == 0) {
        cout << "This minion has no enchantments." << endl;
        return;
    }

    // Determine number of rows
    int numRows = eCount / MAX_CARD_ROW;
    int remainder = eCount % MAX_CARD_ROW;

    int index = 0;

    // Print each full row of enchantments
	for (int row = 0; row < numRows; row++) {
        // Add to vector to be printed
        for (int i = 0; i < MAX_CARD_ROW; i++) {
			output.emplace_back(displayEnchantment(m->getMinionEnchantments()[index]));
            index++;
        }

        // Print row
        for (int j = 0; j < CARD_HEIGHT; j++){
            for (int k = 0; k < MAX_CARD_ROW; k++){
                cout << output[k][j];
            }
            cout << endl;
        }

        // Reset output
        output.clear();
    }

    // Add remaining enchantments to be printed
    for (int i = 0; i < remainder; i++) {
		output.emplace_back(displayEnchantment(m->getMinionEnchantments()[index]));
        index++;
    }

    // Print remaining enchantments
	for (int j = 0; j < CARD_HEIGHT; j++){
		for (int k = 0; k < remainder; k++){
			cout << output[k][j];
		}
		cout << endl;
	}
}

void TextDisplay::displayBoard() {
    // Top Border
    cout << printBorder(true) << endl;

    // Player 1 edge row
    printRow(displayEdgeCards(1));

    // Player 1 minions
    printRow(displayMinionsPlayed(1));

    // Game name
    for (auto line : displayTitle()) {
        cout << line << endl;
    }

    // Player 2 minions
    printRow(displayMinionsPlayed(2));

    // Player 2 edge row
    printRow(displayEdgeCards(2));

    // Bottom Border
    cout << printBorder(false) << endl;
}

void TextDisplay::notifyDisplay(int playerNum) {}
