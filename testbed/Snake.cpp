#include "Snake.h"

Snake::Snake(int blockSize, SharedContext* pSharedContext)
	:
	m_size(blockSize),
	m_spriteSheetHead(pSharedContext->m_pTextureManager),
	m_spriteSheetBody(pSharedContext->m_pTextureManager),
	m_pSharedContext(pSharedContext)
{
	m_spriteSheetHead.LoadSheet("..//..//testbed//assets//snakeSpriteSheetHead.sheet");
	m_spriteSheetBody.LoadSheet("..//..//testbed//assets//snakeSpriteSheetBody.sheet");

	m_bodyRect.setSize(sf::Vector2f(m_size-1, m_size-1)); //used to see the segments more clearly
	Reset(); //resets lives score segments and position of the snake

	//set some different colors for the body
	m_bodyColorContainer.push_back(sf::Color(0, 193, 0, 255));
	m_bodyColorContainer.push_back(sf::Color(0, 127, 0, 255));
	m_bodyColorContainer.push_back(sf::Color(0,  63, 0, 255));
	m_bodyColorContainer.push_back(sf::Color(0, 127, 0, 255));


}

Snake::~Snake()
{
}

void Snake::SetDirection(SnakeDirection dir)
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

SnakeDirection Snake::GetDirection()
{
	return m_dir;
}

SnakeDirection Snake::GetPhysicalDirection()
{
	if (m_snakeBody.size() <= 1)
	{
		return SnakeDirection::None;
	}

	SnakeSegment& head = m_snakeBody[0];
	SnakeSegment& neck = m_snakeBody[1];

	if (head.position.x == neck.position.x)
	{
		return head.position.y > neck.position.y ? SnakeDirection::Down : SnakeDirection::Up;
	}
	else
	if(head.position.y == neck.position.y)
	{
		return head.position.x > neck.position.x ? SnakeDirection::Right : SnakeDirection::Left;
	}

	return SnakeDirection::None;
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
		if (m_dir == SnakeDirection::Up)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x, last.position.y + 1));
		}
		else
		if(m_dir == SnakeDirection::Down)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x, last.position.y - 1));
		}
		else
		if(m_dir == SnakeDirection::Left)
		{
			m_snakeBody.push_back(SnakeSegment(last.position.x + 1, last.position.y));
		}
		else
		if(m_dir == SnakeDirection::Right)
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

	//set snakes defaultSnakeDirection
	SetDirection(SnakeDirection::None); //snake is not moving at the beginning

	//set the members to their default values
	m_speed = 5;
	m_lives = 3;
	m_score = 0;
	m_lost = false;

	//set the animation
	m_spriteSheetHead.SetAnimation("Up", true, true);
	m_spriteSheetBody.SetAnimation("Idle", true, true);
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
	if (m_dir == SnakeDirection::Left)
	{
		--m_snakeBody[0].position.x;
	}
	else
	if (m_dir == SnakeDirection::Right)
	{
		++m_snakeBody[0].position.x;
	}
	else
	if(m_dir == SnakeDirection::Up)
	{
		--m_snakeBody[0].position.y;
	}
	else
	if(m_dir == SnakeDirection::Down)
	{
		++m_snakeBody[0].position.y;
	}
}

void Snake::Tick()
{
	if (m_snakeBody.empty()) { return; }
	if (m_dir ==SnakeDirection::None) { return; }

	Move();
	CheckCollision();
}

void Snake::UpdateAnimation(const float& dT)
{
	m_spriteSheetHead.Update(dT);
	m_spriteSheetBody.Update(dT);
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

	////body - using simple shapes
	//m_bodyRect.setFillColor(sf::Color::Green);
	//int i = 0;
	//for (auto itr = m_snakeBody.begin() + 1; itr != m_snakeBody.end(); ++itr)
	//{
	//	//set the color
	//	int index = i++ % m_bodyColorContainer.size();
	//	m_bodyRect.setFillColor(m_bodyColorContainer[index]);

	//	m_bodyRect.setPosition(itr->position.x * m_size, itr->position.y * m_size);
	//	window.draw(m_bodyRect);
	//}

	for (auto itr = m_snakeBody.begin() + 1; itr != m_snakeBody.end(); ++itr)
	{

		m_spriteSheetBody.SetSpritePosition(sf::Vector2f(itr->position.x * m_size + m_size / 2.0f, itr->position.y * m_size + m_size / 2.0f));
		m_spriteSheetBody.Draw(&m_pSharedContext->m_pWindow->GetRenderWindow());
	}

	//head - using simple shapes
	//auto head = m_snakeBody.begin();
	//m_bodyRect.setFillColor(sf::Color::Yellow);
	//m_bodyRect.setPosition(head->position.x * m_size, head->position.y * m_size);
	//window.draw(m_bodyRect);

	//head - using spritesheet animations
	sf::Vector2f position(m_snakeBody[0].position.x * m_size + m_size / 2.0f, m_snakeBody[0].position.y * m_size + m_size / 2.0f); //calculate upper left corner
	m_spriteSheetHead.SetSpritePosition(position);
	m_spriteSheetHead.Draw(&m_pSharedContext->m_pWindow->GetRenderWindow());

}

void Snake::Navigate(EventDetails* details)
{
	switch (details->m_keyCode)
	{
	case sf::Keyboard::Up:
		if (GetPhysicalDirection() != SnakeDirection::Down)
		{
			SetDirection(SnakeDirection::Up);
			m_spriteSheetHead.SetAnimation("Up", true, true);
		}
		break;
	case sf::Keyboard::Down:
		if (GetPhysicalDirection() != SnakeDirection::Up)
		{
			SetDirection(SnakeDirection::Down);
			m_spriteSheetHead.SetAnimation("Down", true, true);
		}
		break;
	case sf::Keyboard::Left:
		if (GetPhysicalDirection() != SnakeDirection::Right)
		{
			SetDirection(SnakeDirection::Left);
			m_spriteSheetHead.SetAnimation("Left", true, true);
		}
		break;
	case sf::Keyboard::Right:
		if (GetPhysicalDirection() != SnakeDirection::Left)
		{
			SetDirection(SnakeDirection::Right);
			m_spriteSheetHead.SetAnimation("Right", true, true);
		}
		break;
	}
}

void Snake::CheckCollision()
{
	//its not possible for the snake to collide with itself if it has less than 5 segments
	if (m_snakeBody.size() < 5) { return; }

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
