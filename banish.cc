#include "banish.h"
using namespace std;

// Ctor
Banish::Banish(string name, Player& p): Spell{name, p} {
    setCost(2);
    setDescription("Destroy target minion or ritual");
}

// Destroys minion t
void Banish::playCard(Player& ap, Player& tp, Minion& m, int i, int t) {
    if (t >= tp.getSizeMinionsPlayed()) {
        cout << "Banish failed. Cannot play spell on minion that does not exist." << endl;
    } else {
        tp.switchLocation("minions played", "destroy", t);
    }
}

// Destroy ritual r
void Banish::playCard(Player& p, Ritual& r) {
    p.switchLocation("ritual", "destory", 0);
} 


