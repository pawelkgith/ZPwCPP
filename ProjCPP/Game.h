#pragma once
#include "Board.h"
#include "Player.h"

class Game {
private:
	Board* board;
	Player* player1;
	Player* player2;
	Player* currentPlayer;
	bool vsComputer;
	bool isP1Next = true;

public:
	Game(Player* p1, Player* p2, int boardSize, bool vsComp = false);
	void switchPlayer();
	int checkWinner();
	bool isDraw();
	Player* getCurrentPlayer();
	bool makeMove(int row, int col);
	bool isVsComputer();
	void computerMove();
	Board& getBoard();
	void resetGame();
	~Game();
};
