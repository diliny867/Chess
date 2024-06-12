#include "ChessRenderer.h"

#include <iostream>
#include <unordered_map>

#include "ChessLogic.h"
#include "RenderSettings.h"


void ChessRenderer::Init(ChessLogic* chessLogic){
	 chess = chessLogic;
	 if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		 std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	 }

	 window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y, SDL_WINDOW_SHOWN);
	 if(window == nullptr) {
		 std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	 }

	 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	 if (renderer == nullptr) {
		 std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
	 }

	 SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

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
	switch(piece.type) {
	case ChessBase::Pawn:
		pawnRender.position = position;
		pawnRender.size = size;
		pawnRender.Render(renderer, color);
		break;
	case ChessBase::Bishop:
		bishopRender.position = position;
		bishopRender.size = size;
		bishopRender.Render(renderer, color);
		break;
	case ChessBase::Knight:
		knightRender.position = position;
		knightRender.size = size;
		knightRender.Render(renderer, color);
		break;
	case ChessBase::Rook:
		rookRender.position = position;
		rookRender.size = size;
		rookRender.Render(renderer, color);
		break;
	case ChessBase::King:
		kingRender.position = position;
		kingRender.size = size;
		kingRender.Render(renderer, color);
		break;
	case ChessBase::Queen:
		queenRender.position = position;
		queenRender.size = size;
		queenRender.Render(renderer, color);
		break;
	default:
		break;
	}
}
void ChessRenderer::renderPiece(const ChessBase::Piece& piece, ChessBase::XY boardPosition, SDL_Point size) {
	const SDL_Point position = {boardPosition.x*squareSize, (7 - boardPosition.y)*squareSize};
	renderPieceAt(piece, position, size);
}
void ChessRenderer::renderSelectedPiece() {
	if(!chess->mouse.left || !chess->pieceSelected) {
		return;
	}
	const int halfSquare = squareSize*0.5;
	renderPieceAt(chess->GetPiece(chess->selectedPieceXY.x,chess->selectedPieceXY.y), {chess->mouse.pos.x-halfSquare, chess->mouse.pos.y-halfSquare}, {squareSize,squareSize});
}
void ChessRenderer::renderSquare(SDL_Point position, SDL_Point size, i8 r, i8 g, i8 b, i8 a) {
	boxRGBA(renderer, position.x, position.y, position.x+size.x, position.y+size.y, r, g, b, a);
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
	filledCircleRGBA(renderer, positionX, positionY, rad, r, g, b, a);
}
void ChessRenderer::renderSquareSideNames() {
	const float sideOffset = 0.12;
	const i16 offsetLow = static_cast<i16>(static_cast<float>(squareSize)*sideOffset);
	const i16 offsetHigh = static_cast<i16>(static_cast<float>(squareSize)*(1-sideOffset));
	const float scale = 1.25;
	const i16 gfxPrimitivesCharOffset = 3;
	i16 coordNumberX = offsetLow - gfxPrimitivesCharOffset;
	i16 coordNumberY = static_cast<i16>(squareSize*8) - offsetHigh - gfxPrimitivesCharOffset;
	i16 coordLetterX = offsetHigh - gfxPrimitivesCharOffset;
	i16 coordLetterY = static_cast<i16>(squareSize*8) - offsetLow - gfxPrimitivesCharOffset;
	SDL_RenderSetScale(renderer, scale, scale);
	for(i8 i=0; i<8; i++) {
		characterRGBA(renderer,coordNumberX/scale,coordNumberY/scale,'1'+i,0x26,0x26,0x26,SDL_ALPHA_OPAQUE);
		characterRGBA(renderer,coordLetterX/scale,coordLetterY/scale,'a'+i,0x26,0x26,0x26,SDL_ALPHA_OPAQUE);
		coordNumberY -= squareSize;
		coordLetterX += squareSize;
	}
	SDL_RenderSetScale(renderer, 1, 1);
}
void ChessRenderer::renderPossibleMoves() {
	ChessBase::XY pieceXY = chess->getMousePiece();
	if(chess->promotionData.promotion) {
		for(auto& center: chess->promotionData.centers) {
			if(center.xy == pieceXY) {
				//boxRGBA(renderer, pieceXY.x*squareSize, pieceXY.y*squareSize, pieceXY.x*squareSize+squareSize, pieceXY.y*squareSize+squareSize, 0x74, 0x8c, 0x6f, SDL_ALPHA_OPAQUE);
				renderBoardSquare({pieceXY.x, pieceXY.y}, {squareSize, squareSize}, 0x74, 0x8c, 0x6f, SDL_ALPHA_OPAQUE);
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
			renderBoardSquare({pieceXY.x, pieceXY.y}, {squareSize, squareSize}, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
		} else{
			//filledCircleRGBA(renderer, move.x*squareSize+squareSize*0.5, move.y*squareSize+squareSize*0.5, squareSize/8, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.5);
			renderBoardCircle({move.x, move.y}, {squareSize,squareSize}, squareSize/7, 0x71, 0x81, 0x68, SDL_ALPHA_OPAQUE*0.9);
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
				renderBoardSquare({x, y}, {squareSize, squareSize}, 0xee, 0xee, 0xd2, SDL_ALPHA_OPAQUE);
			}else {
				//SDL_SetRenderDrawColor(renderer, 0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
				//boxRGBA(renderer, x*squareSize, y*squareSize, x*squareSize+squareSize, y*squareSize+squareSize, 0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
				renderBoardSquare({x, y}, {squareSize, squareSize},0x76, 0x96, 0x56, SDL_ALPHA_OPAQUE);
			}
			//SDL_RenderFillRect(renderer, &rect);

			if(!chess->mouse.left || chess->selectedPieceXY.x != x || chess->selectedPieceXY.y != y) {
				//renderPiece(GetPiece(x,y), {rect.x, rect.y}, {squareSize,squareSize});
				renderPiece(chess->GetPiece(x,y), {x, y}, {squareSize,squareSize});
			}
		}
	}
}
void ChessRenderer::renderPromotionChoice() {
	//boxRGBA(renderer, 0, 0, screenSize.x, screenSize.y, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE*0.3);
	renderSquare({0, 0}, {screenSize.x, screenSize.y}, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE*0.3);
	for(i32 i=0; i<4; i++) {
		chess->promotionData.centers[i].xy.x = chess->promotionData.xy.x;
		chess->promotionData.centers[i].xy.y = chess->promotionData.xy.y+i*(chess->promotionData.xy.y == 0 ? 1 : -1);
	}
	chess->promotionData.centers[0].type = ChessBase::Queen;
	chess->promotionData.centers[1].type = ChessBase::Knight;
	chess->promotionData.centers[2].type = ChessBase::Rook;
	chess->promotionData.centers[3].type = ChessBase::Bishop;
	for(i32 i=0; i<4; i++) {
		renderPiece(ChessBase::NewPiece(chess->promotionData.centers[i].type, chess->currPlayerColorId), chess->promotionData.centers[i].xy, {squareSize,squareSize});
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
