#include "Game.h"
#include <sstream>


Game::Game() {

	for (int i = 0; i < 4; i++) {
		haveRooksMoved.push_back(false);
	}

	this->board.push_back({ Option<ChessPiece>(ChessPiece(Color::Black, Type::Rook)),Option<ChessPiece>(ChessPiece(Color::Black, Type::Knight)),Option<ChessPiece>(ChessPiece(Color::Black, Type::Bishop)),Option<ChessPiece>(ChessPiece(Color::Black, Type::Queen)),Option<ChessPiece>(ChessPiece(Color::Black, Type::King)),Option<ChessPiece>(ChessPiece(Color::Black, Type::Bishop)),Option<ChessPiece>(ChessPiece(Color::Black, Type::Knight)),Option<ChessPiece>(ChessPiece(Color::Black, Type::Rook)) });
	for (int i = 1; i < this->boardSize - 1; i++) {
		vector<Option<ChessPiece>> row;
		for (int j = 0; j < this->boardSize; j++) {
			switch (i) {
			case 1:
				row.push_back(Option<ChessPiece>(ChessPiece(Color::Black, Type::Pawn)));
				break;
			case 6:
				row.push_back(Option<ChessPiece>(ChessPiece(Color::White, Type::Pawn)));
				break;
			default:
				row.push_back(Option<ChessPiece>());
				break;
			}
		}
		this->board.push_back(row);
	}
	this->board.push_back({ Option<ChessPiece>(ChessPiece(Color::White, Type::Rook)),Option<ChessPiece>(ChessPiece(Color::White, Type::Knight)),Option<ChessPiece>(ChessPiece(Color::White, Type::Bishop)),Option<ChessPiece>(ChessPiece(Color::White, Type::Queen)),Option<ChessPiece>(ChessPiece(Color::White, Type::King)),Option<ChessPiece>(ChessPiece(Color::White, Type::Bishop)),Option<ChessPiece>(ChessPiece(Color::White, Type::Knight)),Option<ChessPiece>(ChessPiece(Color::White, Type::Rook)) });
}

const tuple<unsigned short int, unsigned short int> Game::getActualPostion(const string position) {

	regex reg("[a-hA-H][0-9]");
	if (position.size() != 2 || !regex_match(position, reg)) {
		throw;
	}

	char letter = position.at(0);
	if (letter >= 'A' && letter <= 'Z') {
		letter = 'a' + (letter - 'A');
	}
	return { this->boardSize - atoi(&position.at(1)),letterPositions.at(letter) };
}

const vector< tuple<unsigned short int, unsigned short int>> Game::checkCastles(Color color) {
	vector< tuple<unsigned short int, unsigned short int>> castles;

	int x = 0;
	int k = 0;
	if (color == Color::White) {
		x = boardSize - 1;
		k = 2;
	}

	if (!hasKingMoved(color)) {

		vector< tuple<unsigned short int, unsigned short int>> possible_stops;

		for (unsigned short int i = 0; i < this->boardSize; i++) {
			for (unsigned short int j = 0; j < this->boardSize; j++) {
				if (!this->board.at(i).at(j).isEmpty() && this->board.at(i).at(j).getData().getColor() != color) {
					vector< tuple<unsigned short int, unsigned short int>> possible_moves = getAllPossibleMoves(i, j);
					possible_stops.insert(possible_stops.end(), possible_moves.begin(), possible_moves.end());
				}
			}
		}

		if (haveRooksMoved.at(k) == false) {
			bool ok = true;
			for (int j = 1; j < 4; j++) {
				if (board.at(x).at(j).isNotEmpty() || std::find(possible_stops.begin(), possible_stops.end(), tuple<unsigned short int, unsigned short int>{x, j}) != possible_stops.end()) {
					ok = false;
					break;
				}
			}
			if (ok) {
				castles.push_back(tuple<unsigned short int, unsigned short int>{x, 2});
			}
		}

		if (haveRooksMoved.at(k + 1) == false) {
			bool ok = true;
			for (int j = 5; j < 7; j++) {
				if (board.at(x).at(j).isNotEmpty() || std::find(possible_stops.begin(), possible_stops.end(), tuple<unsigned short int, unsigned short int>{x, j}) != possible_stops.end()) {
					ok = false;
					break;
				}
			}
			if (ok) {
				castles.push_back(tuple<unsigned short int, unsigned short int>{x, 6});
			}
		}
	}

	return castles;
}

