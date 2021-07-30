#include "Player.h"
#include "Vector2.h"
#include "Application.h"
#include "Input.h"
#include "Entity.h"
#include "Sprite.h"
#include "Throwable.h"
#include "AIManager.h"

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

	//Get input
	Vector2 inputVec = { 0, 0 };
	if (input->IsKeyDown(aie::INPUT_KEY_A))
		inputVec.x -= 1.0f;
	if (input->IsKeyDown(aie::INPUT_KEY_D))
		inputVec.x += 1.0f;
	if (input->IsKeyDown(aie::INPUT_KEY_W))
		inputVec.y += 1.0f;
	if (input->IsKeyDown(aie::INPUT_KEY_S))
		inputVec.y -= 1.0f;

	//Prevents dividing by 0
	if (inputVec.SqrMagnitude() > 0)
	{
		//limit input vec to a unit circle
		inputVec = inputVec.Normalised();
		transform->SetUp(inputVec);
		//Footstep noise
		Sound sound = Sound(entity->GetTransform()->GetGlobalPosition(), FOOTSTEP_LOUDNESS, FOOTSTEP_RADIUS);
		entity->GetGameData()->GetAIManager()->CheckSound(sound);
	}

	//Torso direction
	Vector2 point = entity->GetGameData()->GetWorldMousePos();
	Vector2 centre = entity->GetTransform()->GetGlobalPosition();

	targeter->SetUp((point - centre).Normalised());

	if (heldThrowable != nullptr)
	{
		targeter->GetEntity()->GetComponent<Sprite>()->SetAnimationProgress(0.5f);
		//Throwing and dropping
		if (input->WasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
		{
			heldThrowable->Throw(targeter->GetRight() * THROW_VELOCITY, THROW_SPIN);
			heldThrowable = nullptr;
		}
		else if (input->WasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT))
		{
			heldThrowable->Drop();
			heldThrowable = nullptr;
		}
	}
	else
	{
		targeter->GetEntity()->GetComponent<Sprite>()->SetAnimationProgress(0.0f);
		//Try pick up
		if (input->WasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT))
		{
			Game2D* gameData = entity->GetGameData();
			//Loop through all throwables and assign the first one that is in range to heldThrowable;
			for (int i = 0; i < gameData->GetThrowableCount(); ++i)
			{
				Throwable* throwable = gameData->GetThrowable(i);
				if ((throwable->GetEntity()->GetTransform()->GetGlobalPosition() - entity->GetTransform()->GetGlobalPosition()).SqrMagnitude() < PICKUP_DISTANCE * PICKUP_DISTANCE)
				{
					heldThrowable = throwable;
					heldThrowable->PickUp(targeter, Vector2::Right() * 40.0f);
					break;
				}
			}
		}
	}

	transform->Translate(inputVec * speed * deltaTime, true);
}

void Player::SetTargeter(Transform* t)
{
	targeter = t;
}

bool Player::IsAlive()
{
	return playerAlive;
}

void Player::KillPlayer()
{
	playerAlive = false;
}

void Player::OnCollision(Collision collision)
{
	//Get outta there
	entity->GetTransform()->Translate(collision.resolveVector, true);

	//Push physobjects
	PhysObject* otherPhys = collision.other->GetEntity()->GetComponent<PhysObject>();
	if (otherPhys != nullptr)
	{
		otherPhys->AddForce(-collision.relativePosition.Normalised() * speed * aie::Application::GetInstance()->GetDeltaTime());
	}
}