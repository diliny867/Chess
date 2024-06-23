#include "MenuLogic.h"

#include "Game.h"

#define AS(a, b) reinterpret_cast<b>(a)
static void trigger(MenuLogic::FuncionData& fd) {
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
		{20,20},
		{60,60},
		new CheckBoxMetadata({
			{Checkbox, {true, "Enables/Disables board flip on turn switch"}},
			this->game->chessRenderer.FlipBoardOnTurn()
		}),
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
		{20,100},
		{60,60},
		new CheckBoxMetadata({
			{Click, {true, "Flips board"}},
			this->game->chessRenderer.IsBoardFlipped()
		}),
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
		func.triggerPressed = true;
		if(func.triggerOnPress) {
			trigger(func);
		}
	}
}
void MenuLogic::PressRight() {
	for(auto& func: buttons[Right]) {
		func.triggerPressed = true;
		if(func.triggerOnPress) {
			trigger(func);
		}
	}
}
void MenuLogic::ReleaseLeft() {
	for(auto& func: buttons[Left]) {
		if(func.triggerPressed && !func.triggerOnPress) {
			trigger(func);
		}
		func.triggerPressed = false;
	}
}
void MenuLogic::ReleaseRight() {
	for(auto& func: buttons[Right]) {
		if(func.triggerPressed && !func.triggerOnPress) {
			trigger(func);
		}
		func.triggerPressed = false;
	}
}
