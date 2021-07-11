#include "Player.h"
#include "Vector2.h"
#include "Entity.h"
#include "Input.h"
#include "Application.h"

Player::~Player()
{

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

	Transform* transform = entity->GetTransform();
	transform->Translate(entity->GetTransform()->GetUp() * inputVec.y * speed * deltaTime);
	transform->Rotate(rotationSpeed * deltaTime * -inputVec.x * DEG2RAD);

	aie::Application* app = aie::Application::GetInstance();
	
	float scale = minScale + sin(app->GetTime() * scaleFrequency) * maxScaleOffset;
	transform->SetLocalScale(Vector2::One() * scale);
}

//void Player::Draw(aie::Renderer2D* renderer)
//{
//	// Draw the player's sprite.
//	renderer->DrawSprite(m_texture, m_posX, m_posY);
//}