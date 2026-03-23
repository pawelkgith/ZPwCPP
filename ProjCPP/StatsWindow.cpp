#include "StatsWindow.h"

StatsWindow::StatsWindow(DatabaseManager* dbManager, QWidget* parent) : QDialog(parent), db(dbManager) {
	setWindowTitle("Statystyki rozgrywek");
	resize(700, 500);

	QVBoxLayout* layout = new QVBoxLayout(this);
	tableView = new QTableView(this);
	tableModel = new QSqlQueryModel(this);
	proxyModel = new QSortFilterProxyModel(this);

	statsLabel = new QLabel("£adowanie statystyk", this);
	statsLabel->setStyleSheet("font-size: 14px; padding: 8px; background-color: grey; border: 1px solid black;");

	closeButton = new QPushButton("Zamknij", this);
	connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

	layout->addWidget(tableView);
	layout->addWidget(statsLabel);
	layout->addWidget(closeButton);

	loadData();
	showStats();
}

StatsWindow::~StatsWindow() {}

void StatsWindow::loadData() {
	//polaczenie meczów i graczy zeby miec ich nazwy zamiast id
	QString query = "SELECT Mecze.id AS ID, G1.Nazwa AS Gracz_1, G2.nazwa AS Gracz_2, "
		"IFNULL(Z.nazwa, 'Remis') AS Zwyciezca, Mecze.liczba_ruchow AS Ruchy, Mecze.data_rozgrywki AS Data "
		"FROM Mecze "
		"JOIN Gracze G1 ON Mecze.gracz1_id = G1.id "
		"JOIN Gracze G2 ON Mecze.gracz2_id = G2.id "
		"LEFT JOIN Gracze Z ON Mecze.zwyciezca_id = Z.id";

	tableModel->setQuery(query);

	if (tableModel->lastError().isValid()) {
		qDebug() << "Blad ladowania danych do tabeli";
	}

	proxyModel->setSourceModel(tableModel);
	tableView->setModel(proxyModel);

	tableView->setSortingEnabled(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->resizeColumnsToContents();
}

void StatsWindow::showStats() {
	QSqlQuery query("SELECT liczba_ruchow FROM Mecze");
	std::vector<int> moves;

	while (query.next()) {
		moves.push_back(query.value(0).toInt());
	}

	if (moves.empty()) {
		statsLabel->setText("Brak rozegranych meczow");
		return;
	}

	int minMoves = *std::min_element(moves.begin(), moves.end());
	int maxMoves = *std::max_element(moves.begin(), moves.end());

	double sum = std::accumulate(moves.begin(), moves.end(), 0.0);
	double avg = sum / moves.size();

	std::sort(moves.begin(), moves.end());
	double median = 0;
	if (moves.size() % 2 == 0)
		median = (moves[moves.size() / 2 - 1] + moves[moves.size() / 2]) / 2;
	else
		median = moves[moves.size() / 2];

	double varianceSum = 0;
	for (int m : moves) {
		varianceSum += (m - avg) * (m - avg);
	}

	double variance = std::sqrt(varianceSum / moves.size());

	QString statsText = QString("<b>Statystyki bazy: </b> <br />"
		"Rozegrane mecze: <b>%1</b><br />"
		"Liczba ruchow: Min: <b>%2</b> | Max: <b>%3</b><br />"
		"Srednia: <b>%4</b> | Mediana: <b>%5</b> | Odchylenie standardowe: <b>%6</b>")
		.arg(moves.size()).arg(minMoves).arg(maxMoves).arg(avg, 0, 'f', 2).arg(median, 0, 'f', 2).arg(variance, 0, 'f', 2);

	statsLabel->setText(statsText);
}