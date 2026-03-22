#pragma once
#include "Board.h"
#include "Player.h"
#include <vector>

struct MoveRecord {
	Player* player;
	int row;
	int col;
};

class Game {
private:
	Board* board;
	Player* player1;
	Player* player2;
	Player* currentPlayer;
	bool vsComputer;
	bool isP1Next = true;
	std::vector<MoveRecord> moveHistory;

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
	std::vector<MoveRecord> getMoveHistory() { return moveHistory; }
	~Game();
};
