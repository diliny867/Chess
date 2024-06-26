#include "MenuRenderer.h"

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Game.h"


void MenuRenderer::Init(i32Vec2 size) {
	Game* game = Game::GetCurrentGame();
	init(game->GetRenderer(),size);
	menu = game->GetMenuLogic();
	checkboxRender.Init(currRenderer,"res/checkbox-checked.png", "res/checkbox-unchecked.png");
	buttonRender.Init(currRenderer, "res/button-darkgreen.png", "res/button-hovered-darkgreen.png");
	textRender.Init(currRenderer);
}


void MenuRenderer::renderBackground() {
	clear(0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	const i32Vec2 size = Game::menuSize;
	const i32 offset = 4;
	renderSquare({offset,offset},{size.x-offset*2,size.y-offset*2},0xee, 0xee, 0xd2,SDL_ALPHA_OPAQUE);
}

void MenuRenderer::renderButton(const MenuLogic::FuncionData& fd) {
	auto mouse = Game::GetMouseRelative(Game::menuPosition);
	bool hovered = mouse.x >= fd.position.x && mouse.x <= fd.position.x+fd.size.x && mouse.y >= fd.position.y && mouse.y <= fd.position.y+fd.size.y;
	switch(fd.metadata->type) {
	case MenuLogic::Click:
		buttonRender.position = fd.position;
		buttonRender.size = fd.size;
		buttonRender.Render(currRenderer, hovered);
		break;
	case MenuLogic::Checkbox:
		checkboxRender.position = fd.position;
		checkboxRender.size = fd.size;
		checkboxRender.Render(currRenderer, reinterpret_cast<MenuLogic::CheckBoxMetadata*>(fd.metadata)->active);
		break;
	}
	if(fd.metadata->onHoverText.show && hovered) {
		auto mouseAbsolute = Game::mouse.pos;
		textRender.position = mouseAbsolute;
		//textRender.position.x += 35;
		//textRender.position.y += 35;
		textRender.text = fd.metadata->onHoverText.text;
		textRender.textScale = {1.25, 1.25};
		textRender.borderSize = {10,10};
		//textRender.SetFontSize(12, 12);
		Game::GetCurrentGame()->SetOverlayRenderContext();
		textRender.Render(currRenderer, true);
		SetCurrentContext();
	}
}

void MenuRenderer::renderButtons() {
	for(const auto& button: menu->buttons) {
		for(const auto& fd: button.second) {
			renderButton(fd);
		}
	}
}

void MenuRenderer::Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) {
	initRender(renderer);
	renderBackground();
	renderButtons();
	present(position,size);
}