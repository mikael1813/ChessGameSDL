#pragma once


enum class Color {
	White, Black
};

enum class Type {
	Pawn, Knight, Bishop, Rook, Queen, King
};


class ChessPiece {
private:
	Color color;
	Type type;
public:
	ChessPiece(Color color, Type type);
	ChessPiece();

	void setColor(Color color);
	const Color getColor();
	void setType(Type type);
	const Type getType();

	bool operator==(ChessPiece const& other) const;

	bool operator <(const ChessPiece& piece) const;
};