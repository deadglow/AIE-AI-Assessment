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

Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	m_2dRenderer = new aie::Renderer2D();
	aie::Application::GetInstance()->SetVSync(false);

	//Initialise font, textures and animation
	m_font = new aie::Font(FONT_DIRECTORY, 20);
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

	tutorialTimer -= deltaTime;

	// Exit the application if escape is pressed.
	if (input->IsKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		playerQuit = true;
		application->Quit();
	}

	if ((winObject->GetTransform()->GetGlobalPosition() - playerSpawn).SqrMagnitude() < WIN_RADIUS * WIN_RADIUS)
	{
		GameWin();
	}

	//Check player dead
	if (!player->GetComponent<Player>()->IsAlive())
	{
		application->Quit();
	}

	//Update slowmotion
	if (slowMotionTimer > 0)
	{
		slowMotionTimer -= application->GetUnscaledDeltaTime();
		if (slowMotionTimer <= 0)
		{
			slowMotionTimer = 0;
			application->SetTimeScale(1.0f);
		}
	}

	//Get camera position
	Vector2 camPos = Vector2();
	m_2dRenderer->GetCameraPos(camPos.x, camPos.y);

	//Get mousePosition in world space
	Vector2 mousePos = { (float)input->GetMouseX(), (float)input->GetMouseY() };
	mousePos += camPos;

	if (input->WasKeyPressed(aie::INPUT_KEY_B))
		drawColliders = !drawColliders;

	//Update player flowfield
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

	//Update cameraPosition
	Vector2 newCamPos = player->GetTransform()->GetGlobalPosition();
	//Center camera on player
	newCamPos = newCamPos - Vector2((float)application->GetWindowWidth(), (float)application->GetWindowHeight()) / 2;
	
	Vector2 offset = newCamPos - camPos;
	float dist = offset.Magnitude();
	if (dist > 0)
	{
		Vector2 direction = offset / dist;
		//Player is far enough away from the center to limit the camera closer
		if (dist > CAMERA_EDGE)
		{
			//Only move camera so the player is within the defined limits
			newCamPos -= direction * (CAMERA_EDGE);
			m_2dRenderer->SetCameraPos(newCamPos.x, newCamPos.y);
		}
	}
}

void Game2D::Draw()
{
	aie::Application* application = aie::Application::GetInstance();
	float time = application->GetTime();

	// Wipe the screen to clear away the previous frame.
	application->ClearScreen();

	// Prepare the renderer. This must be called before any sprites are drawn.
	m_2dRenderer->Begin();
	
	//Set default sprite colour
	m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f);

	//Initialise variables for flashlight drawing
	Vector2 playerPos = player->GetTransform()->GetGlobalPosition();
	Sprite* sprite;
	float dist, nodeDist, finalRatio, radiusRatio, gScoreRatio;
	PathfinderNode* node;
	
	//Draw all sprites in queue
	while (!spriteDrawCalls.empty())
	{
		sprite = spriteDrawCalls.front();
		spriteDrawCalls.pop();

		if (playerFlowField != nullptr)
		{
			//Calculates ratio of the sprite's distance from the player and the radius of the flashlight
			dist = (playerPos - sprite->GetEntity()->GetTransform()->GetGlobalPosition()).SqrMagnitude();
			radiusRatio = dist / (FLASHLIGHT_RADIUS * FLASHLIGHT_RADIUS);
			
			//Calculates ratio of the sprite's gscore from player and the flashlight max gscore
			node = pathfinder->GetNodeFromPos(sprite->GetEntity()->GetTransform()->GetGlobalPosition());
			nodeDist = (float)playerFlowField[pathfinder->GetIndex(node->x, node->y)].gScore;
			gScoreRatio = nodeDist / FLASHLIGHT_MAX_GSCORE;

			//Choose the largest ratio
			finalRatio = std::max(radiusRatio, gScoreRatio);

			//Negative quadratic falloff, clamped to 0-1. Falloff dictates how steep the falloff is
			finalRatio = std::clamp(FLASHLIGHT_FALLOFF - (FLASHLIGHT_FALLOFF * finalRatio * finalRatio), 0.0f, 1.0f);
			//Set the 
			m_2dRenderer->SetRenderColour(1.0f * finalRatio, 1.0f * finalRatio, 1.0f * finalRatio);
		}
		//DRAW!
		sprite->Draw();
	}

	//Debug purposes
	if (drawColliders)
	{
		//Draw colliders
		m_2dRenderer->SetRenderColour(0.0f, 1.0f, 0.0f);
		collisionManager->DrawColliders(m_2dRenderer);

		//Draw last soundfield
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
	float windowWidth = (float)application->GetWindowWidth();
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", application->GetFPS());
	Vector2 camPos;
	m_2dRenderer->GetCameraPos(camPos.x, camPos.y);
	m_2dRenderer->DrawText2D(m_font, fps, camPos.x + 15.0f, camPos.y + windowHeight - 32.0f);
	m_2dRenderer->DrawText2D(m_font, "WASD to move", camPos.x + 15.0f, camPos.y + windowHeight - 52.0f);
	m_2dRenderer->DrawText2D(m_font, "Mouse2: Pickup/Drop item", camPos.x + 15.0f, camPos.y + windowHeight - 72.0f);
	m_2dRenderer->DrawText2D(m_font, "Mouse1: Throw item", camPos.x + 15.0f, camPos.y + windowHeight - 92.0f);
	m_2dRenderer->DrawText2D(m_font, "ESC: Quit", camPos.x + 15.0f, camPos.y + windowHeight - 112.0f);
	
	//Win message
	if (gameWon)
		m_2dRenderer->DrawText2D(m_font, "You win!", camPos.x + (windowWidth * 0.5f) - 100.0f, camPos.y + (windowHeight * 0.5f));

	//Fades out after a bit
	if (tutorialTimer > 0)
	{
		float ratio = tutorialTimer;
		m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f, std::min(tutorialTimer, 1.0f));
		m_2dRenderer->DrawText2D(m_font, "Find the relic and return here. They can hear you...", camPos.x + (windowWidth * 0.5f) - 250.0f, camPos.y + (windowHeight * 0.5f));
	}

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

	//Load a texture for every entry in the directory
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		textures.insert(std::make_pair(entry.path().filename().string(), new aie::Texture(entry.path().string().c_str())));
	}
}

