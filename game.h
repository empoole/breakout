#ifndef GAME_STATE
#define GAME_STATE

#include "player.h"

struct Game {
	Player* player;
	bool isPaused, isRunning;
};

#endif // GAME_STATE
