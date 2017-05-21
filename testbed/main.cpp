#include "Game.h"

#include <iostream>
#include <memory>

#include "TextureManager.h"

void test();

void testTextureMgr();

int main(int argc, char** argv)
{
	Game game;

	while (!game.GetWindow()->IsDone())
	{
		//update model
		game.Update();

		//render result
		game.Render();

		//sf::sleep(sf::seconds(0.2)); // sleep for 0.2 seconds

		//restart clock
		game.LateUpdate();
	}

	//test();
	//testTextureMgr();

	//system("pause");
}

struct foo
{
	foo(int v)
		:
		value(v)
	{}

	int value;
};

void print(foo& f)
{
	std::cout << "value: " << f.value << std::endl;
}

void test()
{
	foo* p;
	{
		std::unique_ptr<foo> ptr(new foo(5));

		p = &(*ptr);
		p->value = 2;
	}

	
	print(*p);
	

	system("pause");
}

void testTextureMgr()
{
	sf::Texture* pTexture;
	//{ //texturemgr and its loaded textures wouldnt be stored in memory anymore after this block is processed - the scope is left 
	//and the sprite wouldnt hold valid texture informations anymore so a white rectangle is rendered instead
	TextureManager textureMgr;

	textureMgr.RequireResource("Intro");

	pTexture = textureMgr.GetResource("Intro");
	//}

	sf::Sprite sprite;
	sprite.setTexture(*pTexture);

	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::R)
					{
						textureMgr.ReleaseResource("Intro");
					}
				}
		}
		window.clear(sf::Color::Black);
		window.draw(sprite);
		window.display();
	}
}