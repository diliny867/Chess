#pragma once

#include <unordered_map>
#include <functional>

#include "Common.h"

class MenuLogic {
public:
	enum ButtonType {
		Click,
		Checkbox
	};
	enum FunctionActions {
		None,
		ToggleFlipBoardOnTurn,
		FlipBoard,
	};
	struct ButtonMetadata {
		ButtonType type = Click;
		struct{
			bool show = false;
			std::string text = "";
		} onHoverText = {};
	};
	struct CheckBoxMetadata : ButtonMetadata {
		bool active = false;
	};
	struct FuncionData{
		FunctionActions action = None;
		bool triggerPressed = false;
		bool triggerOnPress = false; // on press or on release
		i32Vec2 position = {0,0};
		i32Vec2 size = {0,0};
		ButtonMetadata* metadata;
		std::function<void(FuncionData& fd)> function = nullptr;
		//void(MenuLogic::* function)() = nullptr;
	};
	enum TriggerButton {
		Left,
		Right,
		Both
	};
private:
	friend class MenuRenderer;

	class Game* game = nullptr;
	class ChessLogic* chess = nullptr;

	std::unordered_map<TriggerButton, std::vector<FuncionData>> buttons;

	void toggleFlipBoardOnTurn();
	void flipBoard();

	void initButtons();
public:
	void Init();

	void PressLeft();
	void PressRight();

	void ReleaseLeft();
	void ReleaseRight();
};
