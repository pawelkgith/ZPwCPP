#pragma execution_character_set("utf-8")
#include "GameWindow.h"

GameWindow::GameWindow(QString p1Name, QString p2Name, int size, bool vsComp, QString mode, DatabaseManager* dbManager)
	: boardSize(size), vsComputer(vsComp), gameMode(mode), gameFinished(false), db(dbManager)
{
    // tworzenie obiektow
    player1 = new Player(p1Name, 'X');
    player2 = new Player(p2Name, 'O');
    game = new Game(player1, player2, boardSize, vsComputer);

    // elementy glownego okna gry
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QVBoxLayout* globalLayout = new QVBoxLayout(centralWidget);
    QWidget* gridContainer = new QWidget();
    QScrollArea* scrollArea = new QScrollArea();

    statusLabel = new QLabel();
    newGameButton = new QPushButton();
    gridLayout = new QGridLayout(gridContainer);
    QHBoxLayout* statusLayout = new QHBoxLayout();

    cells = new QPushButton * *[boardSize];
    for (int i = 0; i < boardSize; i++) {
        cells[i] = new QPushButton * [boardSize];
        for (int j = 0; j < boardSize; j++) {
            cells[i][j] = new QPushButton();
        }
    }

    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // ustaw label
    statusLabel->setText("Teraz gra: " + game->getCurrentPlayer()->getName());
    statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");
    newGameButton->setText("Nowa gra");
    newGameButton->setFixedWidth(120);

    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(newGameButton);

    // scroll setup
    scrollArea->setWidget(gridContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    globalLayout->addLayout(statusLayout);
    globalLayout->addWidget(scrollArea);

    // startowe ustawienia (bugfix)
    game->resetGame();

    // odśwież planszę
    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            QChar cell = game->getBoard().getCell(r, c);
            if (cell != ' ') {
                if (cell == 'X')
                    cells[r][c]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: blue; }");
                else
                    cells[r][c]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: red; }");
            }
            else {
                cells[r][c]->setText("");
                cells[r][c]->setStyleSheet(cellStyle);
            }
        }
    }

    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {

            cells[row][col]->setStyleSheet(cellStyle);
            cells[row][col]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            cells[row][col]->setMinimumSize(30, 30);

            gridLayout->addWidget(cells[row][col], row, col);

            QChar cell = game->getBoard().getCell(row, col);
            cells[row][col]->setText(cell == ' ' ? "" : QString(cell));

            connect(cells[row][col], &QPushButton::clicked, [this, row, col]() {
                if (gameFinished)
                    return;

                // logika gry pvp
                if (game->makeMove(row, col)) {
                    QString symbol = QString(game->getCurrentPlayer()->getSymbol());
                    cells[row][col]->setText(symbol);

                    if (symbol == "X") cells[row][col]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: blue }");
                    else cells[row][col]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: red }");

                    // sprawdzenie wygranej
                    int winner = game->checkWinner();
                    if (winner != 0) {
                        saveGameToDatabase(winner);
                        QString winnerName = (winner == 1) ?
                            "" + player1->getName() + "(" + QString(player1->getSymbol()) + ")" :
                            "" + player2->getName() + "(" + QString(player2->getSymbol()) + ")";
                        QMessageBox::information(nullptr, "Koniec gry!", winnerName + " wygrywa!", QMessageBox::Ok);
                        gameFinished = true;
                    }
                    else if (game->isDraw()) {
                        saveGameToDatabase(0);
                        QMessageBox::information(nullptr, "Koniec gry!", "Remis!", QMessageBox::Ok);
                        gameFinished = true;
                    }
                    else {
                        // zmiana gracza
                        game->switchPlayer();
                        statusLabel->setText("Teraz gra: " + game->getCurrentPlayer()->getName());

                        // logika gry pvc
                        if (vsComputer && game->getCurrentPlayer()->getName() == "Komputer") {
                            // ruch komputera
                            game->computerMove();

                            // odswiezenie planszy
                            for (int r = 0; r < boardSize; r++) {
                                for (int c = 0; c < boardSize; c++) {
                                    if (cells[r][c]->text().isEmpty() && !game->getBoard().isCellEmpty(r, c)) {
                                        cells[r][c]->setText(QString(game->getCurrentPlayer()->getSymbol()));
                                        cells[r][c]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: red }");
                                    }
                                }
                            }

                            // sprawdzenie wygranej po ruchu komputera
                            int winner = game->checkWinner();
                            if (winner != 0) {
                                saveGameToDatabase(winner);
                                QString winnerName = (winner == 1) ? player1->getName() : player2->getName();
                                QMessageBox::information(nullptr, "Koniec gry!", winnerName + " wygrywa!", QMessageBox::Ok);
                                gameFinished = true;
                                statusLabel->setText("Koniec gry! " + winnerName + " wygrał!");
                            }
                            else if (game->isDraw()) {
                                saveGameToDatabase(1);
                                QMessageBox::information(nullptr, "Koniec gry!", "Remis!", QMessageBox::Ok);
                                gameFinished = true;
                                statusLabel->setText("Koniec gry! Remis!");
                            }
                            else {
                                game->switchPlayer();
                                statusLabel->setText("Teraz gra: " + game->getCurrentPlayer()->getName());
                            }
                        }
                    }
                }
                });
        }
        gridLayout->setRowStretch(row, 1);
        gridLayout->setColumnStretch(row, 1);
    }
    
    //przycisk nowa gra
    connect(newGameButton, &QPushButton::clicked, [this]() {
        gameFinished = false;
        game->resetGame();
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                cells[i][j]->setText("");
                cells[i][j]->setStyleSheet(cellStyle);
            }
        }

        // odswiez po pierwszym ruchu komputera jesli zaczyna
        for (int r = 0; r < boardSize; r++) {
            for (int c = 0; c < boardSize; c++) {
                QChar cell = game->getBoard().getCell(r, c);
                if (cell != ' ') {
                    cells[r][c]->setText(QString(cell));
                    if (cell == 'X')
                        cells[r][c]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: blue; }");
                    else
                        cells[r][c]->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 0px; font-size: 18px; font-weight: bold; color: red; }");
                }
            }
        }

        statusLabel->setText("Teraz gra: " + game->getCurrentPlayer()->getName());
    });

    resize(600, 650);
    setWindowTitle("Kółko i krzyżyk " + gameMode);
}

void GameWindow::saveGameToDatabase(int winnerFlag) {
    int p1_id = db->getPlayer(player1->getName());
    int p2_id = db->getPlayer(player2->getName());

    int winner_id = -1; // remis
    if (winnerFlag == 1) winner_id = p1_id;
    else if (winnerFlag == 2) winner_id = p2_id;

    std::vector<MoveRecord> history = game->getMoveHistory();

    int match_id = db->saveMatch(p1_id, p2_id, winner_id, history.size());

    if (match_id == -1) {
        qDebug() << "Nie udalo sie utworzyc meczu w bazie";
        return;
    }

    for (size_t i = 0; i < history.size(); i++) {
        int move_player_id = (history[i].player == player1) ? p1_id : p2_id;

        db->saveMove(match_id, move_player_id, i + 1, history[i].row, history[i].col);
    }

    qDebug() << "Gra zapisana";
}

GameWindow::~GameWindow() {
    delete game;
    delete player1;
    delete player2;

    for (int i = 0; i < boardSize; i++) {
        delete[] cells[i];
    }
    delete[] cells;
}
