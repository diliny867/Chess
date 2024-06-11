#pragma once

#include "Image.h"


class PieceRender {
private:
	Image blackImg;
	Image whiteImg;
public:
	SDL_Point position = {0,0};
	SDL_Point size = {0,0};

	enum PieceColor {
		White,
		Black
	};

	void Init(SDL_Renderer* renderer, const std::string& blackPath, const std::string& whitePath){
		blackImg.Init(renderer, blackPath);
		whiteImg.Init(renderer, whitePath);
	}
	
	void Render(SDL_Renderer* renderer, PieceColor color) {
		if(color == Black) {
			blackImg.position = position;
			blackImg.size = size;
			blackImg.Render(renderer);
		}else {
			whiteImg.position = position;
			whiteImg.size = size;
			whiteImg.Render(renderer);
		}
	}
};
