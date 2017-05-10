#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	unsigned int windowWidth = 640;
	unsigned int windowHeight = 480;

	//create sfml window 
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Window");

	//create a texture and load an image from file into it
	sf::Texture texture; //note that the texture has to be stored properly while a sprite uses its information
						 //if its destroyed sprites cant use these information anymore (for example a texture declared in the scope of a function)
	texture.loadFromFile("assets//textures//skeleton.png");

	//create a sprite and assign the texture to it
	sf::Sprite sprite(texture);

	//reset origin to the center of the sprite
	sf::Vector2u size(texture.getSize());
	sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);

	//set the position of the sprite to the center of the window
	sprite.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);

	//set speed for the movement in the x dimension
	sf::Vector2f speed(0.01f, 0.0f);

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

	//handle movement
		//adjust direction
		if ((sprite.getPosition().x + (size.x / 2.0f) > windowWidth && speed.x > 0.0f) || //sprite leaves the window on the right side
			(sprite.getPosition().x - (size.x / 2.0f) < 0 && speed.x < 0.0f))			  //sprite leaves the window on the left side
		{ 
			//invert the direction
			speed.x = -speed.x;
		}
		
		//update position
		sprite.setPosition(sprite.getPosition() + speed);


		//clear the window
		window.clear(sf::Color::Black);

		//draw something here
		window.draw(sprite);

		//display on screen what has been rendered to the window so far
		window.display();
	}

}