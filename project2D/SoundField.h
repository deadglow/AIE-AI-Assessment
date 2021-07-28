#pragma once
#include "Sound.h"
#include "SoundFieldNode.h"

class SoundField
{
public:
	SoundField(Sound sound);
	~SoundField();

	SoundFieldNode* GetSoundFieldNode(int i);
	void AssignFlowField(SoundFieldNode* flowField);

	Sound GetSound();
	
	//Used to add/remove agents for deletion tracking
	void RegisterAgent();
	void DeregisterAgent();

	//Returns true if there's at least one assigned agent
	bool UsedByAgents();

protected:
	Sound sound;
	SoundFieldNode* flowField = nullptr;
	int assignedAgents = 0;
};

