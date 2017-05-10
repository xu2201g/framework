//#include <iostream>

#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Window");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				//close window button clicked
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		//draw 

		window.display();
	}

	//std::cout << "Hello World!" << std::endl;
	system("pause");
}