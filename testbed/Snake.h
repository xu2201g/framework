#pragma once

#include <SFML/Graphics.hpp>

//holds the grid coordinates for the snakesegment
struct SnakeSegment
{
	SnakeSegment(int x, int y)
		:
		position(x, y)
	{}

	sf::Vector2i position;
};

//holds all segments 
using SnakeContainer = std::vector<SnakeSegment>;

//specifies the direction the snake is heading to - none is only used at the start of the game
enum class Direction
{
	None,
	Up, 
	Down, 
	Left,
	Right
};

class Snake
{
public:

	Snake(int blockSize); 
	~Snake();


	//setter 
	void SetDirection(Direction dir);
	void IncreaseScore();
	void IncreaseSpeed();

	//getter
	Direction GetDirection();
	Direction GetPhysicalDirection();

	sf::Vector2i GetPosition();
	int GetSpeed();
	int GetLives();

	bool HasLost();

	//logic
	void Lose();
	void ToggleLost();

	void Extend(); //extends the snake body by 1 segment
	void Reset(); //sets snake to starting position and resets the body and score

	void Move(); //moves the snake to the specified direction
	void Tick(); //updates the model

	void Cut(int segments); //cuts an amount of segments specified as param from the snake body - used for snakehead with snakebody collisions

	void Render(sf::RenderWindow& window); //draw function

private:

	void CheckCollision(); //checks for collisions snake-snakebody

	SnakeContainer m_snakeBody; //vector of snake segments
	int m_size; //size of the blocks
	Direction m_dir; //current direction
	int m_speed; //speed of the snake 
	int m_lives; 
	int m_score;
	bool m_lost;

	sf::RectangleShape m_bodyRect; //shape used to render snakesegments
};