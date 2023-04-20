#include "ChessPiece.h"

ChessPiece::ChessPiece(Color color, Type type) {
	this->color = color;
	this->type = type;
}

ChessPiece::ChessPiece() {

}

void ChessPiece::setColor(Color color) {
	this->color = color;
}

const Color ChessPiece::getColor() {
	return this->color;
}

void ChessPiece::setType(Type type) {
	this->type = type;
}
const Type ChessPiece::getType() {
	return this->type;
}

bool ChessPiece::operator==(ChessPiece const& other) const {
	if (this->color == other.color && this->type == other.type) {
		return true;
	}
	return false;
}

bool ChessPiece::operator <(const ChessPiece& piece) const {
	if (this->color < piece.color) {
		return true;
	}
	else if(this->color > piece.color){
		return false;
	}
	else {
		if (this->type < piece.type) {
			return true;
		}
		else {
			return false;
		}
	}
}