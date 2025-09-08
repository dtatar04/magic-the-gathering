#include "minion.h"
using namespace std;

// Minion's ctor
//   Sets attack, defense, activated and triggered abilities based on name
//   Sets isDead status to false, enchantmentCount to 0, and actions to 0
Minion::Minion(string name, Player& p): Card{name, "Minion", p}, actions{0}, toAddActions{1}, isDead{false}, 
    isSilenced{false}, enchantmentCount{0} {
    // Constructs a Minion based on its name
    // Constructs an Air Elemental Minion
    if (name == "Air Elemental") {
        setCost(0);
        attack = 1;
        defense = 1;
        hasTA = false;
        hasAA = false;
    
    // Constructs an Air Elemental Minion
    } else if (name == "Earth Elemental") {
        setCost(3);
        attack = 4;
        defense = 4;
        hasTA = false;
        hasAA = false;
    
    // Constructs an Air Elemental Minion
    } else if (name == "Bone Golem") {
        setCost(2);
        attack = 1;
        defense = 3;
        hasTA = true;
        hasAA = false;
        ta = make_unique<TriggeredAbility>("Bone Golem");

    // Constructs an Air Elemental Minion
    } else if (name == "Potion Seller") {
        setCost(2);
        attack = 1;
        defense = 3;
        hasTA = true;
        hasAA = false;
        ta = make_unique<TriggeredAbility>("Potion Seller");

    // Constructs an Air Elemental Minion
    } else if (name == "Novice Pyromancer") {
        setCost(1);
        attack = 0;
        defense = 1;
        hasTA = false;
        hasAA = true;
        aa = make_unique<ActivatedAbility>("Novice Pyromancer");
            
    // Constructs an Air Elemental Minion
    } else if (name == "Apprentice Summoner") {
        setCost(1);
        attack = 1;
        defense = 1;
        hasTA = false;
        hasAA = true;
        aa = make_unique<ActivatedAbility>("Apprentice Summoner"); 
        
    // Constructs an Air Elemental Minion
    } else if (name == "Fire Elemental") {
        setCost(2);
        attack = 2;
        defense = 2;
        hasTA = true;
        hasAA = false;
        ta = make_unique<TriggeredAbility>("Fire Elemental");
        
    // Constructs an Air Elemental Minion
    } else if (name == "Master Summoner") {
        setCost(3);
        attack = 2;
        defense = 3;
        hasTA = false;
        hasAA = true;
        aa = make_unique<ActivatedAbility>("Master Summoner"); 
    }
}

// Copy ctor
Minion::Minion(Minion& other): Card{other.getName(), other.getType(), other.getOwner()},
    attack{other.getAttack()}, defense{other.getDefense()}, actions{other.getActions()},
    hasTA(other.getBTA()), hasAA{other.getBAA()}, isDead{other.checkDeath()}, 
    isSilenced{other.getSilenced()}, enchantmentCount{other.getEnchantmentCount()} {

        if (hasTA) {
            ta = make_unique<TriggeredAbility>(*other.ta);
        }
        if (hasAA) {
            aa = make_unique<ActivatedAbility>(*other.aa);
        }
        for (const auto& enchantment : other.enchantments) {
            enchantments.push_back(make_shared<Enchantment>(*enchantment));
        }
}

bool Minion::getBTA() {
    return hasTA;
}
bool Minion::getBAA() {
    return hasAA;
}

// called by command use i p t or use i
void Minion::use(Player &ap, Player &tp, int i, int t) {
    // If Minion has an activated ability
    if (getBAA()) {
        if (getActions() < 1) {
            cout << "Minion does not have enough action points to use card.";
            return;
        }
        getActivatedAbility()->use(ap, tp, i , t);
        actions--;
    } else {
        cout << "Cannot use a minion with no activated ability." << endl;
    }
}

void Minion::modAttack(int n) {
    attack += n;
}

bool Minion::modDefense(int n) {
    bool amIAlive = true;
    defense += n;
    if (getDefense() <= 0) {
        amIAlive = false;
        defense = 0;
        notifyObservers(State::MinionExitsPlay);
    }
    return amIAlive;
}

int Minion::getAttack() {
    return attack;
}

int Minion::getDefense() {
    return defense;
}

void Minion::setDefense(int n){
    defense = n;
}

int Minion::getActions() {
    return actions;
}

void Minion::setActions(int i) {
    actions = i;
}

bool Minion::getSilenced() {
    return isSilenced;
}

// Minion attacks Player p
//   The player loses life equal to the attack value of the minion.
void Minion::attackPlayer(Player& p) {
    if (actions >= 1) {
        p.modLife(-this->getAttack());
        actions--;
    } else {
        cout << this->getName() << " does not have an action point. Minion cannot attack." << endl;
        return;
    }
    
}

// Minion attacks opposing Minion m
//   Both minions damage eachother: our minion reduces minion m's defense by 
//   our minion's attack. Then minion m damages our minion in the same way.
// Returns true if our minion is still alive, false otherwise
bool Minion::attackMinion(Player& tp, Minion& m, int i) {
    bool isMinionAlive = true;
    if (actions >= 1) {

        // Remove opponent's minion if dead
        if(!m.modDefense(-this->getAttack())) { // Reduces m's defense by our minion' attack
            tp.switchLocation("minions played", "graveyard", i);
        }
        isMinionAlive = this->modDefense(-m.getAttack()); // Reduces our minion's defense by m's attack
        actions--;
        return isMinionAlive;
    } else {
        cout << this->getName() << " does not have an action point. Minion cannot attack." << endl;
        return true;
    }
}

