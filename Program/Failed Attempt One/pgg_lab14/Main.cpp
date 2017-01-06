#include "GameWorld.h"

int main(int, char**)
{
	GameWorld *world = new GameWorld();

	world->updateGame();

	// Exit Game
	return 0;
}