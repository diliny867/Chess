#pragma once

#include <SDL2/SDL.h>

#include "GameSectionRenderer.h"

class MenuRenderer: public GameSectionRenderer {
private:
	class ChessLogic* chess = nullptr;

public:
	void Init(class Game* game, SDL_Point size) override;

	void Render(SDL_Renderer* renderer, SDL_Point position, SDL_Point size) override;
};

