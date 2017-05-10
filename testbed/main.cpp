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

		//sf::sleep(sf::seconds(0.5)); // sleep for 0.2 seconds

		//restart clock
		game.RestartClock();
	}
}