void Game2D::LoadAnimations()
{
	std::string path = ANIM_DIRECTORY;

	//Loop through all entries
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		//Open anim file
		std::ifstream animFile(entry.path().string().c_str());

		std::string line;

		//Add new animation to animation list
		Animation* animation = new Animation();
		animation->SetName(entry.path().filename().string());
		animations.insert(std::make_pair(animation->GetName(), animation));

		//Loop through each line, using the read text to get a texture
		while (std::getline(animFile, line))
		{
			animation->AddFrame(GetTexture(line));
		}
	}
}

void Game2D::CreateSceneFromMap()
{
	int data;
	Entity* entity;
	//Loop through the grid
	for (int y = 0; y < room->GetHeight(); ++y)
	{
		for (int x = 0; x < room->GetWidth(); ++x)
		{
			data = room->GetData(x, y);
			switch (data)
			{
			case 1:
			{
				//Create wall
				entity = wallTemplate->Clone();
				entity->GetTransform()->SetLocalPosition({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
				pathfinder->GetNode(x, y)->blocked = true;
			}
			break;

			case 2:
			{
				//Set player spawn and create marker
				Vector2 vec = { (float)x * CELL_SIZE, (float)y * CELL_SIZE };
				SetPlayerSpawn(vec);
				entity = mainScene->CreateEntity();
				entity->AddComponent<Sprite>()->SetAnimation(GetAnimation("load.anim"));
				entity->GetTransform()->SetLocalPosition(vec);
			}
			break;

			case 3:
			{
				//Create relic
				winObject = throwableTemplate->Clone();
				Sprite* spr = winObject->GetComponent<Sprite>();
				spr->SetAnimation(GetAnimation("relic.anim"));
				spr->SetFrameRate(5.0f);
				throwables.push_back(winObject->GetComponent<Throwable>());
				winObject->GetTransform()->SetLocalPosition({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
			}
			break;

			case 4:
			{
				//Duplicate AI
				entity = enemyTemplate->Clone();
				AIAgent* agent = entity->GetComponent<AIAgent>();
				aiManager->AddAgent(agent);
				agent->SetSpawn({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
				entity->GetTransform()->SetLocalPosition(agent->GetSpawn());
			}
			break;

			case 5:
			{
				//Dupe throwable
				entity = throwableTemplate->Clone();
				throwables.push_back(entity->GetComponent<Throwable>());
				entity->GetTransform()->SetLocalPosition({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
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

void Game2D::SetPlayerSpawn(Vector2 pos)
{
	playerSpawn = pos;
}

void Game2D::StartSlowMotion(float timeScale, float duration)
{
	aie::Application::GetInstance()->SetTimeScale(timeScale);
	slowMotionTimer = duration;
}

Vector2 Game2D::GetWorldMousePos()
{
	Vector2 camPos = Vector2();
	m_2dRenderer->GetCameraPos(camPos.x, camPos.y);

	//Get mousePosition in world space
	Vector2 mousePos = { (float)aie::Input::GetInstance()->GetMouseX(), (float)aie::Input::GetInstance()->GetMouseY() };
	mousePos += camPos;

	return mousePos;
}

void Game2D::GameWin()
{
	StartSlowMotion(0.2f, 100.0f);
	gameWon = true;
}
