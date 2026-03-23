#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QApplication>
#include <QtWidgets>
#include <DatabaseManager.h>

class StartMenu : public QDialog
{
public:
    QString gameMode;
    QString player1Name;
    QString player2Name;
    bool vsComputer = false;
    int boardSize = 10;
    StartMenu(DatabaseManager* dbManager);

private:
    DatabaseManager* db;
    QLineEdit* pvpPlayer1NameEdit;
    QLineEdit* pvpPlayer2NameEdit;
    QLineEdit* pvcPlayerNameEdit;
    QLineEdit* pvcComputerNameEdit;
    QSpinBox* boardSizeWidget;
    QWidget* pvpWidget;
    QWidget* pvcWidget;
    QPushButton* startButton;
    QPushButton* statsButton;
};

