#include "Game.h"

int main()
{
	srand(static_cast<unsigned>(time(NULL)));
	
	Game game;

	while (game.isRunning())
	{
		game.update();
		game.render();
	}
}