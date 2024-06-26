#pragma once

#include <SDL2/SDL.h>

#include "CheckboxRender.h"
#include "ButtonRender.h"
#include "TextBoxRender.h"

#include "GameSectionRenderer.h"
#include "MenuLogic.h"

class MenuRenderer: public GameSectionRenderer {
private:
	MenuLogic* menu = nullptr;

	CheckboxRender checkboxRender;
	ButtonRender buttonRender;
	TextBoxRender textRender;

	void renderBackground();
	void renderButton(const MenuLogic::FuncionData& fd);
	void renderButtons();

public:
	void Init(i32Vec2 size) override;

	void Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) override;
};

