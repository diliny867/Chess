#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>
#include <unordered_map>

#include "PieceRender.h"
#include "Chess.h"


class ChessRenderer {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	int squareSize = 80;

	SDL_Point screenSize = {640, 640};

	struct Mouse {
		struct {
			i32 x;
			i32 y;
		} pos, boardRel; //actual position and relative to board
		Chess::XY pieceXY;
		bool left;
		bool right;
		bool middle;
	} mouse ={{0,0},{0,0},{0,0},false,false,false};

	PieceRender pawnRender;
	PieceRender bishopRender;
	PieceRender knightRender;
	PieceRender rookRender;
	PieceRender kingRender;
	PieceRender queenRender;

	bool renderOtherColorMoves = true;

	bool flipBoard = false;
	//bool boardFlipped = false;

	enum PieceColor {
		White,
		Black
	};
	u8 playerColorCount = 2;
	u8 currPlayerColorId = 0;
	std::unordered_map<Chess::ColorId, PieceColor> pieceColors = {{0, White}, {1, Black}};

	bool pieceSelected = false;
	Chess::XY selectedPieceXY = {0,0};
	std::vector<Chess::XY> possibleMoves;
	bool mouseLeftSquare = false;
	i32 selectionCount = 0;

	struct PromotionData{
		bool promotion;
		Chess::XY xy;
		struct Choice {
			Chess::XY xy;
			Chess::PieceType type;
		};
		std::array<Choice,4> centers = {};
	} promotionData = {false, {0,0}};

