#pragma execution_character_set("utf-8")
#include "ProjCPP.h"
#include <QtWidgets/QApplication>
#include <QtWidgets>
#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "StartMenu.h"
#include "GameWindow.h"
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    DatabaseManager dbManager;
    if (dbManager.connectDatabase()) {
        qDebug() << "Baza gotowa";
    }

    StartMenu menu;
    if (menu.exec() != QDialog::Accepted) {
        return 0;
    }

    GameWindow window(
        menu.player1Name,
        menu.player2Name,
        menu.boardSize,
        menu.vsComputer,
        menu.gameMode
    );

    window.show();
    
    return app.exec();
}
