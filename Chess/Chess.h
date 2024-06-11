#pragma once


#include <array>
#include <bitset>
#include <vector>

#include "Common.h"

//#define APPLY_FLAG(target, flag) do{(*(reinterpret_cast<u8*>(&(target)))) |= (flag);}while(0)
//#define DISABLE_FLAG(target, flag) do{(*(reinterpret_cast<u8*>(&(target)))) &= ~(flag);}while(0)
//#define CHECK_FLAG(target, flag) ((*(reinterpret_cast<u8*>(&(target)))) & (flag))
static void applyFlag(u8& target, u8 flag) {
	target |= flag;
}
static void disableFlag(u8& target, u8 flag) {
	target &= ~flag;
}
static u8 getFlag(u8 target, u8 flag) {
	return target & flag;
}
class Chess {
public:
	enum PieceType : i8{
		Empty = 0,
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King
	};
	typedef u8 ColorId;
	static const u8 colorIdCount = 2;
	inline static ColorId ColorIds[colorIdCount] = {0,1};
	struct Piece {
		PieceType type = Empty;
		ColorId colorId = ColorIds[0];
		u32 moveCount = 0;
	};
	struct XY{
		i8 x;
		i8 y;
		inline bool operator==(const XY& rhs) { return x == rhs.x && y == rhs.y; }
		inline bool operator!=(const XY& rhs) { return !(*this == rhs); }
	};
	static Piece NewPiece(PieceType type, ColorId colorId) {
		return Piece{type, colorId, 0};
	}
private:
	struct {
		Piece piece;
		XY from;
		XY to;
	} lastMove = {{Empty,ColorIds[0]},{0,0},{0,0}};
	struct CanCastleResult {
		bool left;
		bool right;
	};
	struct AttackType{
		struct Attack {
			enum Direction : u8 {
				NoDirection = 0,
				Left = 1,
				TopLeft = 2,
				Top = 4,
				TopRight = 8,
				Right = 16,
				BottomRight = 32,
				Bottom = 64,
				BottomLeft = 128
			};
			u8 directions;
			u8 count;
		};
		//Attack atColorId[colorIdCount];
		Attack black;
		Attack white;
	};
	struct {
		XY black = {0,0};
		XY white = {0,0};
	} kingsAt;
	std::array<Piece, 64> board{};
	std::array<AttackType, 64> attacks{};
	AttackType& getAttack(i8 x, i8 y) {
		return attacks[y*8 + x];
	}
	bool incAttack(i8 x, i8 y, ColorId colorId, AttackType::Attack::Direction direction) {
		if(x < 0 || x >= 8 || y < 0 || y >= 8) {
			return false;
		}
		auto& attack = getAttack(x, y);
		if(colorId == ColorIds[1]) {
			attack.black.count++;
			//attack.black.directions |= direction;
			applyFlag(attack.black.directions, direction);
		}else {
			attack.white.count++;
			//attack.white.directions |= direction;
			applyFlag(attack.white.directions, direction);
		}
		return GetPiece(x, y).type == Empty;
	}
	bool decAttack(i8 x, i8 y, ColorId colorId, AttackType::Attack::Direction direction) {
		if(x < 0 || x >= 8 || y < 0 || y >= 8) {
			return false;
		}
		auto& attack = getAttack(x, y);
		if(colorId == ColorIds[1]) {
			attack.black.count--;
			//attack.black.directions &= ~direction;
			disableFlag(attack.black.directions, direction);
		}else {
			attack.white.count--;
			//attack.white.directions &= ~direction;
			disableFlag(attack.white.directions, direction);
		}
		return GetPiece(x, y).type == Empty;
	}
	void incAttackInDirection(i8 x, i8 y, ColorId colorId, AttackType::Attack::Direction direction) {
		switch(direction) {
		case AttackType::Attack::Left:
			for(i8 i=x-1; incAttack(i, y, colorId, AttackType::Attack::Left); i-=1) {}
			break;
		case AttackType::Attack::TopLeft:
			for(i8 x_=x-1, y_=y+1; incAttack(x_, y_, colorId, AttackType::Attack::TopLeft); x_-=1, y_+=1) {}
			break;
		case AttackType::Attack::Top:
			for(i8 i=y+1; incAttack(x, i, colorId, AttackType::Attack::Top); i+=1) {}
			break;
		case AttackType::Attack::TopRight:
			for(i8 x_=x+1, y_=y+1; incAttack(x_, y_, colorId, AttackType::Attack::TopRight); x_+=1, y_+=1) {}
			break;
		case AttackType::Attack::Right:
			for(i8 i=x+1; incAttack(i, y, colorId, AttackType::Attack::Right); i+=1) {}
			break;
		case AttackType::Attack::BottomRight:
			for(i8 x_=x+1, y_=y-1; incAttack(x_, y_, colorId, AttackType::Attack::BottomRight); x_+=1, y_-=1) {}
			break;
		case AttackType::Attack::Bottom:
			for(i8 i=y-1; incAttack(x, i, colorId, AttackType::Attack::Bottom); i-=1) {}
			break;
		case AttackType::Attack::BottomLeft:
			for(i8 x_=x-1, y_=y-1; incAttack(x_, y_, colorId, AttackType::Attack::BottomLeft); x_-=1, y_-=1) {}
			break;
		default: 
			break;
		}
	}
	void decAttackInDirection(i8 x, i8 y, ColorId colorId, AttackType::Attack::Direction direction) {
		switch(direction) {
		case AttackType::Attack::Left:
			for(i8 i=x-1; decAttack(i, y, colorId, AttackType::Attack::Left); i-=1) {}
			break;
		case AttackType::Attack::TopLeft:
			for(i8 x_=x-1, y_=y+1; decAttack(x_, y_, colorId, AttackType::Attack::TopLeft); x_-=1, y_+=1) {}
			break;
		case AttackType::Attack::Top:
			for(i8 i=y+1; decAttack(x, i, colorId, AttackType::Attack::Top); i+=1) {}
			break;
		case AttackType::Attack::TopRight:
			for(i8 x_=x+1, y_=y+1; decAttack(x_, y_, colorId, AttackType::Attack::TopRight); x_+=1, y_+=1) {}
			break;
		case AttackType::Attack::Right:
			for(i8 i=x+1; decAttack(i, y, colorId, AttackType::Attack::Right); i+=1) {}
			break;
		case AttackType::Attack::BottomRight:
			for(i8 x_=x+1, y_=y-1; decAttack(x_, y_, colorId, AttackType::Attack::BottomRight); x_+=1, y_-=1) {}
			break;
		case AttackType::Attack::Bottom:
			for(i8 i=y-1; decAttack(x, i, colorId, AttackType::Attack::Bottom); i-=1) {}
			break;
		case AttackType::Attack::BottomLeft:
			for(i8 x_=x-1, y_=y-1; decAttack(x_, y_, colorId, AttackType::Attack::BottomLeft); x_-=1, y_-=1) {}
			break;
		default: 
			break;
		}
	}
	void addPieceAttack(i8 x, i8 y, PieceType type, ColorId colorId) {
		switch(type) {
		case Pawn:
			incAttack(x - 1, y + (colorId == ColorIds[1] ? -1 : 1), colorId, AttackType::Attack::NoDirection);
			incAttack(x + 1, y + (colorId == ColorIds[1] ? -1 : 1), colorId, AttackType::Attack::NoDirection);
			break;
		case Knight:
			incAttack(x - 1, y + 2, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 1, y + 2, colorId, AttackType::Attack::NoDirection);
			incAttack(x - 1, y - 2, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 1, y - 2, colorId, AttackType::Attack::NoDirection);
			incAttack(x - 2, y + 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 2, y + 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x - 2, y - 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 2, y - 1, colorId, AttackType::Attack::NoDirection);
			break;
		case Bishop: {
			incAttackInDirection(x, y, colorId, AttackType::Attack::TopRight);
			incAttackInDirection(x, y, colorId, AttackType::Attack::BottomRight);
			incAttackInDirection(x, y, colorId, AttackType::Attack::TopLeft);
			incAttackInDirection(x, y, colorId, AttackType::Attack::BottomLeft);
			break;
		}
		case Rook:
			incAttackInDirection(x, y, colorId, AttackType::Attack::Right);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Left);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Top);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Bottom);
			break;
		case Queen:
			incAttackInDirection(x, y, colorId, AttackType::Attack::TopRight);
			incAttackInDirection(x, y, colorId, AttackType::Attack::BottomRight);
			incAttackInDirection(x, y, colorId, AttackType::Attack::TopLeft);
			incAttackInDirection(x, y, colorId, AttackType::Attack::BottomLeft);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Right);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Left);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Top);
			incAttackInDirection(x, y, colorId, AttackType::Attack::Bottom);
			break;
		case King:
			incAttack(x - 1, y + 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x - 1, y, colorId, AttackType::Attack::NoDirection);
			incAttack(x - 1, y - 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 1, y + 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 1, y, colorId, AttackType::Attack::NoDirection);
			incAttack(x + 1, y - 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x, y + 1, colorId, AttackType::Attack::NoDirection);
			incAttack(x, y - 1, colorId, AttackType::Attack::NoDirection);
			break;
		default:
			break;
		}
	}
	void removePieceAttack(i8 x, i8 y, PieceType type, ColorId colorId) {
		switch(type) {
		case Pawn:
			decAttack(x - 1, y + (colorId == ColorIds[1] ? -1 : 1), colorId, AttackType::Attack::NoDirection);
			decAttack(x + 1, y + (colorId == ColorIds[1] ? -1 : 1), colorId, AttackType::Attack::NoDirection);
			break;
		case Knight:
			decAttack(x - 1, y + 2, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 1, y + 2, colorId, AttackType::Attack::NoDirection);
			decAttack(x - 1, y - 2, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 1, y - 2, colorId, AttackType::Attack::NoDirection);
			decAttack(x - 2, y + 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 2, y + 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x - 2, y - 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 2, y - 1, colorId, AttackType::Attack::NoDirection);
			break;
		case Bishop: {
			decAttackInDirection(x, y, colorId, AttackType::Attack::TopRight);
			decAttackInDirection(x, y, colorId, AttackType::Attack::BottomRight);
			decAttackInDirection(x, y, colorId, AttackType::Attack::TopLeft);
			decAttackInDirection(x, y, colorId, AttackType::Attack::BottomLeft);
			break;
		}
		case Rook:
			decAttackInDirection(x, y, colorId, AttackType::Attack::Right);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Left);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Top);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Bottom);
			break;
		case Queen:
			decAttackInDirection(x, y, colorId, AttackType::Attack::TopRight);
			decAttackInDirection(x, y, colorId, AttackType::Attack::BottomRight);
			decAttackInDirection(x, y, colorId, AttackType::Attack::TopLeft);
			decAttackInDirection(x, y, colorId, AttackType::Attack::BottomLeft);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Right);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Left);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Top);
			decAttackInDirection(x, y, colorId, AttackType::Attack::Bottom);
			break;
		case King:
			decAttack(x - 1, y + 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x - 1, y, colorId, AttackType::Attack::NoDirection);
			decAttack(x - 1, y - 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 1, y + 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 1, y, colorId, AttackType::Attack::NoDirection);
			decAttack(x + 1, y - 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x, y + 1, colorId, AttackType::Attack::NoDirection);
			decAttack(x, y - 1, colorId, AttackType::Attack::NoDirection);
			break;
		default:
			break;
		}
	}
	void continueAttacks(i8 x, i8 y) {
		ColorId colorId = ColorIds[0];
		for(int i=0; i<2; i++){
			const auto& directions = colorId == ColorIds[1] ? getAttack(x, y).black.directions : getAttack(x, y).white.directions;
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Left)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::TopLeft)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Top)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::TopRight)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Right)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::BottomRight)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Bottom)));
			incAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::BottomLeft)));
			colorId = colorId == ColorIds[0] ? ColorIds[1] : ColorIds[0];
		}
	}
	void endAttacks(i8 x, i8 y) {
		ColorId colorId = ColorIds[0];
		for(int i=0; i<2; i++){
			const auto& directions = colorId == ColorIds[1] ? getAttack(x, y).black.directions : getAttack(x, y).white.directions;
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Left)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::TopLeft)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Top)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::TopRight)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Right)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::BottomRight)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::Bottom)));
			decAttackInDirection(x, y, colorId, static_cast<AttackType::Attack::Direction>(getFlag(directions, AttackType::Attack::BottomLeft)));
			colorId = colorId == ColorIds[0] ? ColorIds[1] : ColorIds[0];
		}
	}
	void recalculateAllAttacks() {
		attacks.fill({{0,0},{0,0}});
		for(i8 y=0; y<8; y++) {
			for(i8 x=0; x<8 ;x++) {
				const auto& piece = GetPiece(x, y);
				addPieceAttack(x, y, piece.type, piece.colorId);
			}
		}
	}
	bool squareInCheck(i8 x, i8 y, ColorId colorId) {
		auto& attack = getAttack(x, y);
		return colorId == ColorIds[1] ? attack.white.count : attack.black.count;
	}
	bool inCheckIf(i8 fromX, i8 fromY, i8 toX, i8 toY, ColorId colorId) {
		i8 kingX = colorId == ColorIds[1] ? kingsAt.black.x : kingsAt.white.x;
		i8 kingY = colorId == ColorIds[1] ? kingsAt.black.y : kingsAt.white.y;
		auto lastAttacks = std::array(attacks);
		auto lastBoard = std::array(board);
		auto lastKingsAt = kingsAt;
		TeleportPiece(fromX, fromY, toX, toY);
		if(kingX == fromX && kingY == fromY) {
			kingX = toX;
			kingY = toY;
		}
		const bool check = squareInCheck(kingX, kingY, colorId);
		attacks = lastAttacks;
		board = lastBoard;
		kingsAt = lastKingsAt;
		return check;
	}
	bool isValidTurnNoCheck(i8 fromX, i8 fromY, i8 toX, i8 toY) {
		if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 || toX < 0 || toX >= 8 || toY < 0 || toY >= 8) {
			return false;
		}
		if(fromX == toX && fromY == toY) {
			return false;
		}
		const Piece& pieceFrom = GetPiece(fromX, fromY);
		const Piece& pieceTo = GetPiece(toX, toY);
		if(pieceFrom.type == Empty || pieceTo.type != Empty && pieceTo.colorId == pieceFrom.colorId) {
			return false;
		}
		const i8 deltaX = toX-fromX;
		const i8 deltaY = toY-fromY;

		switch(pieceFrom.type) {
		case Pawn:
			if(pieceFrom.moveCount == 0) {
				if(pieceFrom.colorId && deltaY == -2 || !pieceFrom.colorId && deltaY == 2) {
					if(deltaX == 0 && pieceTo.type == Empty) {
						return true;
					}
				}
			}
			if(pieceFrom.colorId && deltaY == -1 || !pieceFrom.colorId && deltaY == 1) {
				if(deltaX == 0 && pieceTo.type == Empty) {
					return true;
				}
				if(std::abs(deltaX) == 1) {
					if(pieceTo.type != Empty && pieceTo.colorId != pieceFrom.colorId) {
						return true;
					}
					if(lastMove.to.x == toX && lastMove.piece.type == Pawn && lastMove.piece.colorId != pieceFrom.colorId && std::abs(lastMove.to.y - lastMove.from.y) == 2 
					   && (pieceFrom.colorId && fromY == 3 || !pieceFrom.colorId && fromY == 4)) { //en passant
						return true;
					}
				}
			}
			return false;
		case Knight:
			if(abs(deltaX) == 1 && abs(deltaY) == 2 || abs(deltaX) == 2 && abs(deltaY) == 1) {
				return true;
			}
			return false;
		case Bishop:
			if(abs(deltaX) == abs(deltaY)) {
				const i8 directionX = deltaX > 0 ? 1: -1;
				const i8 directionY = deltaY > 0 ? 1: -1;
				i8 x, y;
				for(x=fromX+directionX, y=fromY+directionY; x!=toX || y!=toY; x+=directionX, y+=directionY) {
					if(GetPiece(x,y).type != Empty) {
						return false;
					}
				}
				return true;
			}
			return false;
		case Rook:
			if(deltaY == 0) {
				const i8 direction = deltaX > 0 ? 1: -1;
				for(i8 i=fromX+direction; i!=toX; i+=direction) {
					if(GetPiece(i,fromY).type != Empty) {
						return false;
					}
				}
				return true;
			}
			if(deltaX == 0) {
				const i8 direction = deltaY > 0 ? 1: -1;
				for(i8 i=fromY+direction; i!=toY; i+=direction) {
					if(GetPiece(fromX,i).type != Empty) {
						return false;
					}
				}
				return true;
			}
			return false;
		case Queen:
			if(deltaY == 0) {
				const i8 direction = deltaX > 0 ? 1: -1;
				for(i8 i=fromX+direction; i!=toX; i+=direction) {
					if(GetPiece(i,fromY).type != Empty) {
						return false;
					}
				}
				return true;
			}
			if(deltaX == 0) {
				const i8 direction = deltaY > 0 ? 1: -1;
				for(i8 i=fromY+direction; i!=toY; i+=direction) {
					if(GetPiece(fromX,i).type != Empty) {
						return false;
					}
				}
				return true;
			}
			if(abs(deltaX) == abs(deltaY)) {
				const i8 directionX = deltaX > 0 ? 1: -1;
				const i8 directionY = deltaY > 0 ? 1: -1;
				i8 x, y;
				for(x=fromX+directionX, y=fromY+directionY; x!=toX || y!=toY; x+=directionX, y+=directionY) {
					if(GetPiece(x,y).type != Empty) {
						return false;
					}
				}
				return true;
			}
			return false;
		case King:
			CanCastleResult castle = canCastle(fromX, fromY);
			if(deltaY == 0) {
				i8 castleKingX;
				if(castle.left) {
					castleKingX = fromX + (-fromX - 1) / 2;
					if(castleKingX == toX) {
						return true;
					}
				}
				if(castle.right) {
					castleKingX = fromX + (7 - fromX + 1) / 2;
					if(castleKingX == toX) {
						return true;
					}
				}
			}
			if(abs(deltaX) <= 1 && abs(deltaY) <= 1) {
				if(!squareInCheck(toX,toY,pieceFrom.colorId)) {
					return true;
				}
			}
			return false;
		default:
			return false;
		}
	}
	CanCastleResult canCastle(i8 kingX, i8 kingY) {
		const Piece& king = GetPiece(kingX, kingY);
		if(king.type != King || king.moveCount>0) {
			return {false, false};
		}
		CanCastleResult result = {true, true};
		const Piece& leftRook = GetPiece(0, kingY);
		const Piece& rightRook = GetPiece(7, kingY);
		if(leftRook.type == Rook && leftRook.moveCount == 0 && leftRook.colorId == king.colorId) {
			for(i8 i=1;i<kingX;i++) {
				if(GetPiece(i,kingY).type != Empty || squareInCheck(i,kingY,king.colorId)) {
					result.left = false;
					break;
				}
			}
		}
		if(rightRook.type == Rook && rightRook.moveCount == 0 && rightRook.colorId == king.colorId) {
			for(i8 i=kingX+1;i<7;i++) {
				if(GetPiece(i,kingY).type != Empty || squareInCheck(i,kingY,king.colorId)) {
					result.right = false;
					break;
				}
			}
		}
		return result;
	}
	void dumpAttacks(ColorId colorId) {
		std::cout<<"\n";
		for(i8 y=7;y>=0;y--) {
			std::cout<<i32(y+1);
			for(i8 x=0;x<8;x++) {
				auto& attack = colorId == ColorIds[0] ? getAttack(x, y).white : getAttack(x, y).black;
				auto& directions = attack.directions;
				std::cout<< (getFlag(directions, AttackType::Attack::TopLeft) ? "Q" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::TopRight) ? "E" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::Top) ? "T" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::BottomLeft) ? "Z" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::BottomRight) ? "C" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::Bottom) ? "B" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::Left) ? "L" : " ");
				std::cout<< (getFlag(directions, AttackType::Attack::Right) ? "R" : " ");
				std::cout<< (attack.count > 0 ? std::to_string(attack.count) : " ");
				std::cout<<"|";
			}
			std::cout<<"\n";
		}
		std::cout<<" ";
		for(i8 i=0; i<8; i++) {
			std::cout<<char('a'+i)<<"         ";
		}
		std::cout<<"\n";
	}
	void dumpBoard() {
		dumpAttacks(ColorIds[1]);
		dumpAttacks(ColorIds[0]);
	}
	void flipDirectionsVertical(u8& directions) {
		if(getFlag(directions, AttackType::Attack::TopLeft)) {
			disableFlag(directions, AttackType::Attack::TopLeft);
			applyFlag(directions, AttackType::Attack::BottomLeft);
		}
		if(getFlag(directions, AttackType::Attack::TopRight)) {
			disableFlag(directions, AttackType::Attack::TopRight);
			applyFlag(directions, AttackType::Attack::BottomRight);
		}
		if(getFlag(directions, AttackType::Attack::Top)) {
			disableFlag(directions, AttackType::Attack::Top);
			applyFlag(directions, AttackType::Attack::Bottom);
		}
		if(getFlag(directions, AttackType::Attack::BottomLeft)) {
			disableFlag(directions, AttackType::Attack::BottomLeft);
			applyFlag(directions, AttackType::Attack::TopLeft);
		}
		if(getFlag(directions, AttackType::Attack::BottomRight)) {
			disableFlag(directions, AttackType::Attack::BottomRight);
			applyFlag(directions, AttackType::Attack::TopRight);
		}
		if(getFlag(directions, AttackType::Attack::Bottom)) {
			disableFlag(directions, AttackType::Attack::Bottom);
			applyFlag(directions, AttackType::Attack::Top);
		}
	}
	void flipAttack(i8 x, i8 y) {
		auto& attack = getAttack(x, y);
		flipDirectionsVertical(attack.white.directions);
		flipDirectionsVertical(attack.black.directions);
	}
	void clearBoard() {
		board.fill(NewPiece(Empty,ColorIds[0]));
		attacks.fill({{0,0},{0,0}});
	}
	void setStartBoard() {
		clearBoard();
		for (i8 i=0; i<8; i++) {
			SetPiece(i, 1, NewPiece(Pawn, ColorIds[0]));
			SetPiece(i, 6, NewPiece(Pawn, ColorIds[1]));
		}
		for(i8 y = 0; y<=7; y+=7){
			SetPiece(0, y, NewPiece(Rook, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(7, y, NewPiece(Rook, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(1, y, NewPiece(Knight, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(6, y, NewPiece(Knight, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(2, y, NewPiece(Bishop, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(5, y, NewPiece(Bishop, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(3, y, NewPiece(Queen, y == 7 ? ColorIds[1] : ColorIds[0]));
			SetPiece(4, y, NewPiece(King, y == 7 ? ColorIds[1] : ColorIds[0]));
		}
	}
public:
	bool IsValidTurn(i8 fromX, i8 fromY, i8 toX, i8 toY) {
		if(isValidTurnNoCheck(fromX,fromY,toX,toY)) {
			const auto& pieceFrom = GetPiece(fromX, fromY);
			return !inCheckIf(fromX, fromY, toX, toY, pieceFrom.colorId);
		}
		return false;
	}
	std::vector<XY> GetAllValidTurns(i8 x, i8 y) {
		std::vector<XY> res;
		if(GetPiece(x,y).type == Empty) {
			return res;
		}
		for(i8 y_=0; y_<8; y_++) {
			for(i8 x_=0; x_<8; x_++) {
				if(IsValidTurn(x,y, x_, y_)) {
					res.push_back({x_, y_});
				}
			}
		}
		return res;
	}
	//enum MoveResultsFlags : u8{
	//	Check = 1,
	//	Mate = 1<<1,
	//	Castle = 1<<2,
	//	Promotion = 1<<3,
	//	EnPassant = 1<<4
	//};
	struct MoveResults{
		enum : u8{
			NonSpecial,
			Castle,
			Promotion,
			EnPassant
		} type;
		union{
			struct {
				XY rookFromXY;
				XY rookToXY;
			} castleData;
			struct {
				XY xy;
			} enPassandData;
		};
	} GetMoveSpecialResults(i8 fromX, i8 fromY, i8 toX, i8 toY) {
		MoveResults res = {MoveResults::NonSpecial, {{{0,0},{0,0}}}};
		const auto& piece = GetPiece(fromX, fromY);
		if(piece.type == Pawn){
			if(toY == 7 && piece.colorId == 0 || toY == 0 && piece.colorId == 1) {
				res.type = MoveResults::Promotion;
			}else if(lastMove.to.x == toX && lastMove.piece.type == Pawn && lastMove.piece.colorId != piece.colorId && std::abs(lastMove.to.y - lastMove.from.y) == 2 
					 && (piece.colorId && fromY == 3 || !piece.colorId && fromY == 4)) {
				res.type = MoveResults::EnPassant;
				res.enPassandData.xy = lastMove.to;
			}
		} else if(piece.type == King){
			CanCastleResult castle = canCastle(fromX, fromY);
			if(fromY == toY) {
				i8 castleKingX;
				if(castle.left) {
					castleKingX = fromX + (-fromX - 1) / 2;
					if(castleKingX == toX) {
						res.type = MoveResults::Castle;
						res.castleData.rookFromXY.x = 0;
						res.castleData.rookFromXY.y = fromY;
						res.castleData.rookToXY.x = toX+1;
						res.castleData.rookToXY.y = toY;
					}
				}
				if(castle.right) {
					castleKingX = fromX + (7 - fromX + 1) / 2;
					if(castleKingX == toX) {
						res.type = MoveResults::Castle;
						res.castleData.rookFromXY.x = 7;
						res.castleData.rookFromXY.y = fromY;
						res.castleData.rookToXY.x = toX-1;
						res.castleData.rookToXY.y = toY;
					}
				}
			}
		}
		return res;
	}
	bool IsMated(ColorId colorId) { //TODO: FIX
		return false;
		i8 kingX = colorId == ColorIds[1] ? kingsAt.black.x : kingsAt.white.x;
		i8 kingY = colorId == ColorIds[1] ? kingsAt.black.y : kingsAt.white.y;
		const auto& attack =  colorId == ColorIds[1] ? getAttack(kingX, kingY).black : getAttack(kingX, kingY).white;
		if(attack.count == 0) {
			return false;
		}

		bool kingCanMove = !GetAllValidTurns(kingX,kingY).empty();
		if(!squareInCheck(kingX, kingY, colorId)) {
			return false;
		}

		if(attack.count > 1) { // more than 1 attacking piece
			return !kingCanMove;
		}
		std::vector<XY> matePreventionSquares;
		switch(attack.directions) { // here only maximum of 1 direction flag is toggled
		case AttackType::Attack::NoDirection:
			for(i8 y=0; y<8; y++) {
				for(i8 x=0; x<8; x++) {
					if(IsValidTurn(x,y,kingX,kingY)) {
						matePreventionSquares.push_back({x,y});
					}
				}
			}
			break;
		// go opposite side
		case AttackType::Attack::Left:
			for(i8 i=1; i<8; i++) {
				const XY square = {static_cast<i8>(kingX+i), kingY};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::TopLeft:
			for(i8 i=1; i<8; i++) {
				const XY square = {static_cast<i8>(kingX+i), static_cast<i8>(kingY-i)};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::Top:
			for(i8 i=1; i<8; i++) {
				const XY square = {kingX, static_cast<i8>(kingY-i)};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::TopRight:
			for(i8 i=1; i<8; i++) {
				const XY square = {static_cast<i8>(kingX-i), static_cast<i8>(kingY-i)};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::Right:
			for(i8 i=1; i<8; i++) {
				const XY square = {static_cast<i8>(kingX-i), kingY};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::BottomRight:
			for(i8 i=1; i<8; i++) {
				const XY square = {static_cast<i8>(kingX-i), static_cast<i8>(kingY+i)};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::Bottom:
			for(i8 i=1; i<8; i++) {
				const XY square = {kingX, static_cast<i8>(kingY+i)};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		case AttackType::Attack::BottomLeft:
			for(i8 i=1; i<8; i++) {
				const XY square = {static_cast<i8>(kingX+i), static_cast<i8>(kingY+i)};
				const auto& piece = GetPiece(square.x, square.y);
				matePreventionSquares.push_back(square);
				if(piece.type != Empty && piece.colorId != colorId) {
					break;
				}
			}
			break;
		}

		for(i8 y=0; y<8; y++) {
			for(i8 x=0; x<8; x++) {
				for(auto& square: matePreventionSquares) {
					if(IsValidTurn(y,x, square.x, square.y)) {
						return false;
					}
				}
			}
		}
		return true;
	}
	void FlipBoard() {
		for(i8 y=0;y<4;y++) {
			for(i8 x=0;x<8;x++) {
				std::swap(GetPiece(x,y), GetPiece(x,7-y));
				std::swap(getAttack(x,y), getAttack(x,7-y));
				flipAttack(x, y); flipAttack(x, 7-y);
			}
		}
	}
	Piece& GetPiece(i8 x, i8 y) {
		return board[y*8 + x];
	}
	void SetPiece(i8 x, i8 y, const Piece& piece) { //why wont work
		auto& lastPiece = GetPiece(x, y);
		if(lastPiece.type == Empty) {
			if(piece.type != Empty) {
				endAttacks(x, y);
			}
		}else {
			removePieceAttack(x, y, lastPiece.type, lastPiece.colorId);
			if(piece.type == Empty) {
				continueAttacks(x, y);
			}
		}
		lastPiece = piece;
		addPieceAttack(x, y, piece.type, piece.colorId);
		if(piece.type == King) {
			if(piece.colorId) {
				kingsAt.black.x = x;
				kingsAt.black.y = y;
			}else {
				kingsAt.white.x = x;
				kingsAt.white.y = y;
			}
		}
	}
	void TeleportPiece(i8 fromX, i8 fromY, i8 toX, i8 toY) {
		SetPiece(toX, toY, GetPiece(fromX, fromY));
		SetPiece(fromX, fromY, NewPiece(Empty,ColorIds[0]));
	}
	void MovePiece(i8 fromX, i8 fromY, i8 toX, i8 toY) {
		TeleportPiece(fromX, fromY, toX, toY);
		Piece& piece = GetPiece(toX, toY);
		piece.moveCount++;
		lastMove = {piece, {fromX, fromY}, {toX, toY}};
	}
	void Reset() {
		setStartBoard();
	}
};