	void renderPieceAt(const Chess::Piece& piece, SDL_Point position, SDL_Point size) {
		//Chess::ColorId color = boardFlipped ? piece.colorId : (piece.colorId == Chess::ColorIds[0] ? Chess::ColorIds[1] : Chess::ColorIds[0]);
		const Chess::ColorId color = pieceColors[piece.colorId];
		switch(piece.type) {
		case Chess::Pawn:
			pawnRender.position = position;
			pawnRender.size = size;
			pawnRender.Render(renderer, color);
			break;
		case Chess::Bishop:
			bishopRender.position = position;
			bishopRender.size = size;
			bishopRender.Render(renderer, color);
			break;
		case Chess::Knight:
			knightRender.position = position;
			knightRender.size = size;
			knightRender.Render(renderer, color);
			break;
		case Chess::Rook:
			rookRender.position = position;
			rookRender.size = size;
			rookRender.Render(renderer, color);
			break;
		case Chess::King:
			kingRender.position = position;
			kingRender.size = size;
			kingRender.Render(renderer, color);
			break;
		case Chess::Queen:
			queenRender.position = position;
			queenRender.size = size;
			queenRender.Render(renderer, color);
			break;
		default:
			break;
		}
	}
	void renderPiece(const Chess::Piece& piece, Chess::XY boardPosition, SDL_Point size) {
		const SDL_Point position = {boardPosition.x*squareSize, (7 - boardPosition.y)*squareSize};
		renderPieceAt(piece, position, size);
	}
	void renderSelectedPiece() {
		if(!mouse.left || !pieceSelected) {
			return;
		}
		const int halfSquare = squareSize*0.5;
		renderPieceAt(GetPiece(selectedPieceXY.x,selectedPieceXY.y), {mouse.pos.x-halfSquare, mouse.pos.y-halfSquare}, {squareSize,squareSize});
	}
	void renderSquare(SDL_Point position, SDL_Point size, i8 r, i8 g, i8 b, i8 a) {
		boxRGBA(renderer, position.x, position.y, position.x+size.x, position.y+size.y, r, g, b, a);
	}
	void renderBoardSquare(Chess::XY boardPosition, SDL_Point size, i8 r, i8 g, i8 b, i8 a) {
		i16 positionX = boardPosition.x*size.x;
		i16 positionY = (7 - boardPosition.y)*size.y;
		//boxRGBA(renderer, positionX, positionY, positionX+size.x, positionY+size.y, r, g, b, a);
		renderSquare({positionX, positionY}, size, r, g, b, a);
	}
	void renderBoardCircle(Chess::XY boardPosition, SDL_Point size, i32 rad, i8 r, i8 g, i8 b, i8 a) {
		i16 positionX = boardPosition.x*size.x+size.x*0.5;
		i16 positionY = (7 - boardPosition.y)*size.y+size.y*0.5;
		filledCircleRGBA(renderer, positionX, positionY, rad, r, g, b, a);
	}
	void renderSquareSideNames(){
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
	void renderPossibleMoves() {
		Chess::XY pieceXY = mouse.pieceXY;
		if(promotionData.promotion) {
			for(auto& center: promotionData.centers) {
				if(center.xy == pieceXY) {
					//boxRGBA(renderer, pieceXY.x*squareSize, pieceXY.y*squareSize, pieceXY.x*squareSize+squareSize, pieceXY.y*squareSize+squareSize, 0x74, 0x8c, 0x6f, SDL_ALPHA_OPAQUE);
					renderBoardSquare({pieceXY.x, pieceXY.y}, {squareSize, squareSize}, 0x74, 0x8c, 0x6f, SDL_ALPHA_OPAQUE);
					break;
				}
			}
			return;
		}
		if(!pieceSelected || !renderOtherColorMoves && GetPiece(selectedPieceXY.x,selectedPieceXY.y).colorId != getCurrPlayerColor()) {
			return;
		}

		for(auto& move: possibleMoves) {
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
	void renderBackgroundAndPieces() {
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

				if(!mouse.left || selectedPieceXY.x != x || selectedPieceXY.y != y) {
					//renderPiece(GetPiece(x,y), {rect.x, rect.y}, {squareSize,squareSize});
					renderPiece(GetPiece(x,y), {x, y}, {squareSize,squareSize});
				}
			}
		}
	}
	void renderPromotionChoice() {
		//boxRGBA(renderer, 0, 0, screenSize.x, screenSize.y, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE*0.3);
		renderSquare({0, 0}, {screenSize.x, screenSize.y}, 0x26, 0x26, 0x26, SDL_ALPHA_OPAQUE*0.3);
		for(i8 i=0; i<4; i++) {
			promotionData.centers[i].xy.x = promotionData.xy.x;
			promotionData.centers[i].xy.y = promotionData.xy.y+i*(promotionData.xy.y == 0 ? 1 : -1);
		}
		promotionData.centers[0].type = Chess::Queen;
		promotionData.centers[1].type = Chess::Knight;
		promotionData.centers[2].type = Chess::Rook;
		promotionData.centers[3].type = Chess::Bishop;
		renderPiece(Chess::NewPiece(promotionData.centers[0].type, getCurrPlayerColor()), promotionData.centers[0].xy, {squareSize,squareSize});
		renderPiece(Chess::NewPiece(promotionData.centers[1].type, getCurrPlayerColor()), promotionData.centers[1].xy, {squareSize,squareSize});
		renderPiece(Chess::NewPiece(promotionData.centers[2].type, getCurrPlayerColor()), promotionData.centers[2].xy, {squareSize,squareSize});
		renderPiece(Chess::NewPiece(promotionData.centers[3].type, getCurrPlayerColor()), promotionData.centers[3].xy, {squareSize,squareSize});
	}
	void present() {
		SDL_RenderPresent(renderer);
	}

	Chess::ColorId getCurrPlayerColor() {
		return pieceColors[currPlayerColorId];
	}
	void switchPlayer() {
		currPlayerColorId++;
		currPlayerColorId %= playerColorCount;
		if(flipBoard) {
			//boardFlipped = !boardFlipped;
			chess.FlipBoard();
		}
	}
	void selectPromotion() {
		Chess::XY pieceXY = mouse.pieceXY;
		auto promotionIt = std::find_if(promotionData.centers.begin(), promotionData.centers.end(), 
			[pieceXY](PromotionData::Choice& in){return in.xy == pieceXY; });
		if(promotionIt != promotionData.centers.end()) {
			chess.SetPiece(promotionData.xy.x, promotionData.xy.y, Chess::NewPiece(promotionIt->type, currPlayerColorId));
			promotionData.promotion = false;
			switchPlayer();
		}
	}
	bool canPlace(i8 x, i8 y) {
		return GetPiece(x, y).colorId == getCurrPlayerColor();
	}
	void selectPiece() {
		selectionCount++;
		pieceSelected = true;
	}
	void deselectPiece() {
		pieceSelected = false;
		selectionCount = 0;
	}
	void choosePiece() {
		Chess::XY pieceXY = mouse.pieceXY;

		if(selectedPieceXY != pieceXY) {
			deselectPiece();
		}
		if(pieceSelected && GetPiece(pieceXY.x,pieceXY.y).type == Chess::Empty) {
			deselectPiece();
			possibleMoves.clear();
		}else {
			selectPiece();
			possibleMoves = chess.GetAllValidTurns(pieceXY.x, pieceXY.y);
		}
		selectedPieceXY = pieceXY;
	}
	bool placePiece() {
		Chess::XY pieceXY = mouse.pieceXY;

		if(std::find(possibleMoves.begin(), possibleMoves.end(), pieceXY) != possibleMoves.end() && canPlace(selectedPieceXY.x,selectedPieceXY.y)) {
			auto isMoveCastleRes = chess.GetMoveSpecialResults(selectedPieceXY.x, selectedPieceXY.y, pieceXY.x, pieceXY.y);
			chess.MovePiece(selectedPieceXY.x, selectedPieceXY.y, pieceXY.x, pieceXY.y);
			switch(isMoveCastleRes.type) {
			case Chess::MoveResults::Promotion:
				promotionData.promotion = true;
				promotionData.xy = pieceXY;
				possibleMoves.clear();
				deselectPiece();
				return false;
			case Chess::MoveResults::Castle:
				chess.MovePiece(isMoveCastleRes.castleData.rookFromXY.x, isMoveCastleRes.castleData.rookFromXY.y, isMoveCastleRes.castleData.rookToXY.x, isMoveCastleRes.castleData.rookToXY.y);
				break;
			case Chess::MoveResults::EnPassant:
				chess.SetPiece(isMoveCastleRes.enPassandData.xy.x, isMoveCastleRes.enPassandData.xy.y, Chess::NewPiece(Chess::Empty, 0));
				break;
			default: 
				break;
			}
			possibleMoves.clear();

			deselectPiece();
			switchPlayer();
			return true;
		}
		return false;
	}
	Chess chess;
public:
	void Init() {
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

		chess.Reset();
	}
	Chess::Piece& GetPiece(i8 x, i8 y) {
		return chess.GetPiece(x, y);
	}
	void MouseDown(const SDL_MouseButtonEvent& mouseButton) {
		mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? true : mouse.left;
		mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? true : mouse.right;
		mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? true : mouse.middle;

		if(mouseButton.button == SDL_BUTTON_LEFT) {
			if(promotionData.promotion) {
				selectPromotion();
			}else if(!placePiece()) {
				mouseLeftSquare = false;
				choosePiece();
			}
		}
		if(mouseButton.button == SDL_BUTTON_RIGHT) {
			deselectPiece();
		}
	}
	void MouseUp(const SDL_MouseButtonEvent& mouseButton) {
		mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? false : mouse.left;
		mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? false : mouse.right;
		mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? false : mouse.middle;

		if(mouseButton.button == SDL_BUTTON_LEFT && pieceSelected) {
			if(!placePiece()) {
				if(mouseLeftSquare || selectionCount > 1){
					deselectPiece();
				}
			}
		}
	}
	void MouseMotion(const SDL_MouseMotionEvent& mouseMotion) {
		mouse.pos.x = mouseMotion.x;
		mouse.pos.y = mouseMotion.y;
		mouse.boardRel.x = mouse.pos.x;
		mouse.boardRel.y = screenSize.y - mouse.pos.y;
		//mouse.boardRel.y = mouse.pos.y;

		mouse.pieceXY.x = static_cast<i8>(mouse.boardRel.x/squareSize);
		mouse.pieceXY.y = static_cast<i8>(mouse.boardRel.y/squareSize);

		if(mouse.pieceXY.x != selectedPieceXY.x || mouse.pieceXY.y != selectedPieceXY.y) {
			mouseLeftSquare = true;
		}
	}

	void Render() {
		renderBackgroundAndPieces();
		renderPossibleMoves();
		renderSquareSideNames();
		renderSelectedPiece();
		if(promotionData.promotion) {
			renderPromotionChoice();
		}
		present();
	}
};
