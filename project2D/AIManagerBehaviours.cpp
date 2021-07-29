#include "AIManagerBehaviours.h"
#include "AIAgent.h"
#include "Vector2.h"

using namespace MathMan;

//Querys

bool AIManagerBehaviours::QUERY_CheckJustReceivedSound(AIAgent* agent)
{
	return agent->JustHeardSound();
}

bool AIManagerBehaviours::QUERY_IsSoundInAttackRange(AIAgent* agent)
{
	return agent->IsSoundInAttackRange(ATTACK_RANGE);
}

bool AIManagerBehaviours::QUERY_CheckHuntTimer(AIAgent* agent)
{
	return agent->IsHunting();
}

bool AIManagerBehaviours::QUERY_CheckPathExists(AIAgent* agent)
{
	return agent->GetSoundField() != nullptr;
}

bool AIManagerBehaviours::QUERY_CheckTooFarFromSpawn(AIAgent* agent)
{
	return !agent->IsSpawnInRange(MAX_DIST_FROM_SPAWN);
}

bool AIManagerBehaviours::QUERY_CheckAheadFree(AIAgent* agent)
{
	return !agent->IsWallAhead();
}

//Actions

bool AIManagerBehaviours::ACTION_SetHuntTimer(AIAgent* agent)
{
	agent->BeginHunting(HUNT_TIME);
	return true;	
}

bool AIManagerBehaviours::ACTION_Attack(AIAgent* agent)
{
	agent->Attack();
	return true;
}

bool AIManagerBehaviours::ACTION_LookAround(AIAgent* agent)
{
	agent->LookAround(LOOK_AROUND_SPEED);
	return true;
}

bool AIManagerBehaviours::ACTION_FollowFlowField(AIAgent* agent)
{
	agent->TravelToTarget();
	return true;
}

bool AIManagerBehaviours::ACTION_FollowSpawnFlowField(AIAgent* agent)
{
	agent->TravelToSpawn();
	return true;
}

bool AIManagerBehaviours::ACTION_MoveForward(AIAgent* agent)
{
	agent->MoveForward();
	return true;
}

bool AIManagerBehaviours::ACTION_Rotate(AIAgent* agent)
{
	agent->Rotate(AGENT_ROTATION_DEGREES * DEG2RAD);
	return true;
}
