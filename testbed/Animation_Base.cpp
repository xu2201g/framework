#include "Animation_Base.h"

Animation_Base::Animation_Base()
	:
	m_frameCurrent(0),
	m_frameStart(0),
	m_frameEnd(0),
	m_frameRow(0),
	m_frameTime(0.0f),
	m_elapsedTime(0.0f),
	m_frameActionStart(-1),
	m_frameActionEnd(-1),
	m_loop(false),
	m_playing(false)
{}

Animation_Base::~Animation_Base()
{}

void Animation_Base::SetSpriteSheet(SpriteSheet* sheet)
{
	m_pSpriteSheet = sheet;
}

void Animation_Base::SetFrame(const unsigned int& frame)
{
	if ((frame >= m_frameStart && frame <= m_frameEnd) || //frame is within correct range - playing forward
		(frame >= m_frameEnd && frame <= m_frameStart))   //necessary to support backwards playing
	{
		m_frameCurrent = frame;
	}
}

void Animation_Base::SetName(const std::string& name)
{
	m_name = name;
}

bool Animation_Base::IsInAction()
{
	if (m_frameActionStart == -1 || m_frameActionEnd == -1) //the action is always performed - like idle breathing animation of a character or something like that 
	{
		return true;
	}

	return m_frameCurrent >= m_frameActionStart && m_frameCurrent <= m_frameActionEnd; //within action range?
}

bool Animation_Base::IsPlaying()
{
	return m_playing;
}

void Animation_Base::Play()
{
	m_playing = true;
}

void Animation_Base::Pause()
{
	m_playing = false;
}

void Animation_Base::Stop()
{
	Pause();
	Reset();
}

void Animation_Base::Reset()
{
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.0f;

	CropSprite();
}

void Animation_Base::Update(const float& dT)
{
	if (!m_playing) { return; } //nothing to change 

	m_elapsedTime += dT;
	if (m_elapsedTime < m_frameTime) { return; } //to early to swap to the next frame

	FrameStep();
	CropSprite();

	m_elapsedTime = 0;
}

void Animation_Base::SetLooping(const bool& loop)
{
	m_loop = loop;
}
