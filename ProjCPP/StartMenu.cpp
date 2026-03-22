#pragma execution_character_set("utf-8")
#include "StartMenu.h"
#include <QtWidgets/QApplication>
#include <QtWidgets>
#include "Player.h"
#include "Board.h"
#include "Game.h"

StartMenu::StartMenu() {
    setWindowTitle("Kółko i Krzyżyk - Menu");
    setFixedSize(400, 450);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QVBoxLayout* pvpLayout = new QVBoxLayout();
    QVBoxLayout* pvcLayout = new QVBoxLayout();
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    pvpWidget = new QWidget();
    pvcWidget = new QWidget();

    // tytul
    QLabel* titleLabel = new QLabel("KÓŁKO I KRZYŻYK");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // przyciski trybow gry
    QPushButton* pvpButton = new QPushButton("Graj ze znajomym");
    QPushButton* pvcButton = new QPushButton("Graj z komputerem");
    pvpButton->setFixedHeight(40);
    pvcButton->setFixedHeight(40);

    // pola tekstowe pvp
    pvpPlayer1NameEdit = new QLineEdit();
    pvpPlayer2NameEdit = new QLineEdit();
    pvpPlayer1NameEdit->setPlaceholderText("Nazwa gracza 1");
    pvpPlayer2NameEdit->setPlaceholderText("Nazwa gracza 2");
    pvpPlayer1NameEdit->setText("Gracz 1");
    pvpPlayer2NameEdit->setText("Gracz 2");

    // pola tekstowe pvc
    pvcPlayerNameEdit = new QLineEdit();
    pvcComputerNameEdit = new QLineEdit();
    pvcPlayerNameEdit->setPlaceholderText("Nazwa gracza 1");
    pvcComputerNameEdit->setPlaceholderText("Nazwa gracza 2");
    pvcPlayerNameEdit->setText("Gracz");
    pvcComputerNameEdit->setText("Komputer");
    pvcComputerNameEdit->setReadOnly(true);

    // pole tekstowe dla rozmiaru planszy
    boardSizeWidget = new QSpinBox();
    boardSizeWidget->setRange(10, 50);
    boardSizeWidget->setValue(10);
    boardSizeWidget->setFixedWidth(100);
    boardSizeWidget->setAlignment(Qt::AlignCenter);

    // skladanie widgetow
    pvpLayout->addWidget(pvpPlayer1NameEdit);
    pvpLayout->addWidget(pvpPlayer2NameEdit);
    pvpWidget->setLayout(pvpLayout);

    pvcLayout->addWidget(pvcPlayerNameEdit);
    pvcLayout->addWidget(pvcComputerNameEdit);
    pvcWidget->setLayout(pvcLayout);

    // przycisk startu
    startButton = new QPushButton("Rozpocznij gre");
    startButton->setFixedHeight(40);

    // ukrywanie widgetów na poczatku
    pvpWidget->setVisible(false);
    pvcWidget->setVisible(false);
    startButton->setVisible(false);

    // skladanie glownego layoutu
    buttonsLayout->addWidget(pvpButton);
    buttonsLayout->addWidget(pvcButton);
    sizeLayout->addWidget(boardSizeWidget, 0, Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(pvpWidget);
    mainLayout->addWidget(pvcWidget);
    mainLayout->addLayout(sizeLayout);
    mainLayout->addWidget(startButton);
    mainLayout->addStretch();

    setLayout(mainLayout);

    connect(pvpButton, &QPushButton::clicked, [this]() {
        gameMode = "PvP";
        vsComputer = false;
        pvpWidget->setVisible(true);
        pvcWidget->setVisible(false);
        startButton->setVisible(true);
        });

    connect(pvcButton, &QPushButton::clicked, [this]() {
        gameMode = "PvC";
        vsComputer = true;
        pvcWidget->setVisible(true);
        pvpWidget->setVisible(false);
        startButton->setVisible(true);
        });

    connect(startButton, &QPushButton::clicked, [this]() {
        boardSize = boardSizeWidget->value();

        if (gameMode == "PvP") {
            player1Name = pvpPlayer1NameEdit->text().isEmpty() ? "Gracz 1" : pvpPlayer1NameEdit->text();
            player2Name = pvpPlayer2NameEdit->text().isEmpty() ? "Gracz 2" : pvpPlayer2NameEdit->text();
        }
        else if (gameMode == "PvC") {
            player1Name = pvcPlayerNameEdit->text().isEmpty() ? "Gracz" : pvcPlayerNameEdit->text();
            player2Name = "Komputer";
        }
        accept(); // zamknij menu
    });
}