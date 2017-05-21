#pragma once

#include <SFML/Graphics.hpp>

#include "SharedContext.h"

#include "EventManager.h"
#include "SpriteSheet.h"

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
enum class SnakeDirection
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

	Snake(int blockSize, SharedContext* pSharedContext); 
	~Snake();

	//setter 
	void SetDirection(SnakeDirection dir);
	void IncreaseScore();
	void IncreaseSpeed();

	//getter
	SnakeDirection GetDirection();
	SnakeDirection GetPhysicalDirection();

	sf::Vector2i GetPosition();
	int GetSpeed();
	int GetLives();
	int GetScore() { return m_score; }

	bool HasLost();

	//logic
	void Lose();
	void ToggleLost();

	void Extend(); //extends the snake body by 1 segment
	void Reset(); //sets snake to starting position and resets the body and score

	void Move(); //moves the snake to the specified direction
	void Tick(); //updates the model
	void UpdateAnimation(const float& dT);

	void Cut(int segments); //cuts an amount of segments specified as param from the snake body - used for snakehead with snakebody collisions

	void Render(sf::RenderWindow& window); //draw function

	void Navigate(EventDetails* details);
	
private:

	SpriteSheet m_spriteSheetHead; //put these to private TODO
	SpriteSheet m_spriteSheetBody;

	void CheckCollision(); //checks for collisions snake-snakebody

	SnakeContainer m_snakeBody; //vector of snake segments
	std::vector<sf::Color> m_bodyColorContainer;
	int m_size; //size of the blocks
	SnakeDirection m_dir; //current direction
	int m_speed; //speed of the snake 
	int m_lives; 
	int m_score;
	bool m_lost;

	sf::RectangleShape m_bodyRect; //shape used to render snakesegments

	

	SharedContext* m_pSharedContext;
};