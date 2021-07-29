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

protected:
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
	
	Entity* player;
	Entity* wallTemplate;
	Entity* enemyTemplate;
	Entity* throwableTemplate;

	std::vector<Throwable*> throwables;

	//Used for lighting
	SoundFieldNode* playerFlowField = nullptr;

	std::queue<Sprite*> spriteDrawCalls;

	bool drawColliders = false;

	//Temp
	PhysObject* physTarget;
};