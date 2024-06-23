#pragma once

#include "Image.h"

class ButtonRender {
private:
	Image buttonImg;
	Image buttonHoveredImg;
public:
	i32Vec2 position = {0,0};
	i32Vec2 size = {0,0};

	void Init(SDL_Renderer* renderer, const std::string& buttonPath, const std::string& buttonHoveredPath){
		buttonImg.Init(renderer, buttonPath);
		buttonHoveredImg.Init(renderer, buttonHoveredPath);
	}

	void Render(SDL_Renderer* renderer, bool hovered) {
		if(hovered) {
			buttonHoveredImg.position = position;
			buttonHoveredImg.size = size;
			buttonHoveredImg.Render(renderer);
		} else{
			buttonImg.position = position;
			buttonImg.size = size;
			buttonImg.Render(renderer);
		}
	}
};
