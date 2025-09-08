#ifndef MINION
#define MINION

#include "card.h"
#include "player.h"
#include "enchantment.h"
#include "triggeredAbility.h"
#include "activatedAbility.h"
#include "observer.h"
#include "subject.h"
#include "state.h"

#include <memory>

using namespace std;

class Player;
class ActivatedAbility;
class TriggeredAbility;

// The Minion class is-a Card, Observer, and Subject
// Minions can attack other minions and players, hold activated and
//   triggered abilities, and gain enchantments.
// Minions observe eachother. 

class Minion: public Card, public Subject, public Observer {
    int attack;
    int defense;
    int actions; 
    int toAddActions;
    bool hasTA;
    bool hasAA;
    bool isDead;
    bool isSilenced;
    unique_ptr<TriggeredAbility> ta;
    unique_ptr<ActivatedAbility> aa;
    int enchantmentCount; 
    vector<shared_ptr<Enchantment>> enchantments;

public:
    // Minion Ctor
    Minion(string name, Player& p);

    // Minion coyp ctor
    Minion(Minion& other);

    // Called when minion uses an activated ability
    void use(Player &ap, Player &tp, int i, int t);

    void modAttack(int n); // adds n to attack
    bool modDefense(int n); // adds n to defense
    int getAttack(); // gets Minion's attack
    int getDefense(); // gets Minion's defense
    void setDefense(int n); // sets Minion's defense to n
    int getActions(); // gets Minion's actions
    void setActions(int i); // sets Minion's actions
    bool getSilenced(); // gets whether Minion is enchanted
    int getToAddActions(); // gets how many actions a minion gets at start of turn

    bool getBTA(); // gets hasTA
    bool getBAA(); // gets hasAA

    // Adds an enchantment to minion
    //   i is the position of enchantment in hand
    //   t is the positon of minion on board of ap
    bool addEnchantment(int i, Player& ap, int t);

    // Used by Disenchant spell
    void removeEnchantment();

    int getEnchantmentCount(); // Gets enchantmentCount
    void incEnchantmentCount(); // Increments enchantmentCount by 1
    void decEnchantmentCount(); // Decrements enchantmentCount by 1
        
    // Returns a vector of all the descriptions of the enchantments a Minion has on
    vector<shared_ptr<Enchantment>> getMinionEnchantments();

    unique_ptr<TriggeredAbility> & getTriggeredAbility();
    unique_ptr<ActivatedAbility> & getActivatedAbility();

    void attackPlayer(Player& p); // Minion attacks opposing Player p
    bool attackMinion(Player &tp, Minion& m, int i); // Minion attacks opposing Minion m

    bool checkDeath(); // checks whether minion is dead or not

    // Overrides Card's getDescription
    string getDescription() override;

    // Subject notifies minion about game state
    void notify(shared_ptr<Subject> whoNotified, State gState);
    // Minion is notified about game state
    void notify(State gState);

    // Minion Dtor
    virtual ~Minion() = default;

};

#endif
