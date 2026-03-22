#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

class DatabaseManager
{
private:
	QSqlDatabase db;

public:
	DatabaseManager();
	~DatabaseManager();

	bool connectDatabase();
	bool createTables();
	int getPlayer(const QString& playerName);
	int saveMatch(int player1_id, int player2_id, int winner_id, int moves_count);
	void saveMove(int match_id, int player_id, int turn_number, int row, int col);
	void clearDatabase();
};