const bool Game::ifEnPassant(const unsigned short int i, const unsigned short int j) {
	if (this->board.at(i).at(j).isEmpty())
		return false;
	ChessPiece piece = this->board.at(i).at(j).getData();

	if (piece.getType() != Type::Pawn) {
		return false;
	}

	if (i == 3 && piece.getColor() != Color::Black) {
		return false;
	}
	if (i == boardSize - 4 && piece.getColor() != Color::White) {
		return false;
	}

	char letter = 'a' + j;
	string pos;
	pos.push_back(letter);
	pos += to_string(boardSize - i);
	string pos2 = pos + "+";

	if (movesHistory.at(movesHistory.size() - 1) != pos && movesHistory.at(movesHistory.size() - 1) != pos2) {
		return false;
	}

	int x = 2;
	if (i == boardSize - 4) {
		x = i + 1;
	}

	pos = "";
	pos.push_back(letter);
	pos += to_string(boardSize - x);
	pos2 = pos + "+";

	for (string position : movesHistory) {
		if (position == pos || position == pos2) {
			return false;
		}
	}

	return true;
}

const  vector< tuple<unsigned short int, unsigned short int>> Game::getAllPossibleMoves(const unsigned short int i, const unsigned short int j) {
	vector< tuple<unsigned short int, unsigned short int>> possible_moves;

	ChessPiece piece = this->board.at(i).at(j).getData();

	if (piece == ChessPiece(Color::Black, Type::Pawn)) {
		if (i < boardSize && this->board.at(i + 1).at(j).isEmpty()) {
			possible_moves.push_back({ i + 1,j });
		}
		if (i == 1 && this->board.at(i + 2).at(j).isEmpty()) {
			possible_moves.push_back({ i + 2,j });
		}
		if (i < boardSize && j < this->boardSize - 1 && this->board.at(i + 1).at(j + 1).isNotEmpty() && this->board.at(i + 1).at(j + 1).getData().getColor() == Color::White) {
			possible_moves.push_back({ i + 1,j + 1 });
		}
		if (i < boardSize && j > 0 && this->board.at(i + 1).at(j - 1).isNotEmpty() && this->board.at(i + 1).at(j - 1).getData().getColor() == Color::White) {
			possible_moves.push_back({ i + 1,j - 1 });
		}
		if (j > 0 && i == boardSize - 4 && this->board.at(i).at(j - 1).isNotEmpty() && this->board.at(i).at(j - 1).getData().getType() == Type::Pawn && ifEnPassant(i, j - 1)) {
			possible_moves.push_back({ i + 1,j - 1 });
		}
		if (j < this->boardSize - 1 && i == boardSize - 4 && this->board.at(i).at(j + 1).isNotEmpty() && this->board.at(i).at(j + 1).getData().getType() == Type::Pawn && ifEnPassant(i, j + 1)) {
			possible_moves.push_back({ i + 1,j + 1 });
		}
	}

	else if (piece == ChessPiece(Color::White, Type::Pawn)) {
		if (i > 0 && this->board.at(i - 1).at(j).isEmpty()) {
			possible_moves.push_back({ i - 1,j });
		}
		if (i == this->boardSize - 2 && this->board.at(i - 2).at(j).isEmpty()) {
			possible_moves.push_back({ i - 2,j });
		}
		if (i > 0 && j < this->boardSize - 1 && this->board.at(i - 1).at(j + 1).isNotEmpty() && this->board.at(i - 1).at(j + 1).getData().getColor() == Color::Black) {
			possible_moves.push_back({ i - 1,j + 1 });
		}
		if (i > 0 && j > 0 && this->board.at(i - 1).at(j - 1).isNotEmpty() && this->board.at(i - 1).at(j - 1).getData().getColor() == Color::Black) {
			possible_moves.push_back({ i - 1,j - 1 });
		}
		if (j > 0 && i == 3 && this->board.at(i).at(j - 1).isNotEmpty() && this->board.at(i).at(j - 1).getData().getType() == Type::Pawn && ifEnPassant(i, j - 1)) {
			possible_moves.push_back({ i - 1,j - 1 });
		}
		if (j < this->boardSize - 1 && i == 3 && this->board.at(i).at(j + 1).isNotEmpty() && this->board.at(i).at(j + 1).getData().getType() == Type::Pawn && ifEnPassant(i, j + 1)) {
			possible_moves.push_back({ i - 1,j + 1 });
		}
	}

	else if (piece.getType() == Type::Knight) {
		vector<int> moves{ -2,-1,1,2 };
		for (auto k : moves) {
			for (auto l : moves) {
				if (abs(k) != abs(l)) {
					int posi = i + k, posj = j + l;
					if (posi < 0 || posi >= this->boardSize || posj < 0 || posj >= this->boardSize) {
						//break;
					}
					else if (this->board.at(posi).at(posj).isEmpty()) {
						possible_moves.push_back({ posi,posj });
					}
					else if (this->board.at(posi).at(posj).getData().getColor() != piece.getColor()) {
						possible_moves.push_back({ posi,posj });
						//break;
					}
					else if (this->board.at(posi).at(posj).getData().getColor() == piece.getColor()) {
						//break;
					}
				}
			}
		}
	}

	else if (piece.getType() == Type::Bishop) {
		possible_moves = this->getAllBishopMoves(i, j);
	}

	else if (piece.getType() == Type::Rook) {
		possible_moves = this->getAllRookMoves(i, j);
	}

	else if (piece.getType() == Type::Queen) {
		possible_moves = this->getAllQueenMoves(i, j);
	}

	else if (piece.getType() == Type::King) {
		for (int k = -1; k <= 1; k++) {
			for (int l = -1; l <= 1; l++) {
				if (!(k == 0 && l == 0) && 0 <= i + k && i + k < this->boardSize && 0 <= j + l && j + l < this->boardSize && (this->board.at(i + k).at(j + l).isEmpty() || this->board.at(i + k).at(j + l).getData().getColor() != piece.getColor())) {
					possible_moves.push_back({ i + k,j + l });
				}
			}
		}
	}

	return possible_moves;
}

