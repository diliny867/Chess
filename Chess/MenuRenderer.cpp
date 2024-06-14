#include "MenuRenderer.h"

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Game.h"

void MenuRenderer::Init(Game* game, SDL_Point size) {
	init(game->GetRenderer(),size);
	chess = game->GetChessLogic();
}
void MenuRenderer::Render(SDL_Renderer* renderer, SDL_Point position, SDL_Point size) {
	initRender(renderer);

	present(position,size);
}