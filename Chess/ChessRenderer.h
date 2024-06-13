#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "PieceRender.h"
#include "ChessBase.h"


class ChessRenderer {
private:
	class ChessLogic* chess = nullptr;

	SDL_Renderer* renderer = nullptr;

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
	void renderSquare(SDL_Point position, SDL_Point size, i8 r, i8 g, i8 b, i8 a);
	void renderBoardSquare(ChessBase::XY boardPosition, SDL_Point size, i8 r, i8 g, i8 b, i8 a);
	void renderBoardCircle(ChessBase::XY boardPosition, SDL_Point size, i32 rad, i8 r, i8 g, i8 b, i8 a);
	void renderCharacter(SDL_Point position, f32 scale, char c, i8 r, i8 g, i8 b, i8 a);
	void renderSquareSideNames();
	void renderPossibleMoves();
	void renderBackgroundAndPieces();
	void renderPromotionChoice();
	void present();
public:
	void Init(class Game* game);

	void Render();
};
