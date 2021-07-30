#include "AIAgent.h"
#include "Entity.h"
#include "AIManager.h"
#include "Application.h"
#include "SoundField.h"
#include <cmath>

AIAgent::~AIAgent()
{
	if (spawnFlowField != nullptr)
		delete[] spawnFlowField;
}

AIAgent* AIAgent::CloneTo(Entity* ent)
{
	AIAgent* agent = ent->AddComponent<AIAgent>();

	//Dupe values
	agent->manager = this->manager;
	agent->spawnFlowField = this->spawnFlowField;
	agent->spawn = this->spawn;
	agent->soundField = this->soundField;
	agent->huntTimer = this->huntTimer;
	
	return agent;
}

void AIAgent::Update()
{
	float deltaTime = aie::Application::GetInstance()->GetDeltaTime();
	if (huntTimer > 0)
	{
		huntTimer -= deltaTime;
		entity->GetComponent<Sprite>()->SetFrameRate(AGENT_HUNT_ANIMSPEED);
		speed = AGENT_HUNT_SPEED;
	}
	else
	{
		entity->GetComponent<Sprite>()->SetFrameRate(AGENT_PATROL_ANIMSPEED);
		speed = AGENT_PATROL_SPEED;
	}

	if (desiredDirection.SqrMagnitude() > 0)
	{
		float angle = Vector2::SignedAngle(entity->GetTransform()->GetRight(), desiredDirection);

		if (!std::isnan(angle))
		{
			float frameRotation = sign(angle) * AGENT_ROTATION_SPEED * deltaTime;

			entity->GetTransform()->Rotate(std::min(angle, frameRotation));
		}
	}
}

void AIAgent::OnCollision(Collision collision)
{
	Player* player = collision.other->GetEntity()->GetComponent<Player>();

	if (player != nullptr)
	{
		player->KillPlayer();
	}
}

SoundFieldNode* AIAgent::GetSpawnFlowField()
{
	return spawnFlowField;
}

void AIAgent::SetSpawnFlowField(SoundFieldNode* field)
{
	this->spawnFlowField = field;
}

void AIAgent::SetSpawn(Vector2 position)
{
	this->spawn = position;
}

Vector2 AIAgent::GetSpawn()
{
	return spawn;
}

SoundField* AIAgent::GetSoundField()
{
	return soundField;
}

void AIAgent::SetSoundField(SoundField* field)
{
	if (soundField != nullptr)
		soundField->DeregisterAgent();

	soundField = field;

	if (soundField != nullptr)
	{
		soundField->RegisterAgent();
		justHeardSound = true;
	}
}

AIManager* AIAgent::GetManager()
{
	return manager;
}

void AIAgent::SetManager(AIManager* manager)
{
	this->manager = manager;
}

void AIAgent::Accelerate(Vector2 direction)
{
	PhysObject* phys = entity->GetComponent<PhysObject>();
	
	Vector2 accelVec = (direction * speed) - phys->GetVelocity();
	float remainSpeed = accelVec.Magnitude();

	if (remainSpeed < 0.00001f)
		return;
	//Normalise
	accelVec /= remainSpeed;

	float potentialAccel = AGENT_ACCELERATION * aie::Application::GetInstance()->GetDeltaTime();

	if (Vector2::Dot(direction, phys->GetVelocity()) <= 0)
		potentialAccel *= AGENT_FRICTION_MULT;

	//Convert to acceleration this frame
	accelVec *= std::min(potentialAccel, remainSpeed);

	phys->AddForce(accelVec);
}

void AIAgent::TravelToTarget()
{
	//Get node pos from world pos
	Vector2 node = manager->GetPathfinder()->GetNodeCoordinatesFromPos(entity->GetTransform()->GetGlobalPosition());
	
	//Get the direction of that node
	int index = manager->GetPathfinder()->GetIndex((int)node.x, (int)node.y);
	Vector2 direction = soundField->GetSoundFieldNode(index)->direction;
	desiredDirection = direction;

	Accelerate(direction);
}

void AIAgent::TravelToSpawn()
{
	//Get node pos from world pos
	Vector2 node = manager->GetPathfinder()->GetNodeCoordinatesFromPos(entity->GetTransform()->GetGlobalPosition());

	//Get the direction of that node
	Vector2 direction = spawnFlowField[manager->GetPathfinder()->GetIndex((int)node.x, (int)node.y)].direction;
	desiredDirection = direction;

	Accelerate(direction);
}

void AIAgent::Attack()
{
	//Jump towards sound source
	Vector2 targetVector = soundField->GetSound().position - entity->GetTransform()->GetGlobalPosition();
	targetVector = targetVector.Normalised();
	
	desiredDirection = targetVector;
	//entity->GetTransform()->SetRight(targetVector);

	entity->GetComponent<PhysObject>()->SetVelocity(targetVector * AGENT_ATTACK_IMPULSE);

	//Slow mo for dramatic effect
	entity->GetGameData()->StartSlowMotion(0.2f, 1.0f);
}

void AIAgent::LookAround(float speed)
{
	Transform* transform = entity->GetTransform();
	Vector2 offset = soundField->GetSound().position - transform->GetGlobalPosition();
	
	//Sway left and right over time
	if (offset.SqrMagnitude() > 0.00001f)
	{
		offset = offset.Normalised();
		Vector2 rangle = offset.RightAngle();
		if (std::sinf(huntTimer * LOOK_AROUND_SPEED) < 0)
			rangle = -rangle;
		desiredDirection = rangle;
	}

}

void AIAgent::BeginHunting(float time)
{
	huntTimer = time;
	justHeardSound = false;
}

bool AIAgent::JustHeardSound()
{
	return justHeardSound;
}

bool AIAgent::IsHunting()
{
	return huntTimer > 0;
}

bool AIAgent::IsSoundInAttackRange(float range)
{
	float sqrDist = (GetEntity()->GetTransform()->GetGlobalPosition() - soundField->GetSound().position).SqrMagnitude();

	return sqrDist <= range * range;
}

bool AIAgent::IsSpawnInRange(int range)
{
	//Get node pos from world pos
	Vector2 node = manager->GetPathfinder()->GetNodeCoordinatesFromPos(entity->GetTransform()->GetGlobalPosition());

	return spawnFlowField[manager->GetPathfinder()->GetIndex((int)node.x, (int)node.y)].gScore < range;
}

bool AIAgent::IsWallAhead()
{
	//Get node pos from world pos
	Vector2 ahead = entity->GetTransform()->GetGlobalPosition() + (entity->GetTransform()->GetRight() * CELL_SIZE);
	return (manager->GetPathfinder()->GetNodeFromPos(ahead)->blocked);
}

void AIAgent::MoveForward()
{
	//Accelerate in the forward direction
	Accelerate(entity->GetTransform()->GetRight());
}

void AIAgent::Rotate(float radians)
{
	entity->GetTransform()->Rotate(radians);
	desiredDirection = entity->GetTransform()->GetLocalTransform().GetRight();
}
