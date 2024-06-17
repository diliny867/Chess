#pragma once

#include <SDL2/SDL.h>

#include "GameSectionRenderer.h"

class MenuRenderer: public GameSectionRenderer {
private:
	class ChessLogic* chess = nullptr;

public:
	void Init(i32Vec2 size) override;

	void Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) override;
};

