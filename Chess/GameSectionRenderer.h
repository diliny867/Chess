#pragma once

#include <SDL2/SDL_render.h>

#include "Common.h"

class GameSectionRenderer {
protected:
	SDL_Renderer* currRenderer = nullptr;
	SDL_Texture* renderTargetTexture = nullptr;

	SDL_Point onScreenPosition = {0,0};
	SDL_Point onScreenSize= {0,0};

	void renderSquare(SDL_Point position, SDL_Point size, i8 r, i8 g, i8 b, i8 a);
	void renderCircle(SDL_Point position, i32 rad, i8 r, i8 g, i8 b, i8 a);
	void renderCharacter(SDL_Point position, f32 scale, char c, i8 r, i8 g, i8 b, i8 a);
	void init(SDL_Renderer* renderer, SDL_Point size);
	void initRender(SDL_Renderer* renderer/*, SDL_Point position, SDL_Point size*/);
	void present(SDL_Point position, SDL_Point size);
public:
	virtual void Init(class Game* game, SDL_Point size);

	virtual void Render(SDL_Renderer* renderer, SDL_Point position, SDL_Point size);
	virtual ~GameSectionRenderer();
};

