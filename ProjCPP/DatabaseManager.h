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
};

