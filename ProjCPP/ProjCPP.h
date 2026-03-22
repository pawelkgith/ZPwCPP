#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProjCPP.h"

class ProjCPP : public QMainWindow
{
    Q_OBJECT

public:
    ProjCPP(QWidget *parent = nullptr);
    ~ProjCPP();

private:
    Ui::ProjCPPClass ui;
};

