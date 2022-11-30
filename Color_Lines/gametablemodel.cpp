#include "gametablemodel.h"

#include "pathfinder.h"

#include <QDebug>

GameTableModel::GameTableModel(QObject *parent) : QAbstractItemModel(parent), m_RowsCount(ROWS_COUNT), m_ColumnsCount(COLUMNS_COUNT) {
    m_UserTurn = {0, 0, 0};
    m_Score = 0;
    m_RoleNames[IconRole] = "icon";
    m_BallImagePaths << "" << "qrc:/res/black.png" << "qrc:/res/red.png" << "qrc:/res/yellow.png" << "qrc:/res/green.png";

    const int emptyImageIndex = 0;
    QList<int> column;
    column.reserve(m_ColumnsCount);
    for(int row = 0; row < m_RowsCount; ++row) {
        for(int col = 0; col < m_ColumnsCount; ++col) {
            column.append(emptyImageIndex);
        }
        m_Data.append(column);
        column.clear();
    }
    // Place initial balls
    computerTurn(generateBallsForComputerTurn());
}

int GameTableModel::rowCount(const QModelIndex& parent) const noexcept {
    Q_UNUSED(parent);
    return m_RowsCount;
}

int GameTableModel::columnCount(const QModelIndex& parent) const noexcept {
    Q_UNUSED(parent)
    return m_ColumnsCount;
}

QVariant GameTableModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    switch (role) {
        case IconRole: {
            return m_BallImagePaths.at(m_Data.at(index.row()).at(index.column()));
        }
    }
    return 0;
}

QHash<int, QByteArray> GameTableModel::roleNames() const noexcept {
    return m_RoleNames;
}

QModelIndex GameTableModel::index(int row, int column, const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column, nullptr);
}

QModelIndex GameTableModel::parent(const QModelIndex& index) const {
    Q_UNUSED(index);
    return QModelIndex();
}

void GameTableModel::cellClicked(int row, int column) {
    if(m_UserTurn.selectedBall != 0 && m_Data.at(row).at(column) == 0) {
        // Check if we can move ball
        if(canBuildPath(m_Data, {m_UserTurn.row, m_UserTurn.col}, {row, column})) {
            // Move ball
            placeBall(row, column, m_UserTurn.selectedBall);
            placeBall(m_UserTurn.row, m_UserTurn.col, 0);
            m_UserTurn.selectedBall = 0;
            // Check lines
            removeLines();
            // Computer make turn
            if(!computerTurn(generateBallsForComputerTurn())) {
                // game over
            }
        }
    } else {
        m_UserTurn = {m_Data.at(row).at(column), row, column};
    }
}

void GameTableModel::placeBall(int row, int col, int ball) noexcept {
    m_Data[row][col] = ball;
    auto index = createIndex(row, col);
    emit dataChanged(index, index, {IconRole});
}

void GameTableModel::removeLines() noexcept {
    const int minLineLength = 5;
    QList<QList<int>> templateField = m_Data;

    auto removeRow = [this](int row, int startIndex, int size) {
        for(int i = startIndex; i < startIndex + size; ++i) {
            placeBall(row, i, 0);
        }
    };
    // Checking for rows
    for(int row = 0; row < ROWS_COUNT; ++row) {
        int ball = templateField.at(row).at(0);
        int lineSize = 1;
        int lineStartIndex = 0;
        for(int col = 1; col < COLUMNS_COUNT; ++col) {
            if(templateField.at(row).at(col) != ball) {
                if(lineSize >= minLineLength && ball != 0) {
                    removeRow(row, lineStartIndex, lineSize);
                    m_Score += lineSize;
                }
                lineSize = 1;
                lineStartIndex = col;
                ball = templateField.at(row).at(col);
            } else {
                lineSize++;
            }
        }

        if(lineSize >= minLineLength && ball != 0) {
            removeRow(row, lineStartIndex, lineSize);
            m_Score += lineSize;
        }
    }

    auto removeColumn = [this](int col, int startIndex, int size) {
        for(int i = startIndex; i < startIndex + size; ++i) {
            placeBall(i, col, 0);
        }
    };

    // Checking for columns
    for(int col = 0; col < COLUMNS_COUNT; ++col) {
        int ball = templateField.at(0).at(col);
        int lineSize = 1;
        int lineStartIndex = 0;
        for(int row = 1; row < ROWS_COUNT; ++row) {
            if(templateField.at(row).at(col) == 0) {
                continue;
            }
            if(templateField.at(row).at(col) != ball) {
                if(lineSize >= minLineLength) {
                    removeColumn(col, lineStartIndex, lineSize);
                    m_Score += lineSize;
                }
                lineSize = 1;
                lineStartIndex = row;
                ball = templateField.at(row).at(col);
            } else {
                lineSize++;
            }
        }

        if(lineSize >= minLineLength) {
            removeColumn(col, lineStartIndex, lineSize);
            m_Score += lineSize;
        }
    }
}

bool GameTableModel::computerTurn(const QList<int>& balls) noexcept {
    QList<QPair<int, int>> freeCells = getFreeCellsIndices();
    if(freeCells.size() < 3) {
        return false;
    }
    std::for_each(balls.begin(), balls.end(), [this, &freeCells](const int& ball){
        int cellIndex = m_RandomGenerator.generate(0, freeCells.size() - 1);
        const int row = freeCells.at(cellIndex).first;
        const int col = freeCells.at(cellIndex).second;
        placeBall(row, col, ball);
        freeCells.removeAt(cellIndex);
    });
    return true;
}

QList<int> GameTableModel::generateBallsForComputerTurn() noexcept {
    QList<int> balls;
    const int ballsToPlace = 3;
    for(int i = 0; i < ballsToPlace; ++i) {
        balls.append(m_RandomGenerator.generate(1, m_BallImagePaths.size() - 1));
    }
    return balls;
}

QList<QPair<int, int>> GameTableModel::getFreeCellsIndices() const noexcept {
    QList<QPair<int, int>> freeCellsIndices;
    const int emptyImageIndex = 0;

    for(int row = 0; row < m_RowsCount; ++row) {
        for(int col = 0; col < m_ColumnsCount; ++col) {
            if(m_Data.at(row).at(col) == emptyImageIndex) {
                freeCellsIndices.append({row, col});
            }
        }
    }
    return freeCellsIndices;
}
