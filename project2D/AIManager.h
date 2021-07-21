#pragma once
#include <list>
#include <vector>
#include "SoundField.h"

class Entity;
class AIAgent;
class Pathfinder;

class AIManager
{
public:
	AIManager(Pathfinder* pathfinder);
	~AIManager();

	void CheckSound(Sound sound);
	void CreateSoundField(Sound sound);

protected:
	Pathfinder* pathfinder;
	std::list<SoundField*> soundFields;
	std::vector<AIAgent*> agents;

};

