#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

#include "spell.h"
#include "player.h"
#include "banish.h"
#include "unsummon.h"
#include "recharge.h"
#include "disenchant.h"
#include "raiseDead.h"
#include "blizzard.h"
#include "minion.h"

using namespace std;

Player::Player (int num, string name, string deckFile, int magic, bool testing): playerNum {num}, name {name}, life {20}, magic {magic}, testingMode{testing} {
	initDeck (deckFile, testing);
	for (int i = 0; i < 5; i++) {
		draw();
	}
}

int Player::getPlayerNum() {
	return playerNum;
}

string Player::getName() {
	return name;
}

int Player::getLife() {
	return life;
}

void Player::modLife(int i) {
	life += i;
}

int Player::getMagic() {
	return magic;
}

void Player::modMagic(int i) {
	magic += i;
}

int Player::getSizeDeck() {
	return static_cast <int> (deck.size());
}

int Player::getSizeHand() {
	return static_cast <int> (hand.size());
}

int Player::getSizeGraveyard() {
	return static_cast <int> (graveyard.size());
}

int Player::getSizeMinionsPlayed() {
	return static_cast <int> (minionsPlayed.size());
}

int Player::getSizeRitual() {
	return static_cast <int> (ritual.size());
}

shared_ptr <Card> Player::getCardDeck (int i) {
	return deck[i];
}

shared_ptr <Card> Player::getCardHand (int i) {
	return hand[i];
}

shared_ptr <Minion> Player::getMinionGraveyard(int i) {
	return graveyard[i];
}

shared_ptr <Minion> Player::getMinionPlayed(int i) {
	return minionsPlayed[i];
}

shared_ptr <Card> Player::popCard (vector <shared_ptr <Card>> &v, int i) {
	shared_ptr <Card> topCard = v[i];
	v.erase(v.begin() + i);
	return topCard;
}

shared_ptr <Card> Player::popCard (vector <shared_ptr <Minion>> &v, int i) {
	shared_ptr <Card> topCard = v[i];
	v.erase(v.begin() + i);
	return topCard;
}

void Player::pushCard (vector <shared_ptr <Card>> &v, shared_ptr <Card> &c) {
	v.emplace_back(c);
}

void Player::pushCard (vector <shared_ptr <Minion>> &v, shared_ptr <Minion> &m) {
	v.emplace_back(m);
}

void Player::pushToGraveyard (shared_ptr <Minion> &m) {
	graveyard.insert(graveyard.begin(), m);
}

void Player::pushToMinionsPlayed (shared_ptr <Minion> &m) {
	minionsPlayed.emplace_back(m);
}

void Player::switchLocation (string cs1, string cs2, int i) {
	shared_ptr <Card> c;
	if (cs1 == "deck") {
		c = popCard(deck, i);
	} else if (cs1 == "hand") {
		c = popCard(hand, i);
	} else if (cs1 == "graveyard") {
		c = popCard(graveyard, i);
	} else if (cs1 == "ritual") {
		if (!ritual.empty()) {
        	ritual.pop_back();
    	}
	} else {
		// removes all enchantments before minion switches zones
		while(getMinionPlayed(i)->getEnchantmentCount()) {
			getMinionPlayed(i)->removeEnchantment();
		}
		c = popCard(minionsPlayed, i);
	}

	if (cs2 == "deck") {
		pushCard (deck, c);
	} else if (cs2 == "hand") {
		pushCard (hand, c);
	} else if (cs2 == "graveyard") {
		shared_ptr <Minion> m = static_pointer_cast <Minion> (c);
		pushToGraveyard(m);
	} else if (cs2 == "minions played") {
		shared_ptr <Minion> m = static_pointer_cast <Minion> (c);
		pushCard (minionsPlayed, m);
	} else if (cs2 == "ritual") {
		shared_ptr <Ritual> r = static_pointer_cast <Ritual> (c);
		setRitual(r);
	}
}