const vector< tuple<unsigned short int, unsigned short int>> Game::getAllBishopMoves(const unsigned short int i, const unsigned short int j) {
	Color color = this->board.at(i).at(j).getData().getColor();

	vector< tuple<unsigned short int, unsigned short int>> possible_moves;

	for (int k = -1; k <= 1; k = k + 2) {
		for (int l = -1; l <= 1; l = l + 2) {
			vector< tuple<unsigned short int, unsigned short int>> aux = this->getMoves(i, j, k, l, color);
			possible_moves.insert(possible_moves.end(), aux.begin(), aux.end());
		}
	}

	return possible_moves;
}


const vector< tuple<unsigned short int, unsigned short int>> Game::getAllRookMoves(const unsigned short int i, const unsigned short int j) {
	Color color = this->board.at(i).at(j).getData().getColor();

	vector< tuple<unsigned short int, unsigned short int>> possible_moves;

	for (int k = -1; k <= 1; k = k + 2) {
		vector< tuple<unsigned short int, unsigned short int>> aux = this->getMoves(i, j, k, 0, color);
		possible_moves.insert(possible_moves.end(), aux.begin(), aux.end());
	}
	for (int l = -1; l <= 1; l = l + 2) {
		vector< tuple<unsigned short int, unsigned short int>> aux = this->getMoves(i, j, 0, l, color);
		possible_moves.insert(possible_moves.end(), aux.begin(), aux.end());
	}

	return possible_moves;
}

const vector< tuple<unsigned short int, unsigned short int>> Game::getAllQueenMoves(const unsigned short int i, const unsigned short int j) {
	vector< tuple<unsigned short int, unsigned short int>> possible_moves;

	vector< tuple<unsigned short int, unsigned short int>> aux;

	aux = this->getAllBishopMoves(i, j);
	possible_moves.insert(possible_moves.end(), aux.begin(), aux.end());

	aux = this->getAllRookMoves(i, j);
	possible_moves.insert(possible_moves.end(), aux.begin(), aux.end());

	return possible_moves;
}

