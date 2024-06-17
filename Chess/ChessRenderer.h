#pragma once


#include <SDL2/SDL.h>

#include "PieceRender.h"

#include "GameSectionRenderer.h"
#include "ChessBase.h"


class ChessRenderer: public GameSectionRenderer{
private:
	class ChessLogic* chess = nullptr;

	PieceRender pawnRender;
	PieceRender bishopRender;
	PieceRender knightRender;
	PieceRender rookRender;
	PieceRender kingRender;
	PieceRender queenRender;

	bool boardFlipped = false;

	bool flipBoard = true;
	bool renderOtherColorMoves = true;

	void renderPieceAt(const ChessBase::Piece& piece, i32Vec2 position, i32Vec2 size);
	void renderPiece(const ChessBase::Piece& piece, ChessBase::XY onBoardPosition, i32Vec2 size);
	void renderSelectedPiece();
	void renderBoardSquare(ChessBase::XY onBoardPosition, i32Vec2 size, i8 r, i8 g, i8 b, i8 a);
	void renderBoardCircle(ChessBase::XY onBoardPosition, i32Vec2 size, i32 rad, i8 r, i8 g, i8 b, i8 a);
	void renderSquareSideNames();
	void renderPossibleMoves();
	void renderBackgroundAndPieces();
	void renderPromotionChoice();
public:
	void Init(i32Vec2 size) override;

	void SetFlipBoardOnTurn(bool flip);
	bool FlipBoardOnTurn();
	void FlipBoard();
	bool IsBoardFlipped();

	void Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) override;
};
