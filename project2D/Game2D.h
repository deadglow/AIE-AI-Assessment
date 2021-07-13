#pragma once

#include <map>
#include <string>
#include <vector>
#include <queue>
#include "Game.h"
#include "Vector2.h"
#include "Renderer2D.h"

class Sprite;
class Scene;
class Entity;
class CollisionManager;

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

	Scene* GetMainScene();

	void AddSpriteDrawCall(Sprite* sprite);

protected:
	aie::Renderer2D* m_2dRenderer;

	CollisionManager* collisionManager;

	Scene* mainScene;
	Entity* player;
	
	aie::Font* m_font;
	std::map<std::string, aie::Texture*> textures;
	std::queue<Sprite*> sprites;
};