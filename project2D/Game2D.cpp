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
#include "AIManager.h"
#include "ComponentIncludes.h"

#define TEXTURE_DIRECTORY "textures/"
#define MUSHROOM_DIRECTORY "maps/exports/"
#define FONT_DIRECTORY "font/consolas.ttf"
#define ANIM_DIRECTORY "anims/"

#define LOADED_MAP "map.mushroom"

#define AI_PHYS_DRAG 0.4f
#define AI_PHYS_ADRAG 0.4f
#define AI_PHYS_RES 0.2f
#define THROWABLE_PHYS_DRAG 0.6f
#define THROWABLE_PHYS_ADRAG 0.8f
#define THROWABLE_PHYS_RES 0.3f

Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	m_2dRenderer = new aie::Renderer2D();
	aie::Application::GetInstance()->SetVSync(false);

	//Initialise font, textures and animation
	m_font = new aie::Font(FONT_DIRECTORY, 24);
	LoadTextures();
	LoadAnimations();

	//Create room from file
	room = new MushRoom(MUSHROOM_DIRECTORY + std::string(LOADED_MAP));
	//Generate pathfinder with same dimensions as room
	pathfinder = new Pathfinder(room->GetWidth(), room->GetHeight(), CELL_SIZE);

	//Create ai manager
	aiManager = new AIManager(pathfinder);

	//Create collision manager
	collisionManager = new CollisionManager();

	//Create new scene
	mainScene = new Scene(this);

	//Create wall entity
	aie::Texture* tex = GetTexture("wall.png");
	wallTemplate = mainScene->CreateEntity();
	wallTemplate->AddComponent<Sprite>()->SetAnimation(GetAnimation("wall.anim"));
	ColliderBox* box = wallTemplate->AddComponent<ColliderBox>();
	box->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	box->SetStatic(true);
	wallTemplate->GetTransform()->SetLocalPosition(Vector2( 10000.0f, 10000.0f ));

	//Create AI agent
	tex = GetTexture("creep1.png");
	enemyTemplate = mainScene->CreateEntity();
	enemyTemplate->AddComponent<Sprite>()->SetAnimation(GetAnimation("creep.anim"));
	PhysObject* phys = enemyTemplate->AddComponent<PhysObject>();
	phys->SetDrag(AI_PHYS_DRAG);
	phys->SetAngularDrag(AI_PHYS_ADRAG);
	ColliderPolygon* colPol = enemyTemplate->AddComponent<ColliderPolygon>();
	colPol->GenerateShape(9, tex->GetWidth() / 2);
	colPol->SetRestitution(AI_PHYS_RES);
	enemyTemplate->AddComponent<AIAgent>();
	//Teleport away since it was just a template
	enemyTemplate->GetTransform()->SetLocalPosition(Vector2(10000.0f, 10000.0f));

	//Create throwable
	tex = GetTexture("battery1.png");
	throwableTemplate = mainScene->CreateEntity();
	throwableTemplate->AddComponent<Sprite>()->SetAnimation(GetAnimation("battery.anim"));
	phys = throwableTemplate->AddComponent<PhysObject>();
	phys->SetDrag(THROWABLE_PHYS_DRAG);
	phys->SetAngularDrag(THROWABLE_PHYS_ADRAG);
	box = throwableTemplate->AddComponent<ColliderBox>();
	box->GenerateBox(Vector2(tex->GetWidth(), tex->GetHeight()) / 2);
	box->SetRestitution(THROWABLE_PHYS_RES);
	throwableTemplate->AddComponent<Throwable>();
	throwableTemplate->GetTransform()->Translate(Vector2(100000, 100000));

	//Create scene now that wall and AI have been defined
	CreateSceneFromMap();
	
	//Generate spawnflowfields for the AI
	aiManager->CreateSpawnFlowFields();	

	//Player
	player = mainScene->CreateEntity();
	tex = GetTexture("legs.png");
	player->AddComponent<Player>();
	player->AddComponent<Sprite>()->SetAnimation(GetAnimation("legs.anim"));
	colPol = player->AddComponent<ColliderPolygon>();
	colPol->GenerateShape(9, tex->GetWidth() / 2);

	//Create upper body
	Entity* torso = mainScene->CreateEntity(player->GetTransform());
	Sprite* spr = torso->AddComponent<Sprite>();
	spr->SetAnimation(GetAnimation("torso.anim"));
	spr->SetFrameRate(0.0);
	spr->SetDepth(-1.0f);
	player->GetComponent<Player>()->SetTargeter(torso->GetTransform());

	player->GetTransform()->SetLocalPosition(playerSpawn);
}

