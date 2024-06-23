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

void GameSectionRenderer::clear(i8 r, i8 g, i8 b, i8 a) {
	SDL_SetRenderDrawColor(currRenderer,r,g,b,a);
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
	SDL_SetTextureBlendMode(renderTargetTexture, SDL_BLENDMODE_BLEND);
}

void GameSectionRenderer::init(SDL_Renderer* renderer, i32Vec2 size) {
	currRenderer = renderer;
	resetTexture(size);
}

void GameSectionRenderer::initRender(SDL_Renderer* renderer/*, i32Vec2 position, i32Vec2 size*/) {
	currRenderer = renderer;
	//onScreenPosition = position;
	//onScreenSize = size;
	SetCurrentContext();
}

void GameSectionRenderer::SetCurrentContext() {
	SDL_SetRenderTarget(currRenderer, renderTargetTexture);
}
void GameSectionRenderer::SetGlobalContext() {
	SDL_SetRenderTarget(currRenderer, NULL);
}

void GameSectionRenderer::present(i32Vec2 position, i32Vec2 size) {
	SDL_Rect rect = {position.x, position.y, size.x, size.y};
	SetGlobalContext();
	SDL_RenderCopy(currRenderer, renderTargetTexture, NULL, &rect);
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
	SetCurrentContext();
	clear(0, 0, 0, 0);
	SetGlobalContext();
}

GameSectionRenderer::~GameSectionRenderer() {
	SDL_DestroyTexture(renderTargetTexture);
}