const vector< tuple<unsigned short int, unsigned short int>> Game::getMoves(short int i, short int j, const short int k, const short int l, const Color color) {

	vector< tuple<unsigned short int, unsigned short int>> possible_moves;

	while (true) {
		i += k;
		j += l;
		if (i < 0 || i >= this->boardSize || j < 0 || j >= this->boardSize) {
			break;
		}
		else if (this->board.at(i).at(j).isEmpty()) {
			possible_moves.push_back({ i,j });
		}
		else if (this->board.at(i).at(j).getData().getColor() != color) {
			possible_moves.push_back({ i,j });
			break;
		}
		else if (this->board.at(i).at(j).getData().getColor() == color) {
			break;
		}
	}

	return possible_moves;
}

void Game::castle(Color color, bool big) {
	int i = 0, j = 4;
	if (color == Color::White) {
		i = boardSize - 1;
	}
	if (big) {
		board.at(i).at(j - 2).setData(board.at(i).at(j).getData());
		board.at(i).at(j).Empty();
		board.at(i).at(j - 1).setData(board.at(i).at(0).getData());
		board.at(i).at(0).Empty();
	}
	else {
		board.at(i).at(j + 2).setData(board.at(i).at(j).getData());
		board.at(i).at(j).Empty();
		board.at(i).at(j + 1).setData(board.at(i).at(boardSize - 1).getData());
		board.at(i).at(boardSize - 1).Empty();
	}
}

const MoveOutcome Game::move(tuple<unsigned short int, unsigned short int> from, tuple<unsigned short int, unsigned short int> to) {
	vector< tuple<unsigned short int, unsigned short int>> correct_moves = this->getAllCorrectMoves(get<0>(from), get<1>(from));

	if (find(correct_moves.begin(), correct_moves.end(), to) != correct_moves.end()) {

		if (this->board.at(get<0>(from)).at(get<1>(from)).getData().getType() == Type::Rook) {
			for (int i = 0; i <= 1; i++) {
				for (int j = 0; j <= 1; j++) {
					tuple<unsigned short int, unsigned short int> tup{ i * (boardSize - 1),j * (boardSize - 1) };
					if (from == tup) {
						haveRooksMoved.at((i * (boardSize - 1) + 2 * j * (boardSize - 1)) / (boardSize - 1)) = true;
					}
				}
			}
		}

		if (this->board.at(get<0>(from)).at(get<1>(from)).getData().getType() == Type::King && abs(get<1>(from) - get<1>(to)) == 2) {
			if (get<1>(from) - get<1>(to) > 0) {
				castle(this->board.at(get<0>(from)).at(get<1>(from)).getData().getColor(), true);
				movesHistory.push_back("0-0-0");
			}
			else {
				castle(this->board.at(get<0>(from)).at(get<1>(from)).getData().getColor(), false);
				movesHistory.push_back("0-0");
			}
		}
		else {
			this->addHistory(get<0>(from), get<1>(from), get<0>(to), get<1>(to));

			if (this->board.at(get<0>(from)).at(get<1>(from)).getData().getType() == Type::Pawn && this->board.at(get<0>(to)).at(get<1>(to)).isEmpty() && get<1>(from) != get<1>(to)) {
				this->board.at(get<0>(from)).at(get<1>(to)).Empty();
			}

			this->board.at(get<0>(to)).at(get<1>(to)).setData(this->board.at(get<0>(from)).at(get<1>(from)).getData());
			this->board.at(get<0>(from)).at(get<1>(from)).Empty();

			int x = 0;
			if (this->board.at(get<0>(to)).at(get<1>(to)).getData().getColor() == Color::Black) {
				x = boardSize - 1;
			}
			if (this->board.at(get<0>(to)).at(get<1>(to)).getData().getType() == Type::Pawn && get<0>(to) == x) {
				return MoveOutcome::PawnPromote;
			}
		}

		return endMove();
	}
	else {
		return MoveOutcome::InvalidMove;
	}
}

