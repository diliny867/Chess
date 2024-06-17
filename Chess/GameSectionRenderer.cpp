#include "GameSectionRenderer.h"

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Game.h"

void GameSectionRenderer::renderSquare(i32Vec2 position, i32Vec2 size, i8 r, i8 g, i8 b, i8 a) {
	boxRGBA(currRenderer, position.x, position.y, position.x+size.x, position.y+size.y, r, g, b, a);
}
void GameSectionRenderer::renderCircle(i32Vec2 position, i32 rad, i8 r, i8 g, i8 b, i8 a) {
	filledCircleRGBA(currRenderer, position.x, position.y, rad, r, g, b, a);
}
void GameSectionRenderer::renderCharacter(i32Vec2 position, f32 scale, char c, i8 r, i8 g, i8 b, i8 a) {
	SDL_RenderSetScale(currRenderer, scale, scale);
	characterRGBA(currRenderer, position.x/scale, position.y/scale, c, r, g, b, a);
	SDL_RenderSetScale(currRenderer, 1, 1);
}

void GameSectionRenderer::clear() {
	SDL_SetRenderDrawColor(currRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(currRenderer);
}

void GameSectionRenderer::resetTexture(i32Vec2 size) {
	if(renderTargetTexture != nullptr) {
		SDL_DestroyTexture(renderTargetTexture);
	}
	renderTargetTexture = SDL_CreateTexture(currRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	if (renderTargetTexture == NULL) {
		std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
	}
}

void GameSectionRenderer::init(SDL_Renderer* renderer, i32Vec2 size) {
	currRenderer = renderer;
	resetTexture(size);
}

void GameSectionRenderer::initRender(SDL_Renderer* renderer/*, i32Vec2 position, i32Vec2 size*/) {
	currRenderer = renderer;
	//onScreenPosition = position;
	//onScreenSize = size;
	SDL_SetRenderTarget(currRenderer, renderTargetTexture);
	clear();
}
void GameSectionRenderer::present(i32Vec2 position, i32Vec2 size) {
	SDL_Rect rect = {position.x, position.y, size.x, size.y};
	SDL_SetRenderTarget(currRenderer, NULL);
	SDL_RenderCopy(currRenderer, renderTargetTexture, NULL, &rect);
	SDL_RenderPresent(currRenderer);
}

void GameSectionRenderer::Init(i32Vec2 size) {
	init(Game::GetCurrentGame()->GetRenderer(), size);

}

void GameSectionRenderer::ResetSize(SDL_Renderer* renderer, i32Vec2 size) {
	currRenderer = renderer;
	resetTexture(size);
}

void GameSectionRenderer::Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) {
	initRender(renderer/*, position, size*/);

	//Do the render thing

	present(position, size);
}

GameSectionRenderer::~GameSectionRenderer() {
	SDL_DestroyTexture(renderTargetTexture);
}
