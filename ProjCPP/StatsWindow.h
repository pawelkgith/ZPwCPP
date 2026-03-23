#pragma once
#include <QDialog>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <vector>
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>
#include <numeric>
#include <cmath>
#include <algorithm>

class StatsWindow : public QDialog
{
private:
	DatabaseManager* db;
	QTableView* tableView;
	QSqlQueryModel* tableModel;
	QSortFilterProxyModel* proxyModel;
	QLabel* statsLabel;
	QPushButton* closeButton;

	void loadData();
	void showStats();

public:
	StatsWindow(DatabaseManager* dbManager, QWidget* parent = nullptr);
	~StatsWindow();
};

