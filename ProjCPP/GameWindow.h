#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets>
#include "Player.h"
#include "Board.h"
#include "Game.h"

class GameWindow : public QMainWindow
{
private:
	QString gameMode;
	int boardSize;
	bool vsComputer;
	bool gameFinished;

	Player* player1;
	Player* player2;
	Game* game;

    QPushButton*** cells;
    QLabel* statusLabel;
    QPushButton* newGameButton;
    QGridLayout* gridLayout;

    QString cellStyle =
        "QPushButton {"
        "border: 1px solid black;"
        "border-radius: 0px;"
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #333;"
        "}";

public:
    GameWindow(QString p1Name, QString p2Name, int size, bool vsComp, QString mode);
    ~GameWindow();
};

