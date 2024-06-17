#pragma once

#include <SDL2/SDL_render.h>

#include "Common.h"

class GameSectionRenderer {
protected:
	SDL_Renderer* currRenderer = nullptr;
	SDL_Texture* renderTargetTexture = nullptr;

	//i32Vec2 onScreenPosition = {0,0};
	//i32Vec2 onScreenSize= {0,0};

	void renderSquare(i32Vec2 position, i32Vec2 size, i8 r, i8 g, i8 b, i8 a);
	void renderCircle(i32Vec2 position, i32 rad, i8 r, i8 g, i8 b, i8 a);
	void renderCharacter(i32Vec2 position, f32 scale, char c, i8 r, i8 g, i8 b, i8 a);
	void clear();
	void resetTexture(i32Vec2 size);
	void init(SDL_Renderer* renderer, i32Vec2 size);
	void initRender(SDL_Renderer* renderer/*, i32Vec2 position, i32Vec2 size*/);
	void present(i32Vec2 position, i32Vec2 size);
public:
	virtual void Init(i32Vec2 size);

	void ResetSize(SDL_Renderer* renderer, i32Vec2 size);

	virtual void Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size);
	virtual ~GameSectionRenderer();
};

