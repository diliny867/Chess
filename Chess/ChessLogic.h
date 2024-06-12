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

	ChessBase::ColorId getNextPlayerColorId();
	void switchPlayer();
	void concludeTurn();

	void selectPromotion(ChessBase::XY pieceXY);
	void choosePiece(ChessBase::XY pieceXY);
	bool placePiece(ChessBase::XY pieceXY);

	bool canPlace(i8 x, i8 y);

	ChessBase chess;
public:
	void Init();

	ChessBase::Piece& GetPiece(i8 x, i8 y);

	void CheckMouseDeselect(ChessBase::XY pieceXY);
	void CheckMouseLeftSquare(ChessBase::XY pieceXY);

	void SelectPiece(ChessBase::XY pieceXY);
	void DeselectPiece();
};
