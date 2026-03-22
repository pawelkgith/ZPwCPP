#pragma once
#include <QtWidgets>
class StartDialog
{
private:
	QLineEdit* pvpPlayer1Name;
	QLineEdit* pvpPlayer2Name;
	QLineEdit* pvcPlayerName;
	QLineEdit* pvcComputerName;
	QSpinBox* boardSizeWidget;
	QWidget* pvpWidget;
	QWidget* pvcWidget;
	QPushButton* startButton;

	QString gameMode;
};

