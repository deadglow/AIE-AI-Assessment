#pragma once

#include <map>
#include <string>
#include <vector>
#include <queue>
#include "Game.h"
#include "Vector2.h"
#include "Renderer2D.h"
#include "MushRoom.h"
#include "Pathfinder.h"
#include "SoundFieldNode.h"

#define CELL_SIZE 32
#define TEXTURE_DIRECTORY "textures/"
#define MUSHROOM_DIRECTORY "maps/exports/"
#define FONT_DIRECTORY "font/consolas.ttf"
#define ANIM_DIRECTORY "anims/"

#define LOADED_MAP "hive.mushroom"

#define WIN_RADIUS 60.0f

#define AI_PHYS_DRAG 0.4f
#define AI_PHYS_ADRAG 0.4f
#define AI_PHYS_RES 0.2f
#define THROWABLE_PHYS_DRAG 0.6f
#define THROWABLE_PHYS_ADRAG 0.8f
#define THROWABLE_PHYS_RES 0.3f

#define FLASHLIGHT_FALLOFF 2
#define FLASHLIGHT_RADIUS 450.0f
#define FLASHLIGHT_MAX_GSCORE 200.0f

#define CAMERA_EDGE 100.0f
#define TUTORIAL_TIME 5.0f

class CollisionManager;
class AIManager;
class Scene;
class Entity;
class Sprite;
class Animation;
class PhysObject;
class Throwable;

class Game2D : public aie::Game
{
public:

	Game2D(const char* title, int width, int height, bool fullscreen);
	virtual ~Game2D();

	virtual void Update(float deltaTime);
	virtual void Draw();

	aie::Renderer2D* GetRenderer();

	aie::Texture* GetTexture(std::string name);
	
	Animation* GetAnimation(std::string name);
	
	CollisionManager* GetCollisionManager();
	
	Pathfinder* GetPathfinder();

	AIManager* GetAIManager();
	
	Scene* GetMainScene();

	Throwable* GetThrowable(int i);

	int GetThrowableCount();

	void LoadTextures();

	void LoadAnimations();

	void CreateSceneFromMap();

	void AddSpriteDrawCall(Sprite* sprite);

	void SetPlayerSpawn(Vector2 pos);

	void StartSlowMotion(float timeScale, float duration);

	Vector2 GetWorldMousePos();

	//Used to distinguish the player dying and the player quitting
	bool playerQuit = false;

protected:
	void GameWin();

	aie::Renderer2D* m_2dRenderer;

	std::map<std::string, aie::Texture*> textures;
	std::map<std::string, Animation*> animations;

	aie::Font* m_font;

	CollisionManager* collisionManager;
	Pathfinder* pathfinder;
	AIManager* aiManager;
	MushRoom* room;
	Scene* mainScene;

	Vector2 playerSpawn;
	
	bool gameWon = false;
	Entity* player;
	Entity* winObject;
	Entity* wallTemplate;
	Entity* enemyTemplate;
	Entity* throwableTemplate;

	std::vector<Throwable*> throwables;

	//Used for lighting
	SoundFieldNode* playerFlowField = nullptr;

	std::queue<Sprite*> spriteDrawCalls;

	bool drawColliders = false;

	float slowMotionTimer = 0;
	float tutorialTimer = TUTORIAL_TIME;
};