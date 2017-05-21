#include "Animation_Directional.h"
#include "SpriteSheet.h"

void Animation_Directional::FrameStep()
{
	if (m_frameStart < m_frameEnd) 
	{
		++m_frameCurrent; //forwards animation
	}
	else
	{
		--m_frameCurrent; //backwards animation
	}

	if ((m_frameStart < m_frameEnd && m_frameCurrent > m_frameEnd) || //forwards animation leaving animation range
		(m_frameStart > m_frameEnd && m_frameCurrent < m_frameEnd))   //backwards animation
	{
		if (m_loop)	
		{
			m_frameCurrent = m_frameStart; //reset the animation to its start when its looping
			return;
		}

		//animation has come to an end
		Pause();
	}
}

void Animation_Directional::CropSprite()
{
	//locate the sprite representing the current frame of the animation in the spritesheet by an intrect
	sf::IntRect rect(m_pSpriteSheet->GetSpriteSize().x * m_frameCurrent,										//left
					 m_pSpriteSheet->GetSpriteSize().y * (m_frameRow + (short) m_pSpriteSheet->GetDirection()),	//top
					 m_pSpriteSheet->GetSpriteSize().x,															//width
					 m_pSpriteSheet->GetSpriteSize().y);														//height

	m_pSpriteSheet->CropSprite(rect);
}

void Animation_Directional::ReadIn(std::stringstream& stream)
{
	stream >> m_frameStart >> m_frameEnd >> m_frameRow >> m_frameTime >> m_frameActionStart >> m_frameActionEnd;
}
