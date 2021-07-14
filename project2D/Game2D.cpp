#include "Game2D.h"

#include <string>
#include <iostream>
#include <filesystem>
#include "Application.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Sprite.h"
#include "Player.h"

aie::Renderer2D* Game2D::GetRenderer()
{
	return m_2dRenderer;
}

Scene* Game2D::GetMainScene()
{
	return mainScene;
}

CollisionManager* Game2D::GetCollisionManager()
{
	return collisionManager;
}

void Game2D::LoadTextures()
{
	std::string path = "../bin/textures/";

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		textures.insert(std::make_pair(entry.path().filename().string(), new aie::Texture(entry.path().string().c_str())));
	}
}

aie::Texture* Game2D::GetTexture(std::string name)
{
	return textures[name];
}

void Game2D::AddSpriteDrawCall(Sprite* sprite)
{
	sprites.push(sprite);
}


Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	m_2dRenderer = new aie::Renderer2D();

	collisionManager = new CollisionManager();

	LoadTextures();

	m_font = new aie::Font("./font/consolas.ttf", 30);

	mainScene = new Scene(this);

	//Player
	player = mainScene->CreateEntity();
	aie::Texture* tex = GetTexture("block.png");
	player->AddComponent<Player>();
	player->AddComponent<Sprite>();
	player->GetComponent<Sprite>()->SetTexture(tex);
	player->AddComponent<ColliderBox>();
	player->GetComponent<ColliderBox>()->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	player->GetTransform()->Rotate(1.3f);

	////Create upper body
	Entity* newEnt = mainScene->CreateEntity(player->GetTransform());
	newEnt->AddComponent<Sprite>();
	newEnt->GetComponent<Sprite>()->SetTexture(GetTexture("torso.png"));
	newEnt->GetComponent<Sprite>()->SetDepth(-1.0f);
	player->GetComponent<Player>()->SetTargeter(newEnt->GetTransform());

	////Create wall
	Entity* newWall = mainScene->CreateEntity();
	newWall->AddComponent<Sprite>();
	tex = GetTexture("grass.png");
	newWall->GetComponent<Sprite>()->SetTexture(tex);
	newWall->AddComponent<ColliderBox>();
	newWall->GetComponent<ColliderBox>()->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	newWall->GetTransform()->SetLocalPosition(Vector2( 500.0f, 500.0f ));
}

Game2D::~Game2D()
{
	// Delete scene
	delete mainScene;

	delete collisionManager;

	// Deleted the textures.
	delete m_font;
	
	for (auto& element : textures)
	{
		delete element.second;
		element.second = nullptr;
	}

	// Delete the renderer.
	delete m_2dRenderer;
}

void Game2D::Update(float deltaTime)
{
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

	if (input->WasKeyPressed(aie::INPUT_KEY_C))
	{
		Entity* clone = player->Clone();
		clone->GetTransform()->Translate(-Vector2::One() * 10.0f, false);
	}

	if (input->WasKeyPressed(aie::INPUT_KEY_B))
		drawColliders = !drawColliders;

	mainScene->Update();
	mainScene->GetTransform()->UpdateGlobalMatrix();
	collisionManager->CheckCollisions();
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
	
	//Draw all sprites in queue
	while (!sprites.empty())
	{
		sprites.front()->Draw();
		sprites.pop();
	}

	if (drawColliders)
	{
		m_2dRenderer->SetRenderColour(0.0f, 1.0f, 0.0f);
		collisionManager->DrawColliders(m_2dRenderer);
	}
	
	// Draw some text.
	m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f, 1.0f);
	float windowHeight = (float)application->GetWindowHeight();
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", application->GetFPS());
	m_2dRenderer->DrawText2D(m_font, fps, 15.0f, windowHeight - 32.0f);

	// Done drawing sprites. Must be called at the end of the Draw().
	m_2dRenderer->End();
}