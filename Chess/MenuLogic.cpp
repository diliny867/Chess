#include "MenuLogic.h"

#include "Game.h"

#define AS(a, b) reinterpret_cast<b>(a)
void MenuLogic::trigger(FuncionData& fd) {
	auto mouse = Game::GetMouseRelative(Game::menuPosition);
	if(mouse.x >= fd.position.x && mouse.x <= fd.position.x+fd.size.x && mouse.y >= fd.position.y && mouse.y <= fd.position.y+fd.size.y) {
		//(this->*fd.function)();
		fd.function(fd);
	}
}

void MenuLogic::toggleFlipBoardOnTurn() {
	game->chessRenderer.SetFlipBoardOnTurn(!game->chessRenderer.FlipBoardOnTurn());
}
void MenuLogic::flipBoard() {
	game->chessRenderer.FlipBoard();
}

void MenuLogic::initButtons() {
	buttons[Left].push_back({
		ToggleFlipBoardOnTurn,
		false,
		true,
		new CheckBoxMetadata({{Checkbox}, this->game->chessRenderer.FlipBoardOnTurn()}),
		{20,20},
		{60,60},
		[this](FuncionData& fd) {
			this->toggleFlipBoardOnTurn();
			auto& active = AS(fd.metadata, MenuLogic::CheckBoxMetadata*)->active;
			active = this->game->chessRenderer.FlipBoardOnTurn();
		}
		//&MenuLogic::toggleFlipBoardOnTurn
	});
	buttons[Left].push_back({
		FlipBoard,
		false,
		true,
		new CheckBoxMetadata({{Click}, this->game->chessRenderer.IsBoardFlipped()}),
		{20,100},
		{60,60},
		[this](FuncionData& fd) {
			this->flipBoard();
			auto& active = AS(fd.metadata, MenuLogic::CheckBoxMetadata*)->active;
			active = this->game->chessRenderer.IsBoardFlipped();
		}
		//&MenuLogic::flipBoard
	});
}

void MenuLogic::Init() {
	game = Game::GetCurrentGame();
	initButtons();
}

void MenuLogic::PressLeft() {
	for(auto& func: buttons[Left]) {
		func.pressed = true;
		if(func.onPress) {
			trigger(func);
		}
	}
}
void MenuLogic::PressRight() {
	for(auto& func: buttons[Right]) {
		func.pressed = true;
		if(func.onPress) {
			trigger(func);
		}
	}
}
void MenuLogic::ReleaseLeft() {
	for(auto& func: buttons[Left]) {
		if(func.pressed && !func.onPress) {
			trigger(func);
		}
		func.pressed = false;
	}
}
void MenuLogic::ReleaseRight() {
	for(auto& func: buttons[Right]) {
		if(func.pressed && !func.onPress) {
			trigger(func);
		}
		func.pressed = false;
	}
}
