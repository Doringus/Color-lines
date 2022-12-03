#include "gameloader.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "gametable.h"

GameLoader::GameLoader(const QString& dbName) : m_Database(QSqlDatabase::addDatabase("QSQLITE")) {
    m_Database.setDatabaseName(dbName);
    m_Database.open();
}

GameLoader::~GameLoader() {
    m_Database.close();
    m_Database.removeDatabase("QSQLITE");
}

gameRecord_t GameLoader::load() {
    auto table = std::make_unique<GameTable>(9);
    if(!m_Database.isOpen()) {
        return {std::move(table), 0};
    }

    QSqlQuery query(m_Database);
    if(!query.exec("SELECT * FROM GameTable")) {
        return {std::move(table), 0};
    }
    QSqlRecord record = query.record();

    const int rowIndex = record.indexOf("row");
    const int colIndex = record.indexOf("col");
    const int ballIndex = record.indexOf("ball");

    while(query.next()) {
        table->putBall(query.value(rowIndex).toULongLong(),
                       query.value(colIndex).toULongLong(),
                       query.value(ballIndex).toInt());
    }

    int score = 0;
    if(!query.exec("SELECT * FROM ScoreTable")) {
        return {std::move(table), 0};
    }
    record = query.record();
    const int scoreIndex = record.indexOf("score");
    query.next();
    score = query.value(scoreIndex).toInt();

    return {std::move(table), score};
}
