#include "gamesaver.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

#include "gametable.h"
#include "gameloader.h"

GameSaver::GameSaver(const QString& dbName) : m_Database(QSqlDatabase::addDatabase("QSQLITE")) {
    m_Database.setDatabaseName(dbName);
    if(!m_Database.open()) {
        qDebug() << m_Database.lastError();
    }
    QSqlQuery query(m_Database);
    query.exec("CREATE TABLE IF NOT EXISTS GameTable("
               "row INTEGER NOT NULL, "
               "col INTEGER NOT NULL, "
               "ball INTEGER NOT NULL"
               ")");
    query.exec("DELETE FROM GameTable");
    query.exec("CREATE TABLE IF NOT EXISTS ScoreTable("
               "score INTEGER NOT NULL"
               ")");
    query.exec("DELETE FROM ScoreTable");
}

GameSaver::~GameSaver() {
    m_Database.close();
    m_Database.removeDatabase("QSQLITE");
}

void GameSaver::saveGame(const gameRecord_t& record) {
    if(!m_Database.isOpen()) {
        return;
    }
    QSqlQuery query(m_Database);
    QVariantList rows, cols, balls;
    query.prepare("INSERT INTO GameTable(row, col, ball) "
                  " VALUES(?, ?, ?)");
    for(size_t row = 0; row < record.table->getSize(); ++row) {
        for(size_t col = 0; col < record.table->getSize(); ++col) {
            rows << row;
            cols << col;
            balls << record.table->getBall(row, col);
        }
    }
    query.addBindValue(rows);
    query.addBindValue(cols);
    query.addBindValue(balls);
    query.execBatch();


    query.prepare("INSERT INTO ScoreTable(score) "
                  " VALUES(:score)");
    query.bindValue(":score", record.score);
    query.exec();
}
