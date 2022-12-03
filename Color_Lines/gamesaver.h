#pragma once

#include <QString>
#include <QSqlDatabase>
#include <memory>

class GameTable;
struct gameRecord_t;

class GameSaver {
public:
    GameSaver(const QString& dbName);
    ~GameSaver();

    void saveGame(const gameRecord_t& record);
private:
    QSqlDatabase m_Database;
};