Game2D::~Game2D()
{
	// Delete scene
	delete mainScene;

	delete collisionManager;

	delete room;

	delete aiManager;

	delete pathfinder;

	if (playerFlowField != nullptr)
		delete[] playerFlowField;


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
				physTarget->AddForce(vec * 600.0f * deltaTime);
			}
		}
	}
	else
	{
		physTarget = nullptr;
	}

	//if (input->WasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT))
	//{
	//	aiManager->CheckSound(Sound(mousePos, 500, 500));
	//}

	//Player flowfield
	if (playerFlowField != nullptr)
		delete[] playerFlowField;

	 playerFlowField = nullptr;
	pathfinder->CreateFlowField(playerFlowField, player->GetTransform()->GetGlobalPosition());


	//Call update and then check collisions
	mainScene->Update();
	aiManager->Update();
	mainScene->GetTransform()->UpdateGlobalMatrix();
	collisionManager->CheckCollisions();
	mainScene->GetTransform()->UpdateGlobalMatrix();

	Vector2 newCamPos = player->GetTransform()->GetGlobalPosition();
	newCamPos = newCamPos - Vector2((float)application->GetWindowWidth(), (float)application->GetWindowHeight()) / 2;
	m_2dRenderer->SetCameraPos(newCamPos.x, newCamPos.y);

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
	Vector2 playerPos = player->GetTransform()->GetGlobalPosition();

	while (!spriteDrawCalls.empty())
	{
		m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f);

		if (playerFlowField != nullptr)
		{
			float dist = (playerPos - spriteDrawCalls.front()->GetEntity()->GetTransform()->GetGlobalPosition()).Magnitude();
			PathfinderNode* node = pathfinder->GetNodeFromPos(spriteDrawCalls.front()->GetEntity()->GetTransform()->GetGlobalPosition());
			float nodeDist = (float)playerFlowField[pathfinder->GetIndex(node->x, node->y)].gScore;
	#define scale 2
	#define distance 350.0f
#define nodeDistance 150.0f
			float ratio = dist / distance;
			float otherRatio = nodeDist / nodeDistance;

			ratio = std::max(ratio, otherRatio);

			ratio = std::clamp(scale - (scale * ratio * ratio), 0.0f, 1.0f);
			m_2dRenderer->SetRenderColour(1.0f * ratio, 1.0f * ratio, 1.0f * ratio);

		}
		spriteDrawCalls.front()->Draw();
		spriteDrawCalls.pop();
	}

	if (drawColliders)
	{
		m_2dRenderer->SetRenderColour(0.0f, 1.0f, 0.0f);
		collisionManager->DrawColliders(m_2dRenderer);

		if (aiManager->GetSoundFieldCount() > 0)
		{
			SoundField* soundField = aiManager->GetNewestSoundField();

			m_2dRenderer->SetRenderColour(1.0f, 0.2f, 0.0f);
			for (int i = 0; i < pathfinder->GetWidth() * pathfinder->GetHeight(); ++i)
			{
				if (pathfinder->GetNode(i) != nullptr)
				{
					auto node = pathfinder->GetNode(i);
					if (!node->blocked)
					{
						Vector2 origin = (Vector2((float)node->x, (float)node->y) * CELL_SIZE);
						Vector2 end = origin + (soundField->GetSoundFieldNode(i)->direction * 20.0f);

						m_2dRenderer->DrawLine(origin.x, origin.y, end.x, end.y, 1.2f);
						m_2dRenderer->DrawCircle(origin.x, origin.y, 3.0f);

					}

				}
			}
		}
	}
	
	// Draw some text.
	m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f, 1.0f);
	float windowHeight = (float)application->GetWindowHeight();
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", application->GetFPS());
	Vector2 camPos;
	m_2dRenderer->GetCameraPos(camPos.x, camPos.y);
	m_2dRenderer->DrawText2D(m_font, fps, camPos.x + 15.0f, camPos.y + windowHeight - 32.0f);
	m_2dRenderer->DrawText2D(m_font, "WASD to move", camPos.x + 15.0f, camPos.y + windowHeight - 52.0f);
	m_2dRenderer->DrawText2D(m_font, "Mouse2: Pickup/Drop batteries", camPos.x + 15.0f, camPos.y + windowHeight - 72.0f);
	m_2dRenderer->DrawText2D(m_font, "Mouse1: Throw batteries", camPos.x + 15.0f, camPos.y + windowHeight - 92.0f);

	// Done drawing sprites. Must be called at the end of the Draw().
	m_2dRenderer->End();
}

aie::Renderer2D* Game2D::GetRenderer()
{
	return m_2dRenderer;
}

aie::Texture* Game2D::GetTexture(std::string name)
{
	return textures[name];
}

Animation* Game2D::GetAnimation(std::string name)
{
	return animations[name];
}

CollisionManager* Game2D::GetCollisionManager()
{
	return collisionManager;
}

Pathfinder* Game2D::GetPathfinder()
{
	return pathfinder;
}

AIManager* Game2D::GetAIManager()
{
	return aiManager;
}

Scene* Game2D::GetMainScene()
{
	return mainScene;
}

Throwable* Game2D::GetThrowable(int i)
{
	return throwables[i];
}

int Game2D::GetThrowableCount()
{
	return throwables.size();
}

void Game2D::LoadTextures()
{
	std::string path = TEXTURE_DIRECTORY;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		textures.insert(std::make_pair(entry.path().filename().string(), new aie::Texture(entry.path().string().c_str())));
	}
}

void Game2D::LoadAnimations()
{
	std::string path = ANIM_DIRECTORY;

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

void Game2D::CreateSceneFromMap()
{
	int data;
	float top = (float)room->GetHeight() * CELL_SIZE;
	Entity* entity;
	for (int y = 0; y < room->GetHeight(); ++y)
	{
		for (int x = 0; x < room->GetWidth(); ++x)
		{
			data = room->GetData(x, y);
			switch (data)
			{
			case 1:
			{
				entity = wallTemplate->Clone();
				entity->GetTransform()->SetLocalPosition({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
				pathfinder->GetNode(x, y)->blocked = true;
			}
			break;

			case 2:
			{
				SetPlayerSpawn({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
			}
			break;

			case 4:
			{
				entity = enemyTemplate->Clone();
				AIAgent* agent = entity->GetComponent<AIAgent>();
				aiManager->AddAgent(agent);
				agent->SetSpawn({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
				entity->GetTransform()->SetLocalPosition(agent->GetSpawn());
			}
			break;

			case 5:
			{
				entity = throwableTemplate->Clone();
				Throwable* throwable = entity->GetComponent<Throwable>();
				throwables.push_back(throwable);
				entity->GetTransform()->SetLocalPosition({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
			}

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

void Game2D::SetPlayerSpawn(Vector2 pos)
{
	playerSpawn = pos;
}
