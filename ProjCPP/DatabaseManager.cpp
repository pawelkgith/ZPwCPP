#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() {
	db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager() {
	if (db.isOpen()) {
		db.close();
	}
}

bool DatabaseManager::connectDatabase() {
	QString path = QCoreApplication::applicationDirPath() + "/mainDB.db";

	db.setDatabaseName(path);

	if (!db.open()) {
		qDebug() << "Blad polaczenia z baza" << db.lastError().text();
		return false;
	}

	qDebug() << "Polaczono z baza";
	return true;
}