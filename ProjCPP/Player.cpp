#include "Player.h"
#include "Board.h"

Player::Player(QString playerName, QChar playerSymbol) {
	this->name = playerName;
	this->symbol = playerSymbol;
}

QString Player::getName() {
	return this->name;
}

QChar Player::getSymbol() {
	return this->symbol;
}

void Player::makeMove(Board& board, int row, int col) {
	board.updateCell(row, col, this->symbol);
}