const MoveOutcome Game::endMove() {
	if (this->turn == Color::White) {
		this->turn = Color::Black;
	}
	else {
		this->turn = Color::White;
	}

	if (isKingInCheck(turn)) {
		this->movesHistory.at(movesHistory.size() - 1).push_back('+');
	}

	if (this->noValidMoves(turn)) {
		if (this->movesHistory.at(movesHistory.size() - 1).at(movesHistory.at(movesHistory.size() - 1).size() - 1) == '+') {
			//checkmate
			if (turn == Color::Black) {
				this->movesHistory.push_back("1-0");
				return MoveOutcome::WhiteWin;
			}
			else {
				this->movesHistory.push_back("0-1");
				return MoveOutcome::BlackWin;
			}
		}
		else {
			//stalemate
			this->movesHistory.push_back("1/2-1/2");
			return MoveOutcome::StaleMate;
		}
	}

	return MoveOutcome::ValidMove;
}

void Game::addHistory(const unsigned short int i, const unsigned short int j, const unsigned short int toi, const unsigned short int toj) {
	if (this->board.at(i).at(j).getData().getColor() == Color::White) {
		this->moveCount++;
		this->movesHistory.push_back(to_string(this->moveCount) + ".");
	}
	char letter = 'a' + j;
	string move;
	if (this->board.at(i).at(j).getData().getType() != Type::Pawn && this->historyHelper(i, j, toi, toj)) {
		move.push_back(letter);
	}

	switch (this->board.at(i).at(j).getData().getType()) {
	case Type::Bishop:
		move.push_back('B');
		break;
	case Type::Knight:
		move.push_back('N');
		break;
	case Type::Rook:
		move.push_back('R');
		break;
	case Type::Queen:
		move.push_back('Q');
		break;
	case Type::King:
		move.push_back('K');
		break;
	}

	if (this->board.at(toi).at(toj).isNotEmpty()) {
		if (this->board.at(i).at(j).getData().getType() == Type::Pawn) {
			move.push_back(letter);
		}
		move += 'x';
	}
	letter = 'a' + toj;
	move.push_back(letter);
	move += to_string(this->boardSize - toi);
	movesHistory.push_back(move);
}

const bool Game::historyHelper(const unsigned short int x, const unsigned short int y, const unsigned short int toi, const unsigned short int toj) {

	Type type = this->board.at(x).at(y).getData().getType();

	for (int i = 0; i < this->boardSize; i++) {
		for (int j = 0; j < this->boardSize; j++) {
			if (this->board.at(i).at(j).isNotEmpty() && this->board.at(i).at(j).getData().getType() == type && (i != x || j != y)) {
				auto moves = this->getAllCorrectMoves(i, j);
				tuple<unsigned short int, unsigned short int> t{ toi,toj };
				if (find(moves.begin(), moves.end(), t) != moves.end()) {
					return true;
				}
			}
		}
	}

	return false;
}

const tuple<unsigned short int, unsigned short int> Game::getPromotedPawnPositions() {
	int i = 0;
	if (this->turn == Color::Black) {
		i = boardSize - 1;
	}
	for (int j = 0; j < boardSize - 1; j++) {
		if (board.at(i).at(j).isNotEmpty() && board.at(i).at(j).getData().getType() == Type::Pawn) {
			return tuple<unsigned short int, unsigned short int>{i, j};
		}
	}
}

const MoveOutcome Game::promote(const short int i, const short int j, ChessPiece piece) {
	board.at(i).at(j).setData(piece);
	this->movesHistory.at(movesHistory.size() - 1).push_back('=');

		switch (piece.getType()) {
		case Type::Bishop:
			this->movesHistory.at(movesHistory.size() - 1).push_back('B');
			break;
		case Type::Knight:
			this->movesHistory.at(movesHistory.size() - 1).push_back('N');
			break;
		case Type::Rook:
			this->movesHistory.at(movesHistory.size() - 1).push_back('R');
			break;
		case Type::Queen:
			this->movesHistory.at(movesHistory.size() - 1).push_back('Q');
			break;
		}

	return endMove();
}

const bool Game::noValidMoves(Color color) {

	for (int i = 0; i < this->boardSize; i++) {
		for (int j = 0; j < this->boardSize; j++) {
			if (this->board.at(i).at(j).isNotEmpty() && this->board.at(i).at(j).getData().getColor() == color) {
				vector< tuple<unsigned short int, unsigned short int>> possible_moves = this->getAllCorrectMoves(i, j);
				if (possible_moves.size() > 0) {
					return false;
				}
			}
		}
	}
		return true;
}

