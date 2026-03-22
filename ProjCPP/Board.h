#ifndef BOARD_H
#define BOARD_H

#include <qchar.h>

class Board {
private:
	int size;
	QChar** grid;
public:
	Board(int boardSize = 10);
	void display();
	void updateCell(int row, int col, QChar symbol);
	bool isCellEmpty(int row, int col);
	bool isFull();
	QChar getCell(int row, int col);
	int getSize() const;
	void clear();
	~Board();
};

#endif