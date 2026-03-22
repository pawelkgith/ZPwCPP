#ifndef PLAYER_H
#define PLAYER_H

#include <qstring.h>
#include <qchar.h>
#include "Board.h"

class Player {
private:
	QString name;
	QChar symbol;

public:
	Player(QString playerName, QChar playerSymbol);
	QString getName();
	QChar getSymbol();
	void makeMove(Board& board, int row, int col);
};

#endif