void Player::initDeck (string deckFile, bool testing) {
	ifstream ifs (deckFile);
	string line;

	while (getline (ifs, line)) {
		istringstream iss {line};
		string cardName;
		iss >> cardName;

		// Adding Spells
		if (cardName == "Banish") {
			shared_ptr <Banish> card = make_shared <Banish> (line, *this);
			deck.emplace_back(card);
		} else if (cardName == "Unsummon") {
			shared_ptr <Unsummon> card = make_shared <Unsummon> (line, *this);
			deck.emplace_back(card);
		} else if (cardName == "Recharge") {
			shared_ptr <Recharge> card = make_shared <Recharge> (line, *this);
			deck.emplace_back(card);
		} else if (cardName == "Disenchant") {
			shared_ptr <Disenchant> card = make_shared <Disenchant> (line, *this);
			deck.emplace_back(card);
		} else if (cardName == "Raise") {
			shared_ptr <RaiseDead> card = make_shared <RaiseDead> (line, *this);
			deck.emplace_back(card);
		} else if (cardName == "Blizzard") {
			shared_ptr <Blizzard> card = make_shared <Blizzard> (line, *this);
			deck.emplace_back(card);
		
		// Adding Rituals
		} else if (cardName == "Dark" || cardName == "Aura" || cardName == "Standstill") {
			shared_ptr <Ritual> card = make_shared <Ritual> (line, *this);
			deck.emplace_back(card);
		
		// Adding Enchantments
		} else if (cardName == "Giant" || cardName == "Enrage" || cardName == "Haste" || cardName == "Magic" || cardName == "Silence") {
			shared_ptr <Enchantment> card = make_shared <Enchantment> (line, *this);
			deck.emplace_back(card);
		
		// Adding Minion
		} else {
			shared_ptr <Minion> card = make_shared <Minion> (line, *this);
			deck.emplace_back(card);
		}
	}

	if (!testing) {
		shuffleDeck();
	}
}

void Player::shuffleDeck () {
	// use a time-based seed for the default seed value
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine rng{seed};
	shuffle( deck.begin(), deck.end(), rng );
}

void Player::draw() {
	if (getSizeDeck() > 0 && getSizeHand() < 5) {
		switchLocation ("deck", "hand", 0);
	}
}

void Player::discard(int i) {
	hand.erase(hand.begin() + i);
}

void Player::setRitual(shared_ptr<Ritual> &r) {
	// Remove the first element
	if (!ritual.empty()) {
    	ritual.erase(ritual.begin());
	}

	// Add a new element to the vector
	ritual.push_back(r);
}

shared_ptr<Ritual> Player::getRitual() {
	if (getSizeRitual() != 0) {
        return ritual.front();
    } else {
        return nullptr;
    }
}

void Player::minionAttacksMinion(int i, int j, Player& np) {
	// If my minion died, move to graveyard
	bool myMinionIsAlive = getMinionPlayed(i)->attackMinion(np, *(np.getMinionPlayed(j)), j);
	if(!myMinionIsAlive) {
		switchLocation("minions played", "graveyard", i);
	}
}

void Player::minionAttacksPlayer(int i, Player &np) {
	getMinionPlayed(i)->attackPlayer(np);
}

void Player::playCardNoTarget(Player& ap, Player& np, int i) {
	if (i < getSizeHand() && i >= 0) {
		string type = getCardHand(i)->getType();
		string cName = getCardHand(i)->getName();
		int cost = getCardHand(i)->getCost();
		auto toPlay = hand.begin() + i;

		// Check if player has enough magic
		if (!testingMode && magic < cost) {
			cout << "Player does not have enough magic to play card." << endl;
			return;
		}

		if (type == "Minion") {

			if (ap.getSizeMinionsPlayed() >= FULL_HAND) {
        		cout << "Hand is full. ";
        		cout << "Unable to move " << ap.getCardHand(i)->getName() << " to board." << endl;
				return;
			}

			// Attaches all minions of the player's board to new minion observers
			attachObserversToMinion(*toPlay);

			// Add new minion to Board
			switchLocation("hand", "minions played", i);

			// Attach new minion as an obsever of all minions on ap's board
			attachMinionToObservers(*toPlay);

			// Attach other player's minion to new minion observers
			np.attachObserversToMinion(*toPlay);

			// Attach new minion to be an observer of all opponent's minions
			np.attachMinionToObservers(*toPlay);

			bool added = true;

			// Need to check our Ritual first because APNAP order
			if (getSizeRitual() == 1) {
				auto ritual = getRitual();
				// Only use Ritual if relevant
                if (ritual->getName() == "Aura of Power") {
                    ritual->use(*this, np, 0);
                } else if (ritual->getName() == "Standstill") {
                    ritual->use(*this, np, 0);
					added = false;
                }
            }
			// Do the same for opponent's Ritual
			if (np.getSizeRitual() == 1 && added) {
                auto oRitual = np.getRitual();
                // Use ritual
                if (oRitual->getName() == "Standstill") {
                    oRitual->use(*this, np, 0);
					added = false;
                }
            }
			// Only if Standstill was not just played
			if (added) {
				// For Fire Elemental: notifies a minion has entered play
				np.notifyMinions(getMinionPlayed(getSizeMinionsPlayed() - 1), State::MinionEntersPlay);
			}
		} // End of Minion

		else if (type == "Ritual") {
			// Adds Ritual to board
			switchLocation("hand", "ritual", i);
		}  

		else if (type == "Spell") {
			auto spellPlay = static_pointer_cast<Spell>(*toPlay);
			// See if Card is of type without a target
			if (cName == "Blizzard" || cName == "Raise Dead" || cName == "Recharge") {
				spellPlay->playCard(ap, np, i);
				switchLocation("hand", "destroy", i);
			}  else {
				cout << "This spell requires a target."  << endl;
			}
		} else {
			cout << "Try using the play i p t command instead." << endl;
			return;
		}
		if (!testingMode) {
			modMagic(-cost); // Only runs if play is a success
		} else {
			if (magic - cost < 0) {
				magic = 0;
			} else {
				modMagic(-cost);
			}
		}
	} 
}

