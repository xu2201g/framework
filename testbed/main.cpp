#include "Game.h"

#include <iostream>
#include <memory>

void test();

int main(int argc, char** argv)
{
	Game game;

	while (!game.GetWindow()->IsDone())
	{
		//update model
		game.Update();

		//render result
		game.Render();

		//sf::sleep(sf::seconds(0.5)); // sleep for 0.2 seconds

		//restart clock
		game.RestartClock();
	}

	//test();
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