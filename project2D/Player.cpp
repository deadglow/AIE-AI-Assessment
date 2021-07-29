#include "Player.h"
#include "Vector2.h"
#include "Application.h"
#include "Input.h"
#include "Entity.h"
#include "Sprite.h"
#include "Throwable.h"

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
	if (input->IsKeyDown(aie::INPUT_KEY_A))
	{
		inputVec.x -= 1.0f;
	}
	if (input->IsKeyDown(aie::INPUT_KEY_D))
	{
		inputVec.x += 1.0f;
	}
	if (input->IsKeyDown(aie::INPUT_KEY_W))
	{
		inputVec.y += 1.0f;
	}
	if (input->IsKeyDown(aie::INPUT_KEY_S))
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