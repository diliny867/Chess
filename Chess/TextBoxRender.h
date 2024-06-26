#pragma once

#include "TextRender.h"

class TextBoxRender {
private:
	TextRender textRender;
	u32 bgColor = 0xFFFFFFFF;
public:
	i32Vec2 position = {0,0};
	i32Vec2 borderSize = {0,0};
	f32Vec2 textScale = {1,1};
	std::string text;

	void Init(SDL_Renderer* renderer){
		textRender.Init(renderer);
	}

	i32Vec2 GetFontSize() {
		return textRender.GetFontSize();
	}
	void SetFontSize(u32 charW, u32 charH) {
		textRender.SetFontSize(charW, charH);
	}

	void Render(SDL_Renderer* renderer, bool rightToLeft = false) {
		textRender.position = position;
		textRender.text = text;
		textRender.scale = textScale;
		auto fontSize = GetFontSize();
		i32Vec2 textSize = {static_cast<i32>(text.size() * fontSize.x), fontSize.y};
		i32 x1 = position.x - borderSize.x - (rightToLeft ? textSize.x : 0)*textScale.x;
		i32 y1 = position.y - borderSize.y;
		i32 x2 = x1 + textSize.x * textScale.x + borderSize.x * 2;
		i32 y2 = y1 + textSize.y * textScale.y + borderSize.y * 2;
		roundedBoxColor(renderer, x1, y1, x2, y2, 4, bgColor);
		textRender.Render(renderer, rightToLeft);
	}
};