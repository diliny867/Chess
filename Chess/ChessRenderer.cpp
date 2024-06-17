#include "ChessRenderer.h"

#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>
#include <unordered_map>

#include "Game.h"


void ChessRenderer::Init(i32Vec2 size){
	Game* game = Game::GetCurrentGame();
	init(game->GetRenderer(),size);
	chess = game->GetChessLogic();

	pawnRender.Init(currRenderer, "res/black-pawn.png", "res/white-pawn.png");
	bishopRender.Init(currRenderer, "res/black-bishop.png", "res/white-bishop.png");
	knightRender.Init(currRenderer, "res/black-knight.png", "res/white-knight.png");
	rookRender.Init(currRenderer, "res/black-rook.png", "res/white-rook.png");
	kingRender.Init(currRenderer, "res/black-king.png", "res/white-king.png");
	queenRender.Init(currRenderer, "res/black-queen.png", "res/white-queen.png");
}

void ChessRenderer::SetFlipBoardOnTurn(bool flip) {
	flipBoard = flip;
}
bool ChessRenderer::FlipBoardOnTurn() {
	return flipBoard;
}
void ChessRenderer::FlipBoard() {
	boardFlipped = !boardFlipped;
}
bool ChessRenderer::IsBoardFlipped() {
	return boardFlipped;
}