const vector< tuple<unsigned short int, unsigned short int>> Game::getAllCorrectMoves(const unsigned short int i, const unsigned short int j) {
	Color color = this->board.at(i).at(j).getData().getColor();
	vector< tuple<unsigned short int, unsigned short int>> correct_moves;
	if (color != this->turn) {
		return correct_moves;
	}

	vector< tuple<unsigned short int, unsigned short int>> possible_moves = this->getAllPossibleMoves(i, j);

	Option<ChessPiece> piece = this->board.at(i).at(j);
	for (auto pos : possible_moves) {
		Option<ChessPiece> aux = this->board.at(get<0>(pos)).at(get<1>(pos));
		Option<ChessPiece> auxForEnPassant = this->board.at(i).at(get<1>(pos));
		if (piece.getData().getType() == Type::Pawn && aux.isEmpty() && get<1>(pos) != j) {
			this->board.at(i).at(get<1>(pos)).Empty();
		}
		this->board.at(get<0>(pos)).at(get<1>(pos)) = piece;
		this->board.at(i).at(j).Empty();
		if (!this->isKingInCheck(color)) {
			correct_moves.push_back(pos);
		}
		this->board.at(get<0>(pos)).at(get<1>(pos)) = aux;
		this->board.at(i).at(j) = piece;
		this->board.at(i).at(get<1>(pos)) = auxForEnPassant;
	}
	vector< tuple<unsigned short int, unsigned short int>> castle_moves = checkCastles(color);
	correct_moves.insert(correct_moves.end(), castle_moves.begin(), castle_moves.end());

	return correct_moves;
}

const bool Game::isKingInCheck(Color color) {
	tuple<unsigned short int, unsigned short int> king_position = this->getKingPosition(color);

	vector< tuple<unsigned short int, unsigned short int>> possible_checks;

	for (int i = 0; i < this->boardSize; i++) {
		for (int j = 0; j < this->boardSize; j++) {
			if (!this->board.at(i).at(j).isEmpty() && this->board.at(i).at(j).getData().getColor() != color) {
				vector< tuple<unsigned short int, unsigned short int>> possible_moves = this->getAllPossibleMoves(i, j);
				possible_checks.insert(possible_checks.end(), possible_moves.begin(), possible_moves.end());
			}
		}
	}

	for (auto pos : possible_checks) {
		if (pos == king_position) {
			return true;
		}
	}

	return false;
}

const bool Game::hasKingMoved(Color color) {
	int i = 0, j = 4;
	if (color == Color::White) {
		i = boardSize - 1;
	}

	if (board.at(i).at(j).isNotEmpty() && board.at(i).at(j).getData().getType() == Type::King && board.at(i).at(j).getData().getColor() == color) {

		for (int k = -1; k <= 1; k++) {
			for (int l = -1; l <= 1; l++) {
				if (!(k == 0 && l == 0) && 0 <= i + k && i + k < this->boardSize && 0 <= j + l && j + l < this->boardSize) {
					string possible_move = "K";
					char letter = 'a' + j + l;
					possible_move.push_back(letter);
					possible_move += to_string(boardSize - (i + k));

					for (string move : movesHistory) {
						move.erase(remove(move.begin(), move.end(), 'x'), move.end());
						move.erase(remove(move.begin(), move.end(), '+'), move.end());
						if (move == possible_move) {
							return true;
						}
					}
				}

			}
		}

		return false;
	}

	return true;
}

const tuple<unsigned short int, unsigned short int> Game::getKingPosition(Color color) {
	tuple<unsigned short int, unsigned short int> kingPosition;
	ChessPiece king(color, Type::King);
	for (int i = 0; i < this->boardSize; i++) {
		for (int j = 0; j < this->boardSize; j++) {
			if (this->board.at(i).at(j).isNotEmpty() && this->board.at(i).at(j).getData() == king) {
				get<0>(kingPosition) = i;
				get<1>(kingPosition) = j;
				return kingPosition;
			}
		}
	}
	return kingPosition;
}


Game::~Game() {

}