#include "ChessLogic.h"

#include "RenderSettings.h"


ChessBase::XY ChessLogic::getMousePiece() {
	return {static_cast<i8>(mouse.pos.x/squareSize), static_cast<i8>((screenSize.y - mouse.pos.y)/squareSize)};
}

ChessBase::ColorId ChessLogic::getNextPlayerColorId() {
	return (currPlayerColorId+1) % playerColorCount;
}
void ChessLogic::switchPlayer() {
	currPlayerColorId = getNextPlayerColorId();
	if(flipBoard) {
		//boardFlipped = !boardFlipped;
		chess.FlipBoard();
	}
}
void ChessLogic::concludeTurn() {
	deselectPiece();
	// Check for checkmate
	if(chess.IsMated(getNextPlayerColorId())) {
		std::cout<<"Mated\n";
	} else{
		switchPlayer();
	}
}
void ChessLogic::selectPromotion(ChessBase::XY pieceXY) {
	auto promotionIt = std::find_if(promotionData.centers.begin(), promotionData.centers.end(), 
	                                [pieceXY](PromotionData::Choice& in){return in.xy == pieceXY; });
	if(promotionIt != promotionData.centers.end()) {
		chess.SetPiece(promotionData.xy.x, promotionData.xy.y, ChessBase::NewPiece(promotionIt->type, currPlayerColorId));
		promotionData.promotion = false;
		concludeTurn();
	}
}
bool ChessLogic::canPlace(i8 x, i8 y) {
	return GetPiece(x, y).colorId == currPlayerColorId;
}
void ChessLogic::selectPiece() {
	selectionCount++;
	pieceSelected = true;
}
void ChessLogic::deselectPiece() {
	pieceSelected = false;
	selectionCount = 0;
}
void ChessLogic::choosePiece(ChessBase::XY pieceXY) {
	if(selectedPieceXY != pieceXY) {
		deselectPiece();
	}
	if(pieceSelected && GetPiece(pieceXY.x,pieceXY.y).type == ChessBase::Empty) {
		deselectPiece();
		possibleMoves.clear();
	}else {
		selectPiece();
		possibleMoves = chess.GetAllValidTurns(pieceXY.x, pieceXY.y);
	}
	selectedPieceXY = pieceXY;
}
bool ChessLogic::placePiece(ChessBase::XY pieceXY) {
	if(std::find(possibleMoves.begin(), possibleMoves.end(), pieceXY) != possibleMoves.end() && canPlace(selectedPieceXY.x,selectedPieceXY.y)) {
		auto isMoveCastleRes = chess.GetMoveSpecialResults(selectedPieceXY.x, selectedPieceXY.y, pieceXY.x, pieceXY.y);
		chess.MovePiece(selectedPieceXY.x, selectedPieceXY.y, pieceXY.x, pieceXY.y);
		switch(isMoveCastleRes.type) {
		case ChessBase::MoveResults::Promotion:
			promotionData.promotion = true;
			promotionData.xy = pieceXY;
			possibleMoves.clear();
			deselectPiece();
			return false;
		case ChessBase::MoveResults::Castle:
			chess.MovePiece(isMoveCastleRes.castleData.rookFromXY.x, isMoveCastleRes.castleData.rookFromXY.y, isMoveCastleRes.castleData.rookToXY.x, isMoveCastleRes.castleData.rookToXY.y);
			break;
		case ChessBase::MoveResults::EnPassant:
			chess.SetPiece(isMoveCastleRes.enPassandData.xy.x, isMoveCastleRes.enPassandData.xy.y, ChessBase::NewPiece(ChessBase::Empty, 0));
			break;
		default: 
			break;
		}
		possibleMoves.clear();

		concludeTurn();
		return true;
	}
	return false;
}
void ChessLogic::Init() {
	chess.Reset();
}
ChessBase::Piece& ChessLogic::GetPiece(i8 x, i8 y) {
	return chess.GetPiece(x, y);
}
void ChessLogic::MouseDown(const SDL_MouseButtonEvent& mouseButton) {
	mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? true : mouse.left;
	mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? true : mouse.right;
	mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? true : mouse.middle;

	if(mouseButton.button == SDL_BUTTON_LEFT) {
		if(promotionData.promotion) {
			selectPromotion(getMousePiece());
		}else if(!placePiece(getMousePiece())) {
			mouseLeftSquare = false;
			choosePiece(getMousePiece());
		}
	}
	if(mouseButton.button == SDL_BUTTON_RIGHT) {
		deselectPiece();
	}
}
void ChessLogic::MouseUp(const SDL_MouseButtonEvent& mouseButton) {
	mouse.left = mouseButton.button == SDL_BUTTON_LEFT ? false : mouse.left;
	mouse.right = mouseButton.button == SDL_BUTTON_RIGHT ? false : mouse.right;
	mouse.middle = mouseButton.button == SDL_BUTTON_MIDDLE ? false : mouse.middle;

	if(mouseButton.button == SDL_BUTTON_LEFT && pieceSelected) {
		if(!placePiece(getMousePiece())) {
			if(mouseLeftSquare || selectionCount > 1){
				deselectPiece();
			}
		}
	}
}
void ChessLogic::MouseMotion(const SDL_MouseMotionEvent& mouseMotion) {
	mouse.pos.x = mouseMotion.x;
	mouse.pos.y = mouseMotion.y;

	ChessBase::XY pieceXY = getMousePiece();
	if(pieceXY.x != selectedPieceXY.x || pieceXY.y != selectedPieceXY.y) {
		mouseLeftSquare = true;
	}
}