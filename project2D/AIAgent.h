#pragma once
#include "Component.h"

class SoundField;
struct SoundFieldNode;
class AIManager;

#define HUNT_TIME 10
#define ATTACK_RANGE 50
#define LOOK_AROUND_SPEED 4
#define MAX_DIST_FROM_SPAWN 70
#define AGENT_ROTATION_DEGREES 70.0f
#define AGENT_ROTATION_SPEED 90.0f

#define AGENT_ACCELERATION 350.0f
#define AGENT_HUNT_SPEED 200.0f
#define AGENT_PATROL_SPEED 100.0f
#define AGENT_FRICTION_MULT 2.0f
#define AGENT_ATTACK_IMPULSE 500.0f

#define AGENT_HUNT_ANIMSPEED 10.0f
#define AGENT_PATROL_ANIMSPEED 3.0f

class AIAgent :
    public Component
{
public:
	
	using Component::Component;

	virtual ~AIAgent();

	AIAgent* CloneTo(Entity* ent) override;

	void Update() override;
	void OnCollision(Collision collision) override;

	SoundFieldNode* GetSpawnFlowField();
	void SetSpawn(Vector2 position);
	Vector2 GetSpawn();
	void SetSpawnFlowField(SoundFieldNode* field);


	SoundField* GetSoundField();
	//Deregister self from current soundfield then register self in new soundfield
	void SetSoundField(SoundField* field);

	AIManager* GetManager();
	void SetManager(AIManager* manager);

	//Works better than just adding force
	void Accelerate(Vector2 direction);

	//Follows the flowfield to the soundfield
	void TravelToTarget();
	//Follows the flowfield to its spawn
	void TravelToSpawn();

	//Dash
	void Attack();

	//Rotates back and forth
	void LookAround(float speed);

	void BeginHunting(float time);

	//Querys
	bool JustHeardSound();
	bool IsHunting();
	bool IsSoundInAttackRange(float range);
	bool IsSpawnInRange(int range);
	bool IsWallAhead();

	//Accelerates forwards
	void MoveForward();

	//Speeeeeen
	void Rotate(float radians);

protected:
	AIManager* manager;
	SoundFieldNode* spawnFlowField = nullptr;
	Vector2 spawn;
	SoundField* soundField = nullptr;
	Vector2 desiredDirection = Vector2::Right();
	bool justHeardSound = false;
	float huntTimer = 0;
	float speed = AGENT_PATROL_SPEED;
};

