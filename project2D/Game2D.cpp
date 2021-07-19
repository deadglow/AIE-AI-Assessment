#include "Game2D.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <conio.h>
#include "Application.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Scene.h"
#include "CollisionManager.h"
#include "ComponentIncludes.h"

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

void Game2D::LoadAnimations()
{
	std::string path = "../bin/anims/";

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::ifstream animFile(entry.path().string().c_str());

		std::string line;

		Animation* animation = new Animation();
		animation->SetName(entry.path().filename().string());
		animations.insert(std::make_pair(animation->GetName(), animation));

		while (std::getline(animFile, line))
		{
			animation->AddFrame(GetTexture(line));
		}
	}
}

Animation* Game2D::GetAnimation(std::string name)
{
	return animations[name];
}

void Game2D::CreateSceneFromMap()
{
	int data;
	float top = room->GetHeight() * CELL_SIZE;
	for (int y = 0; y < room->GetHeight(); ++y)
	{
		for (int x = 0; x < room->GetWidth(); ++x)
		{
			data = room->GetData(x, y);
			switch (data)
			{
			case 1:
				{
					Entity* wallClone = wall->Clone();
					wallClone->GetTransform()->SetLocalPosition({ (float)x * CELL_SIZE, top - (float)y * CELL_SIZE });
				}
				break;
			default:
				break;
			}
		}
	}
}

void Game2D::AddSpriteDrawCall(Sprite* sprite)
{
	spriteDrawCalls.push(sprite);
}


Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	m_2dRenderer = new aie::Renderer2D();

	aie::Application::GetInstance()->SetVSync(false);

	m_font = new aie::Font("../bin/font/consolas.ttf", 24);
	LoadTextures();
	LoadAnimations();
	room = new MushRoom("../bin/maps/exports/map.mushroom");

	collisionManager = new CollisionManager();

	mainScene = new Scene(this);

	////Create wall
	wall = mainScene->CreateEntity();
	wall->AddComponent<Sprite>();
	aie::Texture* tex = GetTexture("wall.png");
	wall->GetComponent<Sprite>()->SetAnimation(GetAnimation("block.anim"));
	wall->GetComponent<Sprite>()->SetFrameRate(5.0f);
	wall->AddComponent<ColliderBox>();
	wall->GetComponent<ColliderBox>()->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	wall->GetComponent<ColliderBox>()->SetStatic(true);
	wall->GetTransform()->SetLocalPosition(Vector2( 10000.0f, 10000.0f ));

	CreateSceneFromMap();


	//Player
	player = mainScene->CreateEntity();
	tex = GetTexture("legs.png");
	player->AddComponent<Player>();
	player->AddComponent<Sprite>();
	player->GetComponent<Sprite>()->SetAnimation(GetAnimation("block.anim"));
	player->AddComponent<ColliderBox>();
	player->GetComponent<ColliderBox>()->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	player->GetTransform()->Rotate(1.3f);

	player->GetTransform()->SetLocalPosition({ 150.0f, 150.0f });

	////Create upper body
	Entity* newEnt = mainScene->CreateEntity(player->GetTransform());
	newEnt->AddComponent<Sprite>();
	newEnt->GetComponent<Sprite>()->SetAnimation(GetAnimation("torso.anim"));
	newEnt->GetComponent<Sprite>()->SetDepth(-1.0f);
	player->GetComponent<Player>()->SetTargeter(newEnt->GetTransform());


	////Create thing
	//Entity* newWall = mainScene->CreateEntity();
	//newWall->AddComponent<Sprite>();
	//tex = GetTexture("block.png");
	//newWall->GetComponent<Sprite>()->SetAnimation(GetAnimation("block.anim"));
	//PhysObject* phys = newWall->AddComponent<PhysObject>();
	//newWall->AddComponent<ColliderBox>();
	//newWall->GetComponent<ColliderBox>()->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	//newWall->GetComponent<ColliderBox>()->SetRestitution(0.0f);
	//
	//newWall->GetTransform()->SetLocalPosition(Vector2(200.0f, 200.0f));

	//newWall->Clone();
	//newWall->Clone();
	//newWall->Clone();

}

Game2D::~Game2D()
{
	// Delete scene
	delete mainScene;

	delete collisionManager;

	delete room;

	// Deleted the textures.
	delete m_font;
	
	for (auto& element : textures)
	{
		delete element.second;
		element.second = nullptr;
	}

	for (auto& anim : animations)
	{
		delete anim.second;
		anim.second = nullptr;
	}

	// Delete the renderer.
	delete m_2dRenderer;
}

void Game2D::Update(float deltaTime)
{
	aie::Input* input = aie::Input::GetInstance();
	aie::Application* application = aie::Application::GetInstance();

	// Exit the application if escape is pressed.
	if (input->IsKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		application->Quit();
	}

	float camPosX;
	float camPosY;

	m_2dRenderer->GetCameraPos(camPosX, camPosY);

	Vector2 mousePos = { (float)input->GetMouseX(), (float)input->GetMouseY() };

	mousePos += {camPosX, camPosY};

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
		clone->GetComponent<Player>()->SetTargeter(clone->GetTransform()->GetChild(0));
	}

	if (input->WasKeyPressed(aie::INPUT_KEY_B))
		drawColliders = !drawColliders;
	
	if (input->IsKeyDown(aie::INPUT_KEY_M))
		application->SetTimeScale(0.5f);
	else
		application->SetTimeScale(1.0f);

	//Drag dudes
	if (input->IsMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		if (physTarget == nullptr)
		{
			float dist = 30.0f;

			physTarget = nullptr;

			for (auto& child : *mainScene->GetTransform()->GetChildrenList())
			{
				PhysObject* phys = child->GetEntity()->GetComponent<PhysObject>();

				if (phys != nullptr)
				{
					float newDist = (child->GetGlobalPosition() - mousePos).Magnitude();
					if (newDist < dist)
					{
						physTarget = phys;
						dist = newDist;
					}
				}
			}
		}
		else
		{
			Vector2 vec = mousePos - physTarget->GetEntity()->GetTransform()->GetGlobalPosition();
			float mag = vec.Magnitude();
			if (mag > 0)
			{
				vec /= mag;
				physTarget->AddForce(vec * 300.0f * deltaTime);
			}
		}
	}
	else
	{
		physTarget = nullptr;
	}

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
	while (!spriteDrawCalls.empty())
	{
		spriteDrawCalls.front()->Draw();
		spriteDrawCalls.pop();
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
	m_2dRenderer->DrawText2D(m_font, "WASD to move camera.", 15.0f, windowHeight - 96.0f);

	// Done drawing sprites. Must be called at the end of the Draw().
	m_2dRenderer->End();
}