int Minion::getEnchantmentCount() {
    return enchantmentCount;
}

void Minion::incEnchantmentCount() {
    enchantmentCount++;
}

void Minion::decEnchantmentCount() {
    enchantmentCount--;
}

int Minion::getToAddActions() {
    return toAddActions;
}

// Adds an enchantment to minion
//   i is the position of enchantment in hand
//   t is the positon of minion on board of ap
// All enchantments in deck are passive
bool Minion::addEnchantment(int i, Player& ap, int t) {
    shared_ptr<Card> cardPtr = ap.getCardHand(i);
    shared_ptr<Enchantment> toAdd = dynamic_pointer_cast<Enchantment>(cardPtr);;
    string eName = toAdd->getName();

    // Giant Strength apply
    if (eName == "Giant Strength") {
        this->modAttack(toAdd->getModValue());
        this->modDefense(toAdd->getModValue());
    
    // Enrage apply
    } else if (eName == "Enrage") {
        attack *= toAdd->getModValue();
        defense *= toAdd->getModValue();
    
    // Haste apply
    } else if (eName == "Haste") {
        toAddActions++;

    // Magic Fatigue apply
    } else if (eName == "Magic Fatigue") {
        if (getBAA()) {
            aa->setActCost(aa->getActCost() + 2);
        }

    // Silence apply
    } else if (eName == "Silence") {
        isSilenced = true;
    } else {
        cout << "Tried to add invalid enchantment" << endl;
        return false;
    }
    enchantments.push_back(toAdd);
    incEnchantmentCount();
    return true;
}

void Minion::removeEnchantment() {
    if (getEnchantmentCount() == 0) {
        cout << "You disenchanted a minion with no enchantments. No effect." << endl;
        return;
    }
    // name of top enchantment
    string topE = enchantments.back()->getName();

    // Giant Strength delete
    if (topE == "Giant Strength") {
        this->modAttack(-2);
        this->modDefense(-2);
    
    // Enrage delete
    } else if (topE == "Enrage") {
        attack = attack / 2;
        defense = defense / 2;
    
    // Haste delete
    } else if (topE == "Haste") {
        toAddActions--;

    // Magic Fatigue delete
    } else if (topE == "Magic Fatigue") {
        aa->setActCost(aa->getActCost() - 2);

    // Silence delete
    } else if (topE == "Silence") {
        isSilenced = false;
    } 
    // removes last enchantment
    enchantments.pop_back();
    decEnchantmentCount();
}

// Returns a shared_ptr vector of enchantments. To be used for display
vector<shared_ptr<Enchantment>> Minion::getMinionEnchantments() {
    return enchantments;
}

unique_ptr<TriggeredAbility> & Minion::getTriggeredAbility(){
    return ta;
}

unique_ptr<ActivatedAbility> & Minion::getActivatedAbility() {
    return aa;
}

string Minion::getDescription() {
    // If a Minion has an actived ability, returns description of that AA
    if (hasAA) {
        return aa->getDescription();
    } else if (hasTA) {
        return ta->getDescription();
    } else {
        return "";
    }
}

// Checks whether a Minion is dead or not
//   If defense <= 0, sets isDead to true
bool Minion::checkDeath() {
    if (isDead) return isDead;
    
    if (defense <= 0) {
        isDead = true;
    } 
    return isDead;
}


void Minion::notify(State gState) {
    // If Minion exists play
    if (gState == State::MinionExitsPlay) {
        // If activated by the correct Minion
        if (getName() == "Bone Golem") { 
            // Apply Bone Golem's triggered ability
            // Get observer index
            int index = 0;
            for (int i = 0; i < getOwner().getSizeMinionsPlayed(); i++) {
                if (getOwner().getMinionPlayed(i)->getName() == "Bone Golem") {
                    index = i;
                    break;
                }
            }
            ta->use((getOwner()), (getOwner()), index); 

        }
    }
}

void Minion::notify(shared_ptr<Subject> whoNotified, State gState) {
    int index = 0;
    
    // If Minion enters play
    if (gState == State::MinionEntersPlay) {
        // If activated by the correct Minion
        if (getName() == "Fire Elemental") { 
            // Get observer index
            for (int i = 0; i < getOwner().getSizeMinionsPlayed(); i++) {
                if (getOwner().getMinionPlayed(i)->getName() == "Fire Elemental") {
                index = i;
                break;
                }
            }
            // Apply Fire Elemental's ability
            auto min = dynamic_pointer_cast<Minion>(whoNotified);
            if (min) {
                ta->use((getOwner()), (min->getOwner()), index);
            } else {
                cout << "Observer gone wrong in Minion notify Fire elem" << endl;
            }
            
        }


    // If End of Turn
    } else if (gState == State::EndOfTurn) {
        // Get observer index
            for (int i = 0; i < getOwner().getSizeMinionsPlayed(); i++) {
                if (getOwner().getMinionPlayed(i)->getName() == "Potion Seller") {
                index = i;
                break;
                }
            }
        // If activated by correct Minion
        if (getName() == "Potion Seller") {
            ta->use((getOwner()), (getOwner()), index);
        }

    }
}

