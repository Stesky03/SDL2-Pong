#include <SDL.h>
#include <iostream>
#include "Game.h"
#include <string>
using std::string;

const int FPS = 150;
const int DELAY_TIME = 1000.0f / FPS;

Game* game = 0;
int main(int argc, char* argv[])
{
	Uint32 frameStart, frameTime;
	game = new Game();
	game->init("Pong", 100, 100, 1280, 720, SDL_WINDOW_FULLSCREEN_DESKTOP);
	while (game->running())
	{
		frameStart = SDL_GetTicks();
		game->handleEvents();
		if(!game->finish)
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));
		}
	}
	game->clean();
	return 0;
}