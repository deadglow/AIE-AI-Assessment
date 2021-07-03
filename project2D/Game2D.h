#pragma once

#include "Game.h"
#include "Renderer2D.h"
#include "Scene.h"

class Game2D : public aie::Game
{
public:
	static float deltaTime;

	Game2D(const char* title, int width, int height, bool fullscreen);
	virtual ~Game2D();

	virtual void Update(float deltaTime);
	virtual void Draw();

protected:
	aie::Renderer2D* m_2dRenderer;

	// Example textures.
	aie::Font* m_font;

	Scene* mainScene;
	Entity* player;
};