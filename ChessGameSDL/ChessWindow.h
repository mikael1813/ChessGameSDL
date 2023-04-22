#pragma once

#include <SDL.h>
#include "Game.h"

enum class Circle {
	No, Small, Big
};

class ChessWindow {
public:
	SDL_Surface* load_surface(char const* path);
	ChessWindow();
	void onLeftClick(SDL_Surface* window_surface, int x, int y);
	void draw(SDL_Surface* window_surface);
	bool isInsideChessBoard(int x, int y);
	void showCircles(int i, int j);
	void removeCircles();
	void proccesMoveOutcome(unsigned short int i, unsigned short int j, MoveOutcome outcome);
	void initializeToPromote() {
		vector<vector<bool>> aux;
		for (int i = 0; i < game->getSize(); i++) {
			std::vector<bool> row;
			for (int j = 0; j < game->getSize(); j++) {
				row.push_back(false);
			}
			aux.push_back(row);
		}
		toPromote = aux;
	}
	const bool needToPromote() {
		for (int i = 0; i < game->getSize(); i++) {
			for (int j = 0; j < game->getSize(); j++) {
				if (toPromote.at(i).at(j) == true) {
					return true;
				}
			}
		}
		return false;
	}
private:
	SDL_Surface* m_black_square;
	SDL_Surface* m_white_square;
	SDL_Surface* m_small_circle;
	SDL_Surface* m_big_circle;
	SDL_Rect     m_image_position;
	std::vector<std::vector<Circle>> circled;
	const std::vector<Type> promoteOrder{ Type::Rook,Type::Bishop,Type::Knight,Type::Queen };
	std::vector<std::vector<bool>> toPromote;
	Game* game;
	int startX = 0;
	int endX = 0;
	int startY = 0;
	int endY = 0;
	int stepX = 0;
	int stepY = 0;
	string imageAddress = "Images/chessboard/";
	map<ChessPiece, SDL_Surface*> pieceImagePaths;
	Option<tuple<int,int>> lastSelectedPiece;
};