#include "Player.h"
#include "Vector2.h"
#include "Application.h"
#include "Input.h"
#include "Entity.h"
#include "Sprite.h"

Player::~Player()
{

}

Player* Player::CloneTo(Entity* ent)
{
	Player* player = ent->AddComponent<Player>();
	player->speed = this->speed;
	player->targeter = player->GetEntity()->GetTransform()->GetChild(0);
	return player;
}

void Player::Update()
{
	// Update input for the player.
	aie::Input* input = aie::Input::GetInstance();
	float deltaTime = aie::Application::GetInstance()->GetDeltaTime();

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
	transform->Rotate(90.0f * DEG2RAD * deltaTime);
	transform->SetLocalScale(transform->GetLocalScale() + Vector2::One() * inputVec.y * deltaTime);
	
}

void Player::SetTargeter(Transform* t)
{
	targeter = t;
}