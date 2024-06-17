#pragma once


#include <iostream>
#include <array>
#include <vector>

#include "Common.h"

class ChessBase {
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
	typedef i8Vec2 XY;
	static Piece NewPiece(PieceType type, ColorId colorId);
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
	using AT_Attack = AttackType::Attack;
	using AT_Dir = AT_Attack::Direction;
	struct BoardState{
		struct {
			XY black ={0,0};
			XY white ={0,0};
		} kingsAt;
		std::array<Piece, 64> board{};
		std::array<AttackType, 64> attacks{};
	} state;

	AttackType& getAttack(i8 x, i8 y);

	bool incAttack(i8 x, i8 y, ColorId colorId, AT_Dir direction);
	bool decAttack(i8 x, i8 y, ColorId colorId, AT_Dir direction);

	void incAttackInDirection(i8 x, i8 y, ColorId colorId, AT_Dir direction);
	void decAttackInDirection(i8 x, i8 y, ColorId colorId, AT_Dir direction);

	void addPieceAttack(i8 x, i8 y, PieceType type, ColorId colorId);
	void removePieceAttack(i8 x, i8 y, PieceType type, ColorId colorId);

	void continueAttacks(i8 x, i8 y);
	void endAttacks(i8 x, i8 y);

	void recalculateAllAttacks();

	bool squareInCheck(i8 x, i8 y, ColorId colorId);
	bool inCheckIf(i8 fromX, i8 fromY, i8 toX, i8 toY, ColorId colorId);
	bool isValidTurnNoCheck(i8 fromX, i8 fromY, i8 toX, i8 toY);

	CanCastleResult canCastle(i8 kingX, i8 kingY);

	void dumpAttacks(ColorId colorId);
	void dumpBoard();

	void flipDirectionsVertical(u8& directions);
	void flipAttack(i8 x, i8 y);

	void clearBoard();
	void setStartBoard();

public:
	bool IsValidTurn(i8 fromX, i8 fromY, i8 toX, i8 toY);
	std::vector<XY> GetAllValidTurns(i8 x, i8 y);

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
	};
	MoveResults GetMoveSpecialResults(i8 fromX, i8 fromY, i8 toX, i8 toY);

	bool IsMated(ColorId colorId);

	void FlipBoard();

	Piece& GetPiece(i8 x, i8 y);
	void SetPiece(i8 x, i8 y, const Piece& piece);
	void TeleportPiece(i8 fromX, i8 fromY, i8 toX, i8 toY);
	void MovePiece(i8 fromX, i8 fromY, i8 toX, i8 toY);

	void Reset();
};