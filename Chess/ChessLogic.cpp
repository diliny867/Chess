#include "ChessLogic.h"


ChessBase::ColorId ChessLogic::getNextPlayerColorId() {
	return (currPlayerColorId+1) % playerColorCount;
}
void ChessLogic::switchPlayer() {
	currPlayerColorId = getNextPlayerColorId();
	turnChangeCallback();
}
void ChessLogic::concludeTurn() {
	DeselectPiece();
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
void ChessLogic::choosePiece(ChessBase::XY pieceXY) {
	if(selectedPieceXY != pieceXY) {
		DeselectPiece();
	}
	if(pieceSelected && GetPiece(pieceXY.x,pieceXY.y).type == ChessBase::Empty) {
		DeselectPiece();
		possibleMoves.clear();
	}else {
		selectionCount++;
		pieceSelected = true;
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
			DeselectPiece();
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
bool ChessLogic::canPlace(i8 x, i8 y) {
	return GetPiece(x, y).colorId == currPlayerColorId;
}
void ChessLogic::CheckMouseDeselect(ChessBase::XY pieceXY) {
	if(pieceSelected && !placePiece(pieceXY)) {
		if(mouseLeftSquare || selectionCount > 1){
			DeselectPiece();
		}
	}
}
void ChessLogic::CheckMouseLeftSquare(ChessBase::XY pieceXY) {
	if(pieceXY.x != selectedPieceXY.x || pieceXY.y != selectedPieceXY.y) {
		mouseLeftSquare = true;
	}
}
void ChessLogic::SelectPiece(ChessBase::XY pieceXY) {
	if(pieceXY.x < 0 || pieceXY.x >= 8 || pieceXY.y < 0 || pieceXY.y >= 8) {
		DeselectPiece();
		return;
	}
	if(promotionData.promotion) {
		selectPromotion(pieceXY);
	}else if(!placePiece(pieceXY)) {
		mouseLeftSquare = false;
		choosePiece(pieceXY);
	}
}
void ChessLogic::DeselectPiece() {
	selectionCount = 0;
	pieceSelected = false;
}
void ChessLogic::Init() {
	chess.Reset();
}

void ChessLogic::SetTurnChangeCallback(const std::function<void()>& callback) {
	turnChangeCallback = callback;
}

PieceRender::PieceColor ChessLogic::GetCurrentPlayerColor() {
	return pieceColors[currPlayerColorId];
}

ChessBase::Piece& ChessLogic::GetPiece(i8 x, i8 y) {
	return chess.GetPiece(x, y);
}
