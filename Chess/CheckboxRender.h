#pragma once

#include "Image.h"


class CheckboxRender {
private:
	Image checkedImg;
	Image uncheckedImg;
public:
	i32Vec2 position = {0,0};
	i32Vec2 size = {0,0};

	void Init(SDL_Renderer* renderer, const std::string& checkedPath, const std::string& uncheckedPath){
		checkedImg.Init(renderer, checkedPath);
		uncheckedImg.Init(renderer, uncheckedPath);
	}

	void Render(SDL_Renderer* renderer, bool checked) {
		if(checked) {
			checkedImg.position = position;
			checkedImg.size = size;
			checkedImg.Render(renderer);
		}else {
			uncheckedImg.position = position;
			uncheckedImg.size = size;
			uncheckedImg.Render(renderer);
		}
	}
};