void ChessRenderer::renderPieceAt(const ChessBase::Piece& piece, i32Vec2 position, i32Vec2 size) {
	//Chess::ColorId color = boardFlipped ? piece.colorId : (piece.colorId == Chess::ColorIds[0] ? Chess::ColorIds[1] : Chess::ColorIds[0]);
	const PieceRender::PieceColor color = chess->pieceColors[piece.colorId];
	PieceRender* pieceRender;
	switch(piece.type) {
	case ChessBase::Pawn:
		pieceRender = &pawnRender;
		break;
	case ChessBase::Bishop:
		pieceRender = &bishopRender;
		break;
	case ChessBase::Knight:
		pieceRender = &knightRender;
		break;
	case ChessBase::Rook:
		pieceRender = &rookRender;
		break;
	case ChessBase::King:
		pieceRender = &kingRender;
		break;
	case ChessBase::Queen:
		pieceRender = &queenRender;
		break;
	default:
		return;
	}
	pieceRender->position = position;
	pieceRender->size = size;
	pieceRender->Render(currRenderer, color);
}
void ChessRenderer::renderPiece(const ChessBase::Piece& piece, ChessBase::XY onBoardPosition, i32Vec2 size) {
	const i16 y = IsBoardFlipped() ? onBoardPosition.y : (7 - onBoardPosition.y);
	const i32Vec2 position = {onBoardPosition.x*Game::squareSize, y*Game::squareSize};
	renderPieceAt(piece, position, size);
}
void ChessRenderer::renderSelectedPiece() {
	if(!Game::mouse.left || !chess->pieceSelected) {
		return;
	}
	const int halfSquare = Game::squareSize*0.5;
	const auto mousePos = Game::GetMouseRelative(Game::boardPosition);
	renderPieceAt(chess->GetPiece(chess->selectedPieceXY.x,chess->selectedPieceXY.y), {mousePos.x-halfSquare, mousePos.y-halfSquare}, {Game::squareSize,Game::squareSize});
}
void ChessRenderer::renderBoardSquare(ChessBase::XY onBoardPosition, i32Vec2 size, i8 r, i8 g, i8 b, i8 a) {
	const i16 positionX = onBoardPosition.x;
	const i16 positionY = IsBoardFlipped() ? onBoardPosition.y : (7 - onBoardPosition.y);
	renderSquare({positionX*size.x, positionY*size.y}, size, r, g, b, a);
}
void ChessRenderer::renderBoardCircle(ChessBase::XY onBoardPosition, i32Vec2 size, i32 rad, i8 r, i8 g, i8 b, i8 a) {
	const i16 positionX = onBoardPosition.x;
	const i16 positionY = IsBoardFlipped() ? onBoardPosition.y : (7 - onBoardPosition.y);
	renderCircle({positionX*size.x+size.x/2, positionY*size.y+size.y/2}, rad, r, g, b, a);
}
void ChessRenderer::renderSquareSideNames() {
	const float sideOffset = 0.12;
	const i16 offsetLow = static_cast<i16>(static_cast<float>(Game::squareSize)*sideOffset);
	const i16 offsetHigh = static_cast<i16>(static_cast<float>(Game::squareSize)*(1-sideOffset));
	const float scale = 1.25;
	const i16 gfxPrimitivesCharOffset = 3;
	i16 coordNumberX = offsetLow - gfxPrimitivesCharOffset;
	i16 coordNumberY = static_cast<i16>(Game::squareSize*8) - offsetHigh - gfxPrimitivesCharOffset;
	i16 coordLetterX = offsetHigh - gfxPrimitivesCharOffset;
	i16 coordLetterY = static_cast<i16>(Game::squareSize*8) - offsetLow - gfxPrimitivesCharOffset;
	
	for(i8 i=0; i<8; i++) {
		renderCharacter({coordNumberX,coordNumberY}, scale, IsBoardFlipped() ? '8'-i : '1'+i, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE);
		renderCharacter({coordLetterX,coordLetterY}, scale, 'a'+i, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE);
		coordNumberY -= Game::squareSize;
		coordLetterX += Game::squareSize;
	}
}
void ChessRenderer::renderPossibleMoves() {
	ChessBase::XY pieceXY = Game::GetCurrentGame()->GetMousePiece();
	if(chess->promotionData.promotion) {
		for(auto& center: chess->promotionData.centers) {
			if(center.xy == pieceXY) {
				renderBoardSquare({pieceXY.x, pieceXY.y}, {Game::squareSize,Game::squareSize}, 0x74, 0x8c, 0x6f, SDL_ALPHA_OPAQUE);
				break;
			}
		}
		return;
	}
	if(!chess->pieceSelected || !renderOtherColorMoves && chess->GetPiece(chess->selectedPieceXY.x,chess->selectedPieceXY.y).colorId != chess->currPlayerColorId) {
		return;
	}

	for(auto& move: chess->possibleMoves) {
		if(pieceXY.x == move.x && pieceXY.y == move.y) {
			renderBoardSquare({pieceXY.x, pieceXY.y}, {Game::squareSize,Game::squareSize}, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
		} else{
			renderBoardCircle({move.x, move.y}, {Game::squareSize,Game::squareSize},Game::squareSize/7, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.9);
		}
	}
}
void ChessRenderer::renderBackgroundAndPieces() {
	for(i8 y=0; y<8; y++) {
		for(i8 x=0; x<8;x++) {
			if((x+y)%2) {
				renderBoardSquare({x, y}, {Game::squareSize,Game::squareSize}, 0xee, 0xee, 0xd2, SDL_ALPHA_OPAQUE);
			}else {
				renderBoardSquare({x, y}, {Game::squareSize,Game::squareSize},0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
			}

			if(!Game::mouse.left || chess->selectedPieceXY.x != x || chess->selectedPieceXY.y != y || Game::mouse.left && !chess->pieceSelected) {
				renderPiece(chess->GetPiece(x,y), {x, y}, {Game::squareSize,Game::squareSize});
			}
		}
	}
}
void ChessRenderer::renderPromotionChoice() {
	renderSquare({0, 0}, {Game::boardSize.x,Game::boardSize.y}, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE*0.3);
	for(i32 i=0; i<4; i++) {
		chess->promotionData.centers[i].xy.x = chess->promotionData.xy.x;
		chess->promotionData.centers[i].xy.y = chess->promotionData.xy.y+i*(chess->promotionData.xy.y == 0 ? 1 : -1);
	}
	chess->promotionData.centers[0].type = ChessBase::Queen;
	chess->promotionData.centers[1].type = ChessBase::Knight;
	chess->promotionData.centers[2].type = ChessBase::Rook;
	chess->promotionData.centers[3].type = ChessBase::Bishop;
	for(i32 i=0; i<4; i++) {
		renderPiece(ChessBase::NewPiece(chess->promotionData.centers[i].type, chess->currPlayerColorId), chess->promotionData.centers[i].xy, {Game::squareSize,Game::squareSize});
	}
}

void ChessRenderer::Render(SDL_Renderer* renderer, i32Vec2 position, i32Vec2 size) {
	initRender(renderer);
	renderBackgroundAndPieces();
	renderPossibleMoves();
	renderSquareSideNames();
	renderSelectedPiece();
	if(chess->promotionData.promotion) {
		renderPromotionChoice();
	}
	present(position, size);
}