void Player::playCardWithTarget(Player& tp, int i, int t) {
	auto toPlay = hand.begin() + i;
	string cType = (*toPlay)->getType();
	string cName = (*toPlay)->getName();
	int cost = (*toPlay)->getCost();

	// Checks if player has enough magic to play card
	if (!testingMode && magic < cost) {
		cout << "Player does not have enough magic to play card." << endl;
		return;
	}

	// Enchantment 
	if (cType == "Enchantment") {
		if (t >= tp.getSizeMinionsPlayed()) {
				cout << "Cannot play enchantment on minion that does not exist." << endl;
				return;
		} else {
			tp.getMinionPlayed(t)->addEnchantment(i, *this, t);
			switchLocation("hand", "destroy", i);
		}

	// Spell
	} else if (cType == "Spell") {

		// Checks if spell is one with a target
		if (cName == "Banish" || cName == "Unsummon" || cName == "Disenchant") {
			// Plays Spell on target player's t-th Minion
			if (t >= tp.getSizeMinionsPlayed()) {
				cout << "Cannot play spell on minion that does not exist." << endl;
				return;
			} else {
				getCardHand(i)->playCard(*this, tp, *(tp.getMinionPlayed(t)), i, t);
			}
		} else {
			cout << "This spell must be played using the play i command." << endl;
			return;
		}
		// Destroys played card
		switchLocation("hand", "destroy", i);
	} else {
		cout << "Wrong command used." << endl;
		return;
	}
	if (!testingMode) {
		modMagic(-cost); // Only runs if play is a success
	} else {
		if (magic - cost < 0) {
			magic = 0;
		} else {
			modMagic(-cost);
		}
	}
}

void Player::playRitual(Player& tp, Ritual &r, int i) {
	shared_ptr<Card> cardPtr = getCardHand(i);
	if (cardPtr->getName() == "Banish") {
		cardPtr->playCard(tp, r);
	} else {
		cout << "Command failed. You tried to play an invalid card on a ritual." << endl;
	}
}

void Player::useCardNoTarget(Player& ap, Player& np, int i) {
	string cName = ap.getMinionPlayed(i)->getName();
	// Can only be called by Summoners
	if (cName == "Apprentice Summoner" || cName ==  "Master Summoner") {
		getMinionPlayed(i)->use(ap, np, i, i);
	} else {
		cout << "This card cannot be used with the use i command." << endl;
		return;
	}
}

void Player::useCardWithTarget(Player& ap, Player& tp, int i, int t) {
		int cost = getMinionPlayed(i)->getCost();

	// Can only be called by Novice Pyromancer
	if (ap.getMinionPlayed(i)->getName() == "Novice Pyromancer") {
		if (magic < cost) {
			cout << "Player does not have enough magic to play card." << endl;
			return;
		} else {
			getMinionPlayed(i)->use(ap, tp, i, t);
			modMagic(-cost);
		}
		
	} else {
		cout << "This card cannot be used with the use i p t command." << endl;
		return;
	}
}

void Player::restoreActionsToMinions() {
	// sets the appropriate amount of actions to each minion
	for(auto & inPlay : minionsPlayed) {
		if (inPlay->getActions() < 1) {
			inPlay->setActions(inPlay->getToAddActions());
		}
	}
}

void Player::notifyMinions(shared_ptr<Subject> whoNotified, State gState) {
	for (auto & minionInPlay : minionsPlayed) {
        auto minion = static_pointer_cast<Minion>(minionInPlay);

        // Goes through every minion played, if the game state is appropriate, notifies
        if ((minion->getBTA()) && 
			(minion->getTriggeredAbility()->getCond() == gState)) {
            minion->notify(whoNotified, gState);
        }   
    }

}

void Player::attachObserversToMinion(shared_ptr<Card> newMinion) {
	for (auto& onBoard : minionsPlayed) {
        auto minion = dynamic_pointer_cast<Observer>(onBoard);
        auto newMin = dynamic_pointer_cast<Minion>(newMinion);
        
		// If the casting is successful, attach
        if (minion && newMin) {
            newMin->attach(minion);
        }
    }
}

void Player::attachMinionToObservers(shared_ptr<Card>  newMinion) {
	for (auto & minionInPlay : minionsPlayed) {
		// Adds minion to the observer of newMin
		auto minion = dynamic_pointer_cast<Minion>(minionInPlay);

        if (minion) {
            // Assuming newMinion has observers
            minion->attach(dynamic_pointer_cast<Observer>(newMinion));
        }
	}
}
