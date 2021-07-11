#include "Player.h"
#include "Vector2.h"
#include "Application.h"
#include "Input.h"
#include "Entity.h"
#include "Sprite.h"

Player::Player(Entity* ent) : Component::Component(ent)
{
	//Create upper body
	Entity* newEnt = entity->CreateEntity(entity->GetTransform());
	newEnt->AddComponent<Sprite>();
	newEnt->GetComponent<Sprite>()->SetTexture(entity->GetGameData()->GetTexture("torso.png"));

	newEnt->GetComponent<Sprite>()->SetDepth(-1.0f);

	targeter = newEnt->GetTransform();

}

Player::~Player()
{

}

Player* Player::CloneTo(Entity* ent)
{
	Player* newPlayer = ent->AddComponent<Player>();
	//do this
	return newPlayer;
}

void Player::Update()
{
	// Update input for the player.
	aie::Input* input = aie::Input::GetInstance();

	float deltaTime = entity->GetGameData()->GetDeltaTime();

	Vector2 inputVec = { 0, 0 };
	if (input->IsKeyDown(aie::INPUT_KEY_LEFT))
	{
		inputVec.x -= 1.0f;
	}
	if (input->IsKeyDown(aie::INPUT_KEY_RIGHT))
	{
		inputVec.x += 1.0f;
	}
	if (input->IsKeyDown(aie::INPUT_KEY_UP))
	{
		inputVec.y += 1.0f;
	}
	if (input->IsKeyDown(aie::INPUT_KEY_DOWN))
	{
		inputVec.y -= 1.0f;
	}

	if (inputVec.SqrMagnitude() > 0)
		inputVec = inputVec.Normalised();

	Transform* transform = entity->GetTransform();

	Vector2 point = { (float)input->GetMouseX(), (float)input->GetMouseY() };

	targeter->LookAt(point);

	transform->Translate(inputVec * speed * deltaTime, true);
	
}