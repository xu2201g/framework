#include "Snake.h"

Snake::Snake(int blockSize)
	:
	m_size(blockSize)
{
	m_bodyRect.setSize(sf::Vector2f(m_size-1, m_size-1)); //used to see the segments more clearly
	Reset(); //resets lives score segments and position of the snake
}

Snake::~Snake()
{
}

void Snake::SetDirection(Direction dir)
{
	m_dir = dir;
}

void Snake::IncreaseScore()
{
	m_score += 10;
}

void Snake::IncreaseSpeed()
{
	//limit max speed of the snake to a cute little magic number 30x ticks per second
	m_speed = std::min(m_speed + 1, 20);
}

Direction Snake::GetDirection()
{
	return m_dir;
}

Direction Snake::GetPhysicalDirection()
{
	if (m_snakeBody.size() <= 1)
	{
		return Direction::None;
	}

	SnakeSegment& head = m_snakeBody[0];
	SnakeSegment& neck = m_snakeBody[1];

	if (head.position.x == neck.position.x)
	{
		return head.position.y > neck.position.y ? Direction::Down : Direction::Up;
	}
	else
	if(head.position.y == neck.position.y)
	{
		return head.position.x > neck.position.x ? Direction::Right : Direction::Left;
	}

	return Direction::None;
}

sf::Vector2i Snake::GetPosition()
{
	//first inserted segment represents snakes head and its position represents snakes position
	return !m_snakeBody.empty() ? m_snakeBody.front().position : sf::Vector2i(1,1);
}

int Snake::GetSpeed()
{
	return m_speed;
}

int Snake::GetLives()
{
	return m_lives;
}

bool Snake::HasLost()
{
	return m_lost;
}

void Snake::Lose()
{
	m_lost = true;
}

void Snake::ToggleLost()
{
	m_lost = !m_lost;
}

void Snake::Extend()
{
	//invalid snake bodys wont be extended
	if (m_snakeBody.empty()) { return; }
	
	//store the last segment - end of tail
	SnakeSegment& last = m_snakeBody[m_snakeBody.size() - 1]; 

	//H ...head
	//# ...body
	//S ...second last segment
	//L ...last segment

	//snake got more segments besides the head - H#####SL
	if (m_snakeBody.size() > 1)
	{
		//store the second last segment
		SnakeSegment& secondLast = m_snakeBody[m_snakeBody.size() - 2];

		if (last.position.x == secondLast.position.x) //both aligned vertically
		{
			if (last.position.y > secondLast.position.y) 
			{											 
				m_snakeBody.push_back(SnakeSegment(last.position.x, last.position.y + 1)); 				
			}
			else
			{
				m_snakeBody.push_back(SnakeSegment(last.position.x, last.position.y - 1));
			}
		}
		else
		if(last.position.y == secondLast.position.y) //both aligned horizontally
		{
			if (last.position.x > secondLast.position.x)
			{
				m_snakeBody.push_back(SnakeSegment(last.position.x + 1, last.position.y));
			}
			else
			{
				m_snakeBody.push_back(SnakeSegment(last.position.x - 1, last.position.y));
			}
		} 

	}
	else
	{
		if (m_dir == Direction::Up)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x, last.position.y + 1));
		}
		else
		if(m_dir == Direction::Down)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x, last.position.y - 1));
		}
		else
		if(m_dir == Direction::Left)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x + 1, last.position.y));
		}
		else
		if(m_dir == Direction::Right)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x - 1, last.position.y));
		}
	}
}

void Snake::Reset()
{
	//remove all existing segments from the snake segment container
	m_snakeBody.clear();

	//add the default segments
	m_snakeBody.push_back(SnakeSegment(10, 10));
	m_snakeBody.push_back(SnakeSegment(10, 11));
	m_snakeBody.push_back(SnakeSegment(10, 12));

	//set snakes default direction
	SetDirection(Direction::None); //snake is not moving at the beginning

	//set the members to their default values
	m_speed = 5;
	m_lives = 3;
	m_score = 0;
	m_lost = false;
}

void Snake::Move()
{
	//update beginning with the tail to the head for more convenience 
	//update all segments except the head
	for (int i = m_snakeBody.size() - 1; i > 0; --i)
	{
		m_snakeBody[i].position = m_snakeBody[i - 1].position;
	}

	//update the head
	if (m_dir == Direction::Left)
	{
		--m_snakeBody[0].position.x;
	}
	else
	if (m_dir == Direction::Right)
	{
		++m_snakeBody[0].position.x;
	}
	else
	if(m_dir == Direction::Up)
	{
		--m_snakeBody[0].position.y;
	}
	else
	if(m_dir == Direction::Down)
	{
		++m_snakeBody[0].position.y;
	}
}

void Snake::Tick()
{
	if (m_snakeBody.empty()) { return; }
	if (m_dir == Direction::None) { return; }

	Move();
	CheckCollision();
}

void Snake::Cut(int segments)
{
	//remove the amount of segments stored in the param
	for (int i = 0; i < segments; ++i)
	{
		m_snakeBody.pop_back();
	}

	//remove 1 live
	--m_lives;

	//check if the player has no lives anymore
	if (!m_lives) 
	{ 
		Lose(); 
	}
}

void Snake::Render(sf::RenderWindow& window)
{
	if (m_snakeBody.empty()) { return; }

	auto head = m_snakeBody.begin();

	//head
	m_bodyRect.setFillColor(sf::Color::Yellow);
	m_bodyRect.setPosition(head->position.x * m_size, head->position.y * m_size);
	window.draw(m_bodyRect);

	//body
	m_bodyRect.setFillColor(sf::Color::Green);
	for (auto itr = m_snakeBody.begin() + 1; itr != m_snakeBody.end(); ++itr)
	{
		m_bodyRect.setPosition(itr->position.x * m_size, itr->position.y * m_size);
		window.draw(m_bodyRect);
	}
}

void Snake::CheckCollision()
{
	//its not possible for the snake to collide with itself if it has less than 5 segments
	if (m_snakeBody.size() > 5) { return; }

	SnakeSegment& head = m_snakeBody.front();

	//iterate through the snakebody and check if its colliding with the snakehead
	for (auto itr = m_snakeBody.begin() + 1; itr != m_snakeBody.end(); ++itr)
	{
		if (itr->position == head.position) //collision
		{	
			//amount of segments from the collision to the last segment of the tail
			int segments = m_snakeBody.end() - itr;

			Cut(segments); //cut off all segments from the last segment to the cut segment
			break; //there can only be one collision
		}
	}
}
