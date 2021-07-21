#include "SoundField.h"

SoundField::SoundField(Sound sound)
{
	this->sound = sound;
}

SoundField::~SoundField()
{
	if (flowField != nullptr)
		delete[] flowField;
	flowField = nullptr;
}

SoundFieldNode SoundField::GetSoundFieldNode(int i)
{
	return flowField[i];
}

void SoundField::AssignFlowField(SoundFieldNode* flowField)
{
	this->flowField = flowField;
}

Sound SoundField::GetSound()
{
	return sound;
}
