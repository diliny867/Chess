#pragma once

#include <unordered_map>
#include <functional>

#include "Common.h"

class MenuLogic {
private:
	friend class MenuRenderer;

	class Game* game = nullptr;
	class ChessLogic* chess = nullptr;

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
	};
	struct CheckBoxMetadata : ButtonMetadata {
		bool active = false;
	};
	struct FuncionData{
		FunctionActions action = None;
		bool pressed = false;
		bool onPress = false; // on press or on release
		ButtonMetadata* metadata;
		i32Vec2 position = {0,0};
		i32Vec2 size = {0,0};
		std::function<void(FuncionData& fd)> function = nullptr;
		//void(MenuLogic::* function)() = nullptr;
	};
	enum Button {
		Left,
		Right,
		Both
	};
	std::unordered_map<Button, std::vector<FuncionData>> buttons;


	void trigger(FuncionData& fd);

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
