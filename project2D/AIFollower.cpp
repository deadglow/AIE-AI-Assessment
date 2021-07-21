#include "AIFollower.h"
#include "Entity.h"
#include "Application.h"

AIFollower::~AIFollower()
{
}

AIFollower* AIFollower::CloneTo(Entity* ent)
{
	AIFollower* ai = ent->AddComponent<AIFollower>();
	ai->acceleration = this->acceleration;
	ai->direction = this->direction;
	return ai;
}

void AIFollower::Update()
{
	if (entity->GetGameData()->flowField != nullptr)
	{
		PathfinderNode* node = entity->GetGameData()->GetPathfinder()->GetNodeFromPos(entity->GetTransform()->GetGlobalPosition());
		int index = entity->GetGameData()->GetPathfinder()->GetIndex(node->x, node->y);
		direction = entity->GetGameData()->flowField[index].direction;
	}
	entity->GetComponent<PhysObject>()->AddForce(direction * acceleration * aie::Application::GetInstance()->GetDeltaTime());
}