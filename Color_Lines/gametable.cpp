#include "gametable.h"

#include "pathfinder.h"

GameTable::GameTable(size_t size, QObject* parent) noexcept : QObject(parent), m_Size(size) {
    QList<int> column;
    column.reserve(m_Size);
    for(size_t row = 0; row < m_Size; ++row) {
        for(size_t col = 0; col < m_Size; ++col) {
            column.append(0);
        }
        m_Data.append(column);
        column.clear();
    }
}

void GameTable::putBall(size_t row, size_t col, int ball) noexcept {
    if(row >= m_Data.size() || col >= m_Data.size()) {
        return;
    }
    m_Data[row][col] = ball;
}

int GameTable::getBall(int row, int col) const noexcept {
    return m_Data.at(row).at(col);
}

bool GameTable::moveBall(const QPair<int, int>& from, const QPair<int, int>& to, int ball) noexcept {
    if(!canBuildPath(m_Data, from, to)) {
        return false;
    }
    m_Data[from.first][from.second] = 0;
    m_Data[to.first][to.second] = ball;
  //  emit ballRemoved(from.first, from.second);
  //  emit ballPlaced(to.first, to.second);

    return true;
}

int GameTable::removeLines(size_t lineLength) noexcept {
    QList<QList<int>> templateField = m_Data;
    int score = 0;
    // Checking for rows

    auto removeRow = [this](int row, int startIndex, int size) {
        for(int col = startIndex; col < startIndex + size; ++col) {
            m_Data[row][col] = 0;
            emit ballRemoved(row, col);
        }
    };

    for(size_t row = 0; row < m_Size; ++row) {
        int ball = templateField.at(row).at(0);
        size_t lineSize = 1;
        int lineStartIndex = 0;
        for(size_t col = 1; col < m_Size; ++col) {
            if(templateField.at(row).at(col) != ball) {
                if(lineSize >= lineLength && ball != 0) {
                    removeRow(row, lineStartIndex, lineSize);
                    score += lineSize;
                }
                lineSize = 1;
                lineStartIndex = col;
                ball = templateField.at(row).at(col);
            } else {
                lineSize++;
            }
        }

        if(lineSize >= lineLength && ball != 0) {
            removeRow(row, lineStartIndex, lineSize);
            score += lineSize;
        }
    }

    // Checking for columns

    auto removeColumn = [this](int col, int startIndex, int size) {
        for(int row = startIndex; row < startIndex + size; ++row) {
            m_Data[row][col] = 0;
            emit ballRemoved(row, col);
        }
    };

    for(size_t col = 0; col < m_Size; ++col) {
        int ball = templateField.at(0).at(col);
        size_t lineSize = 1;
        int lineStartIndex = 0;
        for(size_t row = 1; row < m_Size; ++row) {
            if(templateField.at(row).at(col) == 0) {
                continue;
            }
            if(templateField.at(row).at(col) != ball) {
                if(lineSize >= lineLength) {
                    removeColumn(col, lineStartIndex, lineSize);
                    score += lineSize;
                }
                lineSize = 1;
                lineStartIndex = row;
                ball = templateField.at(row).at(col);
            } else {
                lineSize++;
            }
        }

        if(lineSize >= lineLength) {
            removeColumn(col, lineStartIndex, lineSize);
            score += lineSize;
        }
    }
    return score;
}

size_t GameTable::getSize() const noexcept {
    return m_Size;
}
