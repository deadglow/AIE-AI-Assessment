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


class Sprite;
class Animation;
class Scene;
class Entity;
class CollisionManager;
class PhysObject;

class Game2D : public aie::Game
{
public:

	Game2D(const char* title, int width, int height, bool fullscreen);
	virtual ~Game2D();

	virtual void Update(float deltaTime);
	virtual void Draw();

	aie::Renderer2D* GetRenderer();

	void LoadTextures();
	aie::Texture* GetTexture(std::string name);

	void LoadAnimations();
	Animation* GetAnimation(std::string name);

	void CreateSceneFromMap();

	Scene* GetMainScene();
	CollisionManager* GetCollisionManager();
	Pathfinder* GetPathfinder();

	void AddSpriteDrawCall(Sprite* sprite);

	SoundFieldNode* flowField = nullptr;
protected:
	aie::Renderer2D* m_2dRenderer;

	CollisionManager* collisionManager;

	MushRoom* room;

	Pathfinder* pathfinder;

	Scene* mainScene;
	
	Entity* player;
	Entity* wall;
	
	aie::Font* m_font;

	std::map<std::string, aie::Texture*> textures;
	std::map<std::string, Animation*> animations;

	std::queue<Sprite*> spriteDrawCalls;

	bool drawColliders = false;

	PhysObject* physTarget;
};