#include "MenuRenderer.h"

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Game.h"

void MenuRenderer::Init(i32Vec2 size) {
	Game* game = Game::GetCurrentGame();
	init(game->GetRenderer(),size);
	chess = game->GetChessLogic();

}
void MenuRenderer::Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) {
	initRender(renderer);

	present(position,size);
}