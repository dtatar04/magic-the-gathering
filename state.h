#ifndef _STATE
#define _STATE

// State describes the four different game states that can trigger an ability

enum class State {
  StartOfTurn,
  EndOfTurn,
  MinionEntersPlay,
  MinionExitsPlay
};

#endif
