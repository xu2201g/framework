#include "Game.h"

int main(int argc, char** argv)
{
	Game game;

	while (!game.GetWindow()->IsDone())
	{
		//handle keyboard mouse etc. events
		game.HandleInput();

		//update model
		game.Update();

		//render result
		game.Render();

		//restart clock
		game.RestartClock();
	}
}