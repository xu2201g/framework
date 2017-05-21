#pragma once

#include "Animation_Base.h"

class Animation_Directional
	:
	public Animation_Base
{
protected:

	void FrameStep();
	void CropSprite();
	void ReadIn(std::stringstream& stream);
};