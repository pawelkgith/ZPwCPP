#include "Game.h"
#include "qdebug.h"

Game::Game(Player* p1, Player* p2, int boardSize, bool vsComp) {
	this->player1 = p1;
	this->player2 = p2;
	this->board = new Board(boardSize);
	this->currentPlayer = p1;
	this->vsComputer = vsComp;
	this->isP1Next = true;
}

void Game::switchPlayer() {
	if (this->currentPlayer == this->player1)
		this->currentPlayer = this->player2;
	else
		this->currentPlayer = this->player1;
}

int Game::checkWinner() {
	const int WIN_LENGTH = 5;
	int directions[4][2] = { {0,1}, {1,0}, {1,1}, {1,-1} }; //prawo, dol, prawy skos do dolu, lewy skos do dolu
	int size = board->getSize();

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (!board->isCellEmpty(i, j)) {
				for (int d = 0; d < 4; d++) { //directions
					int count = 1; //liczba pol spelniajacych warunek wygranej
					for (int k = 1; k < WIN_LENGTH; k++) {
						int ni = i + directions[d][0] * k; //nowy wiersz
						int nj = j + directions[d][1] * k; //nowa kolumna

						//sprawdzenie czy sprawdzana wspolrzedna jest w granicach planszy oraz czy jest takiego samego znaku jak komorka macierzysta
						if (ni >= 0 && ni < size && nj >= 0 && nj < size && board->getCell(ni, nj) == board->getCell(i, j))
							count++;
						else
							break;
					}
					//sprawdzenie czy jest 5 pol w tej samej linii
					if (count == WIN_LENGTH)
						return (board->getCell(i, j) == 'X' ? 1 : 2);
				}
			}
		}
	}
	return 0;
}

bool Game::isDraw() {
	return board->isFull();
}

Player* Game::getCurrentPlayer() {
	return currentPlayer;
}

bool Game::makeMove(int row, int col) {
	if (board->isCellEmpty(row, col)) {
        moveHistory.push_back({ currentPlayer, row, col });
		currentPlayer->makeMove(*board, row, col);
		return true;
	}
	return false;
}

bool Game::isVsComputer() { 
	return vsComputer; 
}

long calculateScore(Board& board, int r, int c, QChar playerSym) {
    long score = 0;

    // poziomo, pionowo, skos prawy, skos lewy
    int directions[4][2] = { {0,1}, {1,0}, {1,1}, {1,-1} };

    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        int rowCount = 0; //ile w rzedzie
        int openEnds = 0; //ile wolnych koncow

        //prawa strona
        int i = 1;
        while (true) {
            int nr = r + i * dx;
            int nc = c + i * dy;
            if (nr >= 0 && nr < board.getSize() && nc >= 0 && nc < board.getSize()) {
                if (board.getCell(nr, nc) == playerSym) {
                    rowCount++;
                }
                else if (board.getCell(nr, nc) == ' ') {
                    openEnds++;
                    break; //koniec ciagu, ale open end
                }
                else {
                    break; //zablokowane
                }
            }
            else {
                break; //krawedz
            }
            i++;
        }

        //lewa
        i = 1;
        while (true) {
            int nr = r - i * dx;
            int nc = c - i * dy;
            if (nr >= 0 && nr < board.getSize() && nc >= 0 && nc < board.getSize()) {
                if (board.getCell(nr, nc) == playerSym) {
                    rowCount++;
                }
                else if (board.getCell(nr, nc) == ' ') {
                    openEnds++;
                    break;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
            i++;
        }

        int currentLen = rowCount + 1; //+1, bo wliczamy ruch ktory jest aktualnie symulwoany

        if (currentLen >= 5) score += 100000000;
        else if (currentLen == 4) {
            if (openEnds == 2) score += 500000;  //otwarta czworka
            else if (openEnds == 1) score += 10000; //zamknieta czworka
        }
        else if (currentLen == 3) {
            if (openEnds == 2) score += 5000;   //otwarta trojka
            else if (openEnds == 1) score += 100;
        }
        else if (currentLen == 2) {
            if (openEnds == 2) score += 50;
        }
    }
    return score;
}

void Game::computerMove() {
    int size = board->getSize();
    long bestScore = -1;
    int bestRow = -1;
    int bestCol = -1;

    //zaczynamy od srodka jesli plansza jest pusta
    if (board->isCellEmpty(size / 2, size / 2)) {
        makeMove(size / 2, size / 2);
        return;
    }

    QChar computerSym = getCurrentPlayer()->getSymbol();
    QChar opponentSym = (computerSym == 'X') ? 'O' : 'X';

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (!board->isCellEmpty(i, j)) continue;

            long currentScore = 0;

            long attackScore = calculateScore(*board, i, j, computerSym);

            long defenseScore = calculateScore(*board, i, j, opponentSym);

            //priorytet ataku
            if (attackScore >= 100000000) currentScore = attackScore;
            //priorytet obrony
            else if (defenseScore >= 100000000) currentScore = defenseScore;
            else {
                currentScore = attackScore + (defenseScore * 1.1); //lekka przewaga dla obrony
            }

            //bonus pozycyjny, dodatkowe punkty za srodek
            int distCenter = abs(i - size / 2) + abs(j - size / 2);
            currentScore += (100 - distCenter);

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestRow = i;
                bestCol = j;
            }
        }
    }

    if (bestRow != -1 && bestCol != -1) {
        makeMove(bestRow, bestCol);
    }
}

Board& Game::getBoard() {
	return *board;
}

void Game::resetGame() {
	board->clear();
    moveHistory.clear();
	
	currentPlayer = isP1Next ? player1 : player2;

	isP1Next = !isP1Next;

	if (vsComputer && currentPlayer == player2) {
		computerMove();
		switchPlayer();
	}
}

Game::~Game() {
	delete board;
}