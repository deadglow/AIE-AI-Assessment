#include "AIManager.h"
#include "Entity.h"
#include "AIAgent.h"
#include "Pathfinder.h"
#include "Application.h"

#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourQuery.h"
#include "BehaviourAction.h"

#include "AIManagerBehaviours.h"

AIManager::AIManager(Pathfinder* pathfinder)
{
	this->pathfinder = pathfinder;

	//Generate behaviour tree
	rootBehaviour = new BehaviourSelector("Root");
	//Generate hunt
	BehaviourComposite* hunt = (BehaviourComposite*)rootBehaviour->AddChild(new BehaviourSelector("Hunt"));

	//Add attack
	BehaviourComposite* attack = (BehaviourComposite*)hunt->AddChild(new BehaviourSequence("Attack"));
	attack->AddChild(new BehaviourQuery("Received Sound?", AIManagerBehaviours::QUERY_CheckJustReceivedSound));
	attack->AddChild(new BehaviourAction("Set hunt timer", AIManagerBehaviours::ACTION_SetHuntTimer));
	attack->AddChild(new BehaviourQuery("Check sound dist", AIManagerBehaviours::QUERY_IsSoundInAttackRange));
	attack->AddChild(new BehaviourAction("Attack", AIManagerBehaviours::ACTION_Attack));

	//Add follow
	BehaviourComposite* follow = (BehaviourComposite*)hunt->AddChild(new BehaviourSequence("Follow"));
	follow->AddChild(new BehaviourQuery("Is hunt active?", AIManagerBehaviours::QUERY_CheckHuntTimer));
	
	//Add follow states
	follow = (BehaviourComposite*)follow->AddChild(new BehaviourSelector("Follow state"));
	
	//Alert state
	BehaviourComposite* followState = (BehaviourComposite*)follow->AddChild(new BehaviourSequence("Be alert"));
	followState->AddChild(new BehaviourQuery("In sound attack range?", AIManagerBehaviours::QUERY_IsSoundInAttackRange));
	followState->AddChild(new BehaviourAction("Look around", AIManagerBehaviours::ACTION_LookAround));

	//Follow state
	followState = (BehaviourComposite*)follow->AddChild(new BehaviourSequence("Follow"));
	followState->AddChild(new BehaviourQuery("Does path exist?", AIManagerBehaviours::QUERY_CheckPathExists));
	followState->AddChild(new BehaviourAction("Follow flow field", AIManagerBehaviours::ACTION_FollowFlowField));

	//Return home
	BehaviourComposite* returnHome = (BehaviourComposite*)rootBehaviour->AddChild(new BehaviourSequence("Return"));
	returnHome->AddChild(new BehaviourQuery("Too far from spawn?", AIManagerBehaviours::QUERY_CheckTooFarFromSpawn));
	returnHome->AddChild(new BehaviourAction("Follow home flowfield", AIManagerBehaviours::ACTION_FollowSpawnFlowField));

	//Add patrol
	BehaviourComposite* patrol = (BehaviourComposite*)rootBehaviour->AddChild(new BehaviourSelector("Patrol"));
	//Walk forward
	BehaviourComposite* walkForward = (BehaviourComposite*)patrol->AddChild(new BehaviourSequence("Try walk forward"));
	walkForward->AddChild(new BehaviourQuery("Wall in front?", AIManagerBehaviours::QUERY_CheckAheadFree));
	walkForward->AddChild(new BehaviourAction("Move forward", AIManagerBehaviours::ACTION_MoveForward));
	//Rotate if walk forward fails
	patrol->AddChild(new BehaviourAction("Rotate agent", AIManagerBehaviours::ACTION_Rotate));
}

AIManager::~AIManager()
{
	for (auto element : soundFields)
		delete element;

	soundFields.clear();

	delete this->rootBehaviour;
}

void AIManager::Update()
{
	for (auto agent : agents)
	{
		rootBehaviour->Execute(agent);
	}
}

void AIManager::CheckSound(Sound sound)
{
	Vector2 dist;
	for (auto element : agents)
	{
		dist = sound.position - element->GetEntity()->GetTransform()->GetGlobalPosition();

		//Only consider the sound if its within range of at least one agent
		if (dist.SqrMagnitude() < sound.radius * sound.radius)
		{
			//Create the soundfield and assign it to agents
			SoundField* soundField = CreateSoundField(sound);
			//Don't assign if the soundfield failed
			if (soundField != nullptr)
			{
				DelegateAgents(soundField);
			}
			return;
		}
	}
}

SoundField* AIManager::CreateSoundField(Sound sound)
{
	//Create flowfield to sound
	SoundFieldNode* flowfield = nullptr;

	if (pathfinder->CreateFlowField(flowfield, sound.position))
	{
		//Assign flowfield to new soundfield
		SoundField* soundField = new SoundField(sound);
		soundField->AssignFlowField(flowfield);

		//Add to soundfield list
		soundFields.push_back(soundField);

		return soundField;
	}
	else
	{
		return nullptr;
	}
}

void AIManager::CreateSpawnFlowFields()
{
	for (auto agent : agents)
	{
		//Create a flow field to the spawn point
		SoundFieldNode* field;
		if (pathfinder->CreateFlowField(field, agent->GetSpawn()))
		{
			agent->SetSpawnFlowField(field);
		}
	}
}

void AIManager::DelegateAgents(SoundField* soundField)
{
	Sound sound = soundField->GetSound();
	Vector2 agentPos;
	Vector2 nodePos;
	int addedAgents = 0;

	for (auto agent : agents)
	{
		agentPos = agent->GetEntity()->GetTransform()->GetGlobalPosition();
		//Grid based position that the agent is currently on
		nodePos = pathfinder->GetNodeCoordinatesFromPos(agentPos);
		//Only consider agent if it is within range of the sound radius
		if ((agentPos - sound.position).SqrMagnitude() < sound.radius * sound.radius)
		{
			//If the path is too far the agent will not be added
			SoundFieldNode* node = soundField->GetSoundFieldNode(pathfinder->GetIndex((int)nodePos.x, (int)nodePos.y));
			if (node->gScore < sound.loudness)
			{
				agent->SetSoundField(soundField);
				addedAgents++;
			}
		}

		//Limit number of agents added
		if (addedAgents == MAX_AGENTS_PER_SOUND)
			break;
	}

	//Since some sound fields would have been unregistered, clean the list
	CleanSoundFields();
}

void AIManager::CleanSoundFields()
{
	if (soundFields.size() > 0)
	{
		auto iter = soundFields.begin();
		SoundField* soundField;

		while (iter != soundFields.end())
		{
			soundField = *iter;
			//Remove soundfield from list if no agents are using this soundfield and clear it from memory
			if (!soundField->UsedByAgents())
			{
				iter = soundFields.erase(iter);
				delete soundField;
				//Continue without incrementing since erase will increment
				continue;
			}
			iter++;
		}
	}
}

void AIManager::AddAgent(AIAgent* agent)
{
	agents.push_back(agent);
	agent->SetManager(this);
}

int AIManager::GetSoundFieldCount()
{
	return soundFields.size();
}

SoundField* AIManager::GetNewestSoundField()
{
	return soundFields.back();
}

Pathfinder* AIManager::GetPathfinder()
{
	return pathfinder;
}