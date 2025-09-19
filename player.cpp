#include "player.h"

void playerMove(Player* player, int dir) {
	player.x += player.speed + dir;
}
