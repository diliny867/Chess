#pragma once

#include "Image.h"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_gfxPrimitives_font.h>

class TextRender {
private:
	i32Vec2 fontSize = {8,8}; // 8 from SDL2_gfxPrimitives.c
public:
	i32Vec2 position = {0,0};
	f32Vec2 scale = {0,0};
	std::string text;

	void Init(SDL_Renderer* renderer){
		gfxPrimitivesSetFont(gfxPrimitivesFontdata, fontSize.x, fontSize.y); //set default gfx_Primitives font
		text = "";
	}

	i32Vec2 GetFontSize() {
		return fontSize;
	}
	void SetFontSize(i32 charW, i32 charH) { // why breaks
		gfxPrimitivesSetFont(gfxPrimitivesFontdata, charW, charH);
		fontSize.x = charW;
		fontSize.y = charH;
	}

	void Render(SDL_Renderer* renderer, bool rightToLeft = false) {
		i16 offsetX = text.size() * fontSize.x * i16(rightToLeft);
		SDL_RenderSetScale(renderer, scale.x, scale.y);
		stringRGBA(renderer, position.x/scale.x - offsetX, position.y/scale.y, text.c_str(), 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		SDL_RenderSetScale(renderer, 1, 1);
	}
};
