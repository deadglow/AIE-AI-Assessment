#pragma once
#include <list>
#include <vector>
#include "SoundField.h"

#define MAX_AGENTS_PER_SOUND 3

class Entity;
class AIAgent;
class Pathfinder;
class BehaviourComposite;
class SoundField;

class AIManager
{
public:
	AIManager(Pathfinder* pathfinder);
	~AIManager();

	//Calls behaviours on all agents
	void Update();

	//Verifies sound is valid
	void CheckSound(Sound sound);

	//Generates all spawn flow fields
	void CreateSpawnFlowFields();

	//Delegates agents to the soundfield
	void DelegateAgents(SoundField* soundField);

	void AddAgent(AIAgent* agent);

	int GetSoundFieldCount();
	SoundField* GetNewestSoundField();

	Pathfinder* GetPathfinder();

protected:
	//Has a flow field and a sound
	SoundField* CreateSoundField(Sound sound);
	//Clear all unused soundfields
	void CleanSoundFields();

	Pathfinder* pathfinder;
	std::list<SoundField*> soundFields;
	std::vector<AIAgent*> agents;
	BehaviourComposite* rootBehaviour = nullptr;
};

