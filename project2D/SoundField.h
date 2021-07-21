#pragma once
#include "Sound.h"
#include "SoundFieldNode.h"

class SoundField
{
public:
	SoundField(Sound sound);
	~SoundField();

	SoundFieldNode GetSoundFieldNode(int i);
	void AssignFlowField(SoundFieldNode* flowField);

	Sound GetSound();

protected:
	Sound sound;
	SoundFieldNode* flowField = nullptr;
};

