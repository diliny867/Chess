#include "ChessRenderer.h"

#include <iostream>
#include <unordered_map>

#include "Game.h"


void ChessRenderer::Init(Game* game){
	chess = game->GetChessLogic();
	renderer = game->GetRenderer();

	pawnRender.Init(renderer, "res/black-pawn.png", "res/white-pawn.png");
	bishopRender.Init(renderer, "res/black-bishop.png", "res/white-bishop.png");
	knightRender.Init(renderer, "res/black-knight.png", "res/white-knight.png");
	rookRender.Init(renderer, "res/black-rook.png", "res/white-rook.png");
	kingRender.Init(renderer, "res/black-king.png", "res/white-king.png");
	queenRender.Init(renderer, "res/black-queen.png", "res/white-queen.png");

}

void ChessRenderer::renderPieceAt(const ChessBase::Piece& piece, SDL_Point position, SDL_Point size) {
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
	pieceRender->Render(renderer, color);
}
void ChessRenderer::renderPiece(const ChessBase::Piece& piece, ChessBase::XY boardPosition, SDL_Point size) {
	const SDL_Point position = {Game::boardPosition.x + boardPosition.x*Game::squareSize, Game::boardPosition.y +  (7 - boardPosition.y)*Game::squareSize};
	renderPieceAt(piece, position, size);
}
void ChessRenderer::renderSelectedPiece() {
	if(!Game::mouse.left || !chess->pieceSelected) {
		return;
	}
	const int halfSquare = Game::squareSize*0.5;
	renderPieceAt(chess->GetPiece(chess->selectedPieceXY.x,chess->selectedPieceXY.y), {Game::mouse.pos.x-halfSquare, Game::mouse.pos.y-halfSquare}, {Game::squareSize,Game::squareSize});
}
void ChessRenderer::renderSquare(SDL_Point position, SDL_Point size, i8 r, i8 g, i8 b, i8 a) {
	boxRGBA(renderer, Game::boardPosition.x+position.x, Game::boardPosition.y+position.y, Game::boardPosition.x+position.x+size.x, Game::boardPosition.y+position.y+size.y, r, g, b, a);
}
void ChessRenderer::renderBoardSquare(ChessBase::XY boardPosition, SDL_Point size, i8 r, i8 g, i8 b, i8 a) {
	i16 positionX = boardPosition.x*size.x;
	i16 positionY = (7 - boardPosition.y)*size.y;
	//boxRGBA(renderer, positionX, positionY, positionX+size.x, positionY+size.y, r, g, b, a);
	renderSquare({positionX, positionY}, size, r, g, b, a);
}
void ChessRenderer::renderBoardCircle(ChessBase::XY boardPosition, SDL_Point size, i32 rad, i8 r, i8 g, i8 b, i8 a) {
	i16 positionX = boardPosition.x*size.x+size.x*0.5;
	i16 positionY = (7 - boardPosition.y)*size.y+size.y*0.5;
	filledCircleRGBA(renderer, Game::boardPosition.x+positionX, Game::boardPosition.y+positionY, rad, r, g, b, a);
}
void ChessRenderer::renderCharacter(SDL_Point position, f32 scale, char c, i8 r, i8 g, i8 b, i8 a) {
	SDL_RenderSetScale(renderer, scale, scale);
	characterRGBA(renderer, (Game::boardPosition.x + position.x)/scale, (Game::boardPosition.y + position.y)/scale, c, r, g, b, a);
	SDL_RenderSetScale(renderer, 1, 1);
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
		renderCharacter({coordNumberX,coordNumberY}, scale, '1'+i, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE);
		renderCharacter({coordLetterX,coordLetterY}, scale, 'a'+i, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE);
		coordNumberY -= Game::squareSize;
		coordLetterX += Game::squareSize;
	}
}
void ChessRenderer::renderPossibleMoves() {
	ChessBase::XY pieceXY = Game::GetMousePiece();
	if(chess->promotionData.promotion) {
		for(auto& center: chess->promotionData.centers) {
			if(center.xy == pieceXY) {
				//boxRGBA(renderer, pieceXY.x*squareSize, pieceXY.y*squareSize, pieceXY.x*squareSize+squareSize, pieceXY.y*squareSize+squareSize, 0x74, 0x8c, 0x6f, SDL_ALPHA_OPAQUE);
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
		//SDL_SetRenderDrawColor(renderer, 0x6f, 0xbb, 0xe1, SDL_ALPHA_OPAQUE);
		if(pieceXY.x == move.x && pieceXY.y == move.y) {
			//SDL_Rect rect = {mouseSquareX*squareSize, mouseSquareY*squareSize, squareSize, squareSize};
			//SDL_SetRenderDrawColor(renderer, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
			//SDL_RenderFillRect(renderer, &rect);
			//boxRGBA(renderer, pieceXY.x*squareSize, pieceXY.y*squareSize, pieceXY.x*squareSize+squareSize, pieceXY.y*squareSize+squareSize, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
			renderBoardSquare({pieceXY.x, pieceXY.y}, {Game::squareSize,Game::squareSize}, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
		} else{
			//filledCircleRGBA(renderer, move.x*squareSize+squareSize*0.5, move.y*squareSize+squareSize*0.5, squareSize/8, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
			renderBoardCircle({move.x, move.y}, {Game::squareSize,Game::squareSize},Game::squareSize/7, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.9);
		}
	}
}
void ChessRenderer::renderBackgroundAndPieces() {
	for(i8 y=0; y<8; y++) {
		for(i8 x=0; x<8;x++) {
			//SDL_Rect rect = {x*squareSize, y*squareSize, squareSize, squareSize};
			if((x+y)%2) {
				//SDL_SetRenderDrawColor(renderer, 0xee, 0xee, 0xd2, SDL_ALPHA_OPAQUE);
				//boxRGBA(renderer, x*squareSize, y*squareSize, x*squareSize+squareSize, y*squareSize+squareSize, 0xee, 0xee, 0xd2, SDL_ALPHA_OPAQUE);
				renderBoardSquare({x, y}, {Game::squareSize,Game::squareSize}, 0xee, 0xee, 0xd2, SDL_ALPHA_OPAQUE);
			}else {
				//SDL_SetRenderDrawColor(renderer, 0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
				//boxRGBA(renderer, x*squareSize, y*squareSize, x*squareSize+squareSize, y*squareSize+squareSize, 0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
				renderBoardSquare({x, y}, {Game::squareSize,Game::squareSize},0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
			}
			//SDL_RenderFillRect(renderer, &rect);

			if(!Game::mouse.left || chess->selectedPieceXY.x != x || chess->selectedPieceXY.y != y) {
				//renderPiece(GetPiece(x,y), {rect.x, rect.y}, {squareSize,squareSize});
				renderPiece(chess->GetPiece(x,y), {x, y}, {Game::squareSize,Game::squareSize});
			}
		}
	}
}
void ChessRenderer::renderPromotionChoice() {
	//boxRGBA(renderer, 0, 0, screenSize.x, screenSize.y, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE*0.3);
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
void ChessRenderer::present() {
	SDL_RenderPresent(renderer);
}

void ChessRenderer::Render() {
	renderBackgroundAndPieces();
	renderPossibleMoves();
	renderSquareSideNames();
	renderSelectedPiece();
	if(chess->promotionData.promotion) {
		renderPromotionChoice();
	}
	present();
}
