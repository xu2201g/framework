#pragma once

#include <sstream>

class SpriteSheet;

using Frame = unsigned int;

class Animation_Base
{
	friend class SpriteSheet; 

public:

	Animation_Base();
	virtual ~Animation_Base();

	void SetSpriteSheet(SpriteSheet* sheet);
	void SetFrame(const unsigned int& frame);
	void SetName(const std::string& name);
	
	const std::string& GetName() { return m_name; }

	bool IsInAction();
	bool IsPlaying();
	
	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(const float& dT);

	friend std::stringstream& operator>>(std::stringstream& stream, Animation_Base& animation_base)
	{
		animation_base.ReadIn(stream);
		return stream;		
	}

protected:

	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& stream) = 0;

	void SetLooping(const bool& loop);

	Frame m_frameCurrent;
	Frame m_frameStart;
	Frame m_frameEnd;
	Frame m_frameRow;

	int m_frameActionStart;
	int m_frameActionEnd;

	float m_frameTime;
	float m_elapsedTime;
	
	bool m_loop;
	bool m_playing;

	std::string m_name;

	SpriteSheet* m_pSpriteSheet; 
};