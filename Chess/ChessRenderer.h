#pragma once


#include <SDL2/SDL.h>

#include "PieceRender.h"
#include "ChessBase.h"

#include "GameSectionRenderer.h"


class ChessRenderer: public GameSectionRenderer{
private:
	class ChessLogic* chess = nullptr;

	PieceRender pawnRender;
	PieceRender bishopRender;
	PieceRender knightRender;
	PieceRender rookRender;
	PieceRender kingRender;
	PieceRender queenRender;

	bool renderOtherColorMoves = true;

	void renderPieceAt(const ChessBase::Piece& piece, SDL_Point position, SDL_Point size);
	void renderPiece(const ChessBase::Piece& piece, ChessBase::XY boardPosition, SDL_Point size);
	void renderSelectedPiece();
	void renderBoardSquare(ChessBase::XY boardPosition, SDL_Point size, i8 r, i8 g, i8 b, i8 a);
	void renderBoardCircle(ChessBase::XY boardPosition, SDL_Point size, i32 rad, i8 r, i8 g, i8 b, i8 a);
	void renderSquareSideNames();
	void renderPossibleMoves();
	void renderBackgroundAndPieces();
	void renderPromotionChoice();
public:
	void Init(class Game* game, SDL_Point size) override;

	void Render(SDL_Renderer* renderer, SDL_Point position, SDL_Point size) override;
};
