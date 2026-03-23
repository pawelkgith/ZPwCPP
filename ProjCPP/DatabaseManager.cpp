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

bool DatabaseManager::createTables() {
	QSqlQuery query;
	
	QString createPlayers = "CREATE TABLE IF NOT EXISTS Gracze(\n"
		"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
		"nazwa TEXT UNIQUE NOT NULL\n"
		")";

	if (!query.exec(createPlayers)) {
		qDebug() << "Blad przy tworzeniu tabeli Gracze";
		return false;
	}

	QString createMatches = "CREATE TABLE IF NOT EXISTS Mecze(\n"
		"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
		"gracz1_id INTEGER,\n"
		"gracz2_id INTEGER,\n"
		"zwyciezca_id INTEGER,\n"
		"liczba_ruchow INTEGER,\n"
		"data_rozgrywki DATETIME DEFAULT CURRENT_TIMESTAMP,\n"
		"FOREIGN KEY(gracz1_id) REFERENCES Gracze(id),\n"
		"FOREIGN KEY(gracz2_id) REFERENCES Gracze(id),\n"
		"FOREIGN KEY(zwyciezca_id) REFERENCES Gracze(id)\n"
		")";

	if (!query.exec(createMatches)) {
		qDebug() << "Blad przy tworzeniu tabeli Mecze";
		return false;
	}

	QString createMoves = "CREATE TABLE IF NOT EXISTS Ruchy(\n"
		"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
		"mecz_id INTEGER,\n"
		"gracz_id INTEGER,\n"
		"numer_tury INTEGER,\n"
		"wiersz INTEGER,\n"
		"kolumna INTEGER,\n"
		"FOREIGN KEY(mecz_id) REFERENCES Mecze(id),\n"
		"FOREIGN KEY(gracz_id) REFERENCES Gracze(id)\n"
		")";

	if (!query.exec(createMoves)) {
		qDebug() << "Blad przy tworzeniu tabeli Ruchy";
		return false;
	}

	return true;
}

int DatabaseManager::getPlayer(const QString& playerName) {
	QSqlQuery query;

	query.prepare("SELECT id FROM Gracze WHERE nazwa = :nazwa");
	query.bindValue(":nazwa", playerName);

	if (query.exec() && query.next()) {
		return query.value(0).toInt(); //zwracamy id znalezionego gracza
	}

	//jesli nie znaleziono
	query.prepare("INSERT INTO Gracze (nazwa) VALUES (:nazwa)");
	query.bindValue(":nazwa", playerName);

	if (query.exec()) {
		return query.lastInsertId().toInt();
	}

	qDebug() << "Blad przy dodawaniu gracza";
	return -1;
}

int DatabaseManager::saveMatch(int player1_id, int player2_id, int winner_id, int moves_count) {
	QSqlQuery query;

	query.prepare("INSERT INTO Mecze (gracz1_id, gracz2_id, zwyciezca_id, liczba_ruchow) "
		"VALUES (:p1, :p2, :win, :moves)");

	query.bindValue(":p1", player1_id);
	query.bindValue(":p2", player2_id);
	
	//jesli remis
	if (winner_id == -1)
		query.bindValue(":win", QVariant());
	else
		query.bindValue(":win", winner_id);

	query.bindValue(":moves", moves_count);

	if (query.exec())
		return query.lastInsertId().toInt();

	qDebug() << "Blad przy zapisywaniu meczu";
	return -1;
}

void DatabaseManager::saveMove(int match_id, int player_id, int turn_number, int row, int col) {
	QSqlQuery query;

	query.prepare("INSERT INTO Ruchy (mecz_id, gracz_id, numer_tury, wiersz, kolumna)"
		"VALUES (:match, :player, :turn, :row, :col)");

	query.bindValue(":match", match_id);
	query.bindValue(":player", player_id);
	query.bindValue(":turn", turn_number);
	query.bindValue(":row", row);
	query.bindValue(":col", col);

	if (!query.exec()) {
		qDebug() << "Blad przy zapisywaniu ruchu";
	}
}

void DatabaseManager::clearDatabase() {
	QSqlQuery query;

	query.exec("DELETE FROM Ruchy");
	query.exec("DELETE FROM Mecze");
	query.exec("DELETE FROM Gracze");
	query.exec("DELETE FROM sqlite_sequence");
}