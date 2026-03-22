#include "Board.h"
#include <iostream>
using namespace std;

Board::Board(int boardSize) : size(boardSize) {
	grid = new QChar * [size];
	for (int i = 0; i < size; i++) {
		grid[i] = new QChar[size];
		for (int j = 0; j < size; j++) {
			this->grid[i][j] = ' ';
		}
	}
}

void Board::display() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << (this->grid[i][j]).toLatin1() << " ";
		}
		cout << endl;
	}
}

void Board::updateCell(int row, int col, QChar symbol) {
	if(row >= 0 && row < size && col >= 0 && col < size)
		this->grid[row][col] = symbol;
}

bool Board::isCellEmpty(int row, int col) {
	return (row >= 0 && row < size && col >= 0 && col < size) && grid[row][col] == ' ';

}

bool Board::isFull() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (this->grid[i][j] == ' ') {
				return false;
			}
		}
	}
	return true;
}

QChar Board::getCell(int row, int col) {
	return grid[row][col];
}

int Board::getSize() const {
	return size;
}

void Board::clear() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			grid[i][j] = ' ';
		}
	}
}

Board::~Board() {
	for (int i = 0; i < size; i++) {
		delete[] grid[i];
	}
	delete[] grid;
}