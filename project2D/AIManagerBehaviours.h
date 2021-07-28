#pragma once
class AIAgent;

//Used by behaviour query and action function pointers
class AIManagerBehaviours
{
public:
	//BEHAVIOUR TREE BEHAVIOURS

	//Querys
	static bool QUERY_CheckJustReceivedSound(AIAgent* agent);

	static bool QUERY_IsSoundInAttackRange(AIAgent* agent);

	static bool QUERY_CheckHuntTimer(AIAgent* agent);

	static bool QUERY_CheckPathExists(AIAgent* agent);

	static bool QUERY_CheckTooFarFromSpawn(AIAgent* agent);

	static bool QUERY_CheckAheadFree(AIAgent* agent);

	//Actions
	static bool ACTION_SetHuntTimer(AIAgent* agent);

	static bool ACTION_Attack(AIAgent* agent);

	static bool ACTION_LookAround(AIAgent* agent);

	static bool ACTION_FollowFlowField(AIAgent* agent);

	static bool ACTION_FollowSpawnFlowField(AIAgent* agent);

	static bool ACTION_MoveForward(AIAgent* agent);

	static bool ACTION_Rotate(AIAgent* agent);
};

