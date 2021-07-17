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
	return player;
}

void Player::Update()
{
	// Update input for the player.
	aie::Input* input = aie::Input::GetInstance();
	aie::Application* app = aie::Application::GetInstance();
	float deltaTime = app->GetDeltaTime();

	Transform* transform = entity->GetTransform();

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
	{
		inputVec = inputVec.Normalised();
		transform->SetUp(inputVec);
	}


	//Torso direction

	Vector2 point = { (float)input->GetMouseX(), (float)input->GetMouseY() };
	Vector2 centre = Vector2((float)app->GetWindowWidth(), (float)app->GetWindowHeight()) / 2;

	targeter->SetUp((point - centre).Normalised());

	transform->Translate(inputVec * speed * deltaTime, true);
}

void Player::SetTargeter(Transform* t)
{
	targeter = t;
}

void Player::OnCollision(Collision collision)
{
	
	//Get outta there
	entity->GetTransform()->Translate(collision.resolveVector, true);

	PhysObject* otherPhys = collision.other->GetEntity()->GetComponent<PhysObject>();

	if (otherPhys != nullptr)
	{
		otherPhys->AddForce(-collision.relativePosition.Normalised() * speed * aie::Application::GetInstance()->GetDeltaTime());
	}
}