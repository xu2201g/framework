#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	unsigned int windowWidth = 640;
	unsigned int windowHeight = 480;

	//create sfml window 
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Window");

	//create a cicleshape 
	float radius = 100.0f;
	sf::CircleShape circle(radius);
	circle.setFillColor(sf::Color::Cyan);

	//top left corner of the cicle area
	//circle.setPosition(100, 100);

	//set origin at the center of the cicle
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(100, 100);

	//window rendering loop
	while (window.isOpen())
	{
		//process all window events by polling one after another
		sf::Event event;
		while (window.pollEvent(event))
		{
			//close the window if we get a close event
			if (event.type == sf::Event::Closed)
			{
				//close window button clicked
				window.close();
			}
		}

		//clear the window
		window.clear(sf::Color::Black);

		//draw something here
		window.draw(circle);

		//display on screen what has been rendered to the window so far
		window.display();
	}

}