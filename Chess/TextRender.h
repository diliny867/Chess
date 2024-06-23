#pragma once

#include "Image.h"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_gfxPrimitives_font.h>

class TextRender {
private:
public:
	inline static u32 gfxPrimCharWidth = 8; // from SDL2_gfxPrimitives.c
	inline static u32 gfxPrimCharHeight = 8;
	i32Vec2 position = {0,0};
	f32Vec2 scale = {0,0};
	std::string text;

	void Init(SDL_Renderer* renderer){
		gfxPrimitivesSetFont(gfxPrimitivesFontdata, gfxPrimCharWidth, gfxPrimCharHeight); //set default gfx_Primitives font
		text = "";
	}

	void SetFontSize(u32 charW, u32 charH) {
		gfxPrimitivesSetFont(gfxPrimitivesFontdata, charW, charH);
		gfxPrimCharWidth = charW;
		gfxPrimCharHeight = charH;
	}

	void Render(SDL_Renderer* renderer, bool rightToLeft = false) {
		i16 offsetX = text.size() * gfxPrimCharWidth * i16(rightToLeft);
		SDL_RenderSetScale(renderer, scale.x, scale.y);
		stringRGBA(renderer, position.x/scale.y - offsetX, position.y/scale.y, text.c_str(), 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		SDL_RenderSetScale(renderer, 1, 1);
	}
};
