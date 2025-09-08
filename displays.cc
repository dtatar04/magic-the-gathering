#include "displays.h"

Displays::Displays (shared_ptr <Player> p1, shared_ptr <Player> p2): p1 {p1}, p2 {p2} {}

void Displays::notifyActivePlayer(bool ap) {
	if (!ap) {
		activePlayer = 1;
	} else {
		activePlayer = 2;
	}
}
