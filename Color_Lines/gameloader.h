#pragma once

#include <QString>
#include <QSqlDatabase>
#include <memory>

class GameTable;

struct gameRecord_t {
    std::unique_ptr<GameTable> table;
    int score;
};

class GameLoader {
public:
    GameLoader(const QString& dbName);
    ~GameLoader();

    gameRecord_t load();

private:
    QSqlDatabase m_Database;
};

