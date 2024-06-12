#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>
#include <unordered_map>

#include "PieceRender.h"
#include "ChessBase.h"
#include "ChessRenderer.h"


class ChessLogic {
private:
	friend ChessRenderer;

	struct Mouse {
		struct {
			i32 x;
			i32 y;
		} pos;
		bool left;
		bool right;
		bool middle;
	} mouse = {{0,0},false,false,false};

	bool flipBoard = false;
	//bool boardFlipped = false;
	
	u8 playerColorCount = 2;
	ChessBase::ColorId currPlayerColorId = 0;
	std::unordered_map<ChessBase::ColorId, PieceRender::PieceColor> pieceColors = {{0,PieceRender::White}, {1,PieceRender::Black}};

	bool pieceSelected = false;
	ChessBase::XY selectedPieceXY = {0,0};
	std::vector<ChessBase::XY> possibleMoves;
	bool mouseLeftSquare = false;
	i32 selectionCount = 0;

	struct PromotionData{
		bool promotion;
		ChessBase::XY xy;
		struct Choice {
			ChessBase::XY xy;
			ChessBase::PieceType type;
		};
		std::array<Choice,4> centers = {};
	} promotionData = {false, {0,0}};

	ChessBase::XY getMousePiece();

	ChessBase::ColorId getNextPlayerColorId();
	void switchPlayer();
	void concludeTurn();

	void selectPromotion(ChessBase::XY pieceXY);

	bool canPlace(i8 x, i8 y);

	void selectPiece();
	void deselectPiece();

	void choosePiece(ChessBase::XY pieceXY);
	bool placePiece(ChessBase::XY pieceXY);
	ChessBase chess;
public:
	void Init();

	ChessBase::Piece& GetPiece(i8 x, i8 y);

	void MouseDown(const SDL_MouseButtonEvent& mouseButton);
	void MouseUp(const SDL_MouseButtonEvent& mouseButton);
	void MouseMotion(const SDL_MouseMotionEvent& mouseMotion);
};
