#include "Game2D.h"

#include <iostream>
#include "Application.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Scene.h"
#include "Entity.h"

float Game2D::deltaTime = 1.0f;

Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 64);

	mainScene = new Scene();
	player = mainScene->CreateEntity();
	
	Entity* newEnt = mainScene->CreateEntity();

	newEnt->GetTransform()->SetParent(player->GetTransform());
	newEnt->GetTransform()->SetLocalPosition({ 45, 90 });
	
	player->SetName("Jimmy");
	std::cout << player->GetName() << std::endl;

}

Game2D::~Game2D()
{
	// Delete scene
	delete mainScene;

	// Deleted the textures.
	delete m_font;

	// Delete the renderer.
	delete m_2dRenderer;
}

void Game2D::Update(float deltaTime)
{
	Game2D::deltaTime = deltaTime;
	// Input example: Update the camera position using the arrow keys.
	aie::Input* input = aie::Input::GetInstance();

	// Exit the application if escape is pressed.
	if (input->IsKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		aie::Application* application = aie::Application::GetInstance();
		application->Quit();
	}

	float camPosX;
	float camPosY;

	m_2dRenderer->GetCameraPos(camPosX, camPosY);

	if (input->IsKeyDown(aie::INPUT_KEY_W))
		camPosY += 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_S))
		camPosY -= 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_A))
		camPosX -= 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_D))
		camPosX += 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_UP))
		player->GetTransform()->Translate(Vector2(0, 1) * deltaTime * 100);

	if (input->IsKeyDown(aie::INPUT_KEY_DOWN))
		player->GetTransform()->Translate(Vector2(0, -1) * deltaTime * 100);

	if (input->IsKeyDown(aie::INPUT_KEY_LEFT))
		player->GetTransform()->Translate(Vector2(-1, 0) * deltaTime * 100);

	if (input->IsKeyDown(aie::INPUT_KEY_RIGHT))
		player->GetTransform()->Translate(Vector2(1, 0) * deltaTime * 100);

	mainScene->Update();
	player->GetTransform()->SetLocalRotation(player->GetTransform()->GetGlobalPosition().x / 360);
	mainScene->GetTransform()->UpdateGlobalMatrix();
	mainScene->GetTransform()->UpdateGlobalMatrix();


	m_2dRenderer->SetCameraPos(camPosX, camPosY);

}

void Game2D::Draw()
{
	aie::Application* application = aie::Application::GetInstance();
	float time = application->GetTime();


	// Wipe the screen to clear away the previous frame.
	application->ClearScreen();

	// Prepare the renderer. This must be called before any sprites are drawn.
	m_2dRenderer->Begin();

	// Draw a rotating sprite with no texture, coloured yellow.
	Vector2 pos = player->GetTransform()->GetGlobalPosition();
	m_2dRenderer->SetRenderColour(1.0f, 1.0f, 0.0f, 1.0f);
	m_2dRenderer->DrawSprite(nullptr, pos.x, pos.y, 50.0f, 50.0f, player->GetTransform()->GetGlobalRotation());

	pos = player->GetTransform()->GetChild(0)->GetGlobalPosition();
	m_2dRenderer->SetRenderColour(0.0f, 1.0f, 1.0f, 1.0f);
	m_2dRenderer->DrawSprite(nullptr, pos.x, pos.y, 20.0f, 20.0f, player->GetTransform()->GetChild(0)->GetGlobalRotation());

	m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f, 1.0f);
	
	// Draw some text.
	float windowHeight = (float)application->GetWindowHeight();
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", application->GetFPS());
	m_2dRenderer->DrawText2D(m_font, fps, 15.0f, windowHeight - 32.0f);
	m_2dRenderer->DrawText2D(m_font, "Arrow keys to move.", 15.0f, windowHeight - 64.0f);
	m_2dRenderer->DrawText2D(m_font, "WASD to move camera.", 15.0f, windowHeight - 96.0f);
	m_2dRenderer->DrawText2D(m_font, "Press ESC to quit!", 15.0f, windowHeight - 128.0f);

	// Done drawing sprites. Must be called at the end of the Draw().
	m_2dRenderer->End();
}