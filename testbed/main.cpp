#include "Game.h"

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <memory>


#include "TextureManager.h"

//relative path used almost everywhere
//../../testbed/assets/

void test();

void testTextureMgr();

class foo
{
public:
	foo(int v)
	:
	value(v)
	{}

	int value;
};

class StoresMap
{
public:
	std::unordered_map<std::string, std::pair<std::unique_ptr<foo>, unsigned int>> m_resources;
};

bool foooo(bool a, bool b, bool c, bool d, bool e, bool f, bool g)
{

	return false;
}

bool fooooo(unsigned char c)
{
	return c;
}

int main(int argc, char** argv)
{
	bool a = false;
	bool b = true;
	bool c = false;
	bool d = false;
	bool e = false;
	bool f = false;
	bool g = false;

	

	unsigned char byteA = 1; //0000 0001
	unsigned char byteB = 128; // 1000 0000

	unsigned char bitmask = 1 + 8 + 64; // 0100 1001

	std::cout << (bool)(byteA & byteB) << std::endl; //returns false
	std::cout << (bool)(byteA & bitmask) << std::endl; //returns true
	std::cout << (bool)(byteB & bitmask) << std::endl; //returns false



	foo f1(1);
	foo f2(2);

	foo* pRaw1;
	foo* pRaw2;

	std::shared_ptr<foo> p1(std::make_shared<foo>(f1));	
	pRaw1 = p1.get();

	std::shared_ptr<foo> p2 = std::move(p1);
	pRaw2 = p2.get();


	std::cout << "p1Raw1: " << pRaw1 << std::endl;
	std::cout << "p2Raw2: " << pRaw2 << std::endl;
	//
	//foo* pfoo;
	//StoresMap* pMap;
	//{
	//	StoresMap map;
	//	pMap = &map;
	//	map.m_resources.emplace("name", std::make_pair<std::unique_ptr<foo>, unsigned int>(std::make_unique<foo>(foo(7)), 1));

	//	pfoo = map.m_resources.find("name")->second.first.get();
	//}
	//pMap->m_resources.find("name");
	//pfoo->value;

	//std::cout << "hurz" << std::endl;


	Game game;

	while (!game.GetWindow()->IsDone())
	{
		//update model
		game.Update();

		//render result
		game.Render();
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
		
		//restart clock
		game.LateUpdate();

		//sf::sleep(sf::seconds(0.01f));
	}

	//test();
	//testTextureMgr();

	//system("pause");
}


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