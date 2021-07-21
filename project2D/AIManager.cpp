#include "AIManager.h"
#include "Entity.h"
#include "AIAgent.h"
#include "Pathfinder.h"

AIManager::AIManager(Pathfinder* pathfinder)
{
	this->pathfinder = pathfinder;
}

AIManager::~AIManager()
{
	for (auto element : soundFields)
		delete element;
}

void AIManager::CheckSound(Sound sound)
{
	Vector2 dist;
	for (auto element : agents)
	{
		dist = sound.position - element->GetEntity()->GetTransform()->GetGlobalPosition();

		if (dist.SqrMagnitude() < sound.radius)
		{
			CreateSoundField(sound);
			return;
		}
	}
}

void AIManager::CreateSoundField(Sound sound)
{
	SoundFieldNode* flowfield;

	pathfinder->CreateFlowField(flowfield, sound.position);

	SoundField* soundField = new SoundField(sound);
	soundField->AssignFlowField(flowfield);

	soundFields.push_back(soundField);
}
