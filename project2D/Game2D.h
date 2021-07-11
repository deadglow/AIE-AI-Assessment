#pragma once

#include <map>
#include <string>
#include <vector>
#include "Game.h"
#include "Renderer2D.h"

class Sprite;
class Scene;
class Entity;

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

	float GetDeltaTime();
	float GetUnscaledDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float scale);

	void AddSpriteDrawCall(Sprite* sprite);

protected:

	float unscaledDeltaTime = 1.0f;
	float deltaTime = 1.0f;
	float timeScale = 1.0f;

	aie::Renderer2D* m_2dRenderer;

	aie::Font* m_font;

	Scene* mainScene;
	Entity* player;
	
	std::map<std::string, aie::Texture*> textures;
	std::vector<Sprite*> sprites;
};