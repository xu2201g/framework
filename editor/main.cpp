#include <sfml/Graphics.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 768

int main(int argc, char** argv)
{
	sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "editor");

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		std::cout


	}

	return 0;
}