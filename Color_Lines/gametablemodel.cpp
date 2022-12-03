#include "gametablemodel.h"

#include "pathfinder.h"

#include <QDebug>

GameTableModel::GameTableModel(QObject *parent)
    : QAbstractItemModel(parent), m_Table(9) {

    connect(&m_Table, &GameTable::ballRemoved, [this](int row, int col){
        auto index = createIndex(row, col);
        m_TableItemsStates[row][col] = CellState::STATE_CELL_REMOVED;
        emit dataChanged(index, index, {StateRole});
    });

    m_UserTurn = {0, 0, 0};
    m_Score = 0;

    m_RoleNames[IconRole] = "icon";
    m_RoleNames[StateRole] = "state";
    QList<int> column;
    column.reserve(9);
    for(size_t row = 0; row < m_Table.getSize(); ++row) {
        for(size_t col = 0; col < m_Table.getSize(); ++col) {
            column.append(CellState::EMPTY_STATE);
        }
        m_TableItemsStates.append(column);
        column.clear();
    }

    m_BallImagePaths << "" << "qrc:/res/black.png" << "qrc:/res/red.png" << "qrc:/res/yellow.png" << "qrc:/res/green.png";

    // Place initial balls
    computerTurn(generateBallsForComputerTurn());
}

int GameTableModel::rowCount(const QModelIndex& parent) const noexcept {
    Q_UNUSED(parent);
    return m_Table.getSize();
}

int GameTableModel::columnCount(const QModelIndex& parent) const noexcept {
    Q_UNUSED(parent)
    return m_Table.getSize();
}

QVariant GameTableModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    switch (role) {
        case IconRole: {
            return m_BallImagePaths.at(m_Table.getBall(index.row(), index.column()));
        }
        case StateRole: {
            return m_TableItemsStates.at(index.row()).at(index.column());
        }
    }
    return 0;
}

bool GameTableModel::setData(const QModelIndex& index, const QVariant& value, int role) noexcept {
    if (!index.isValid()) {
            return false;
    }
    switch (role) {
        case IconRole: {
            return false;
        }
        case StateRole: {
            m_TableItemsStates[index.row()][index.column()] = value.toInt();
            break;
        }
    }
    emit dataChanged(index, index, {StateRole});
    return true;
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
    if(m_UserTurn.selectedBall != 0 && m_Table.getBall(row, column) == 0) {
        if(m_Table.moveBall({m_UserTurn.row, m_UserTurn.col}, {row, column}, m_UserTurn.selectedBall)) {
            // animate move
            m_TableItemsStates[row][column] = CellState::STATE_CELL_CREATED;
            m_TableItemsStates[m_UserTurn.row][m_UserTurn.col] = CellState::STATE_CELL_REMOVED;
            emit dataChanged(createIndex(row, column),createIndex(row, column), {IconRole, StateRole});
            emit dataChanged(createIndex(m_UserTurn.row, m_UserTurn.col), createIndex(m_UserTurn.row, m_UserTurn.col), {IconRole, StateRole});
            m_UserTurn.selectedBall = 0;

            m_Table.removeLines(5);

            if(!computerTurn(generateBallsForComputerTurn())) {
                // game over
            }
            m_Table.removeLines(5);
        }
    } else {
        m_UserTurn = {m_Table.getBall(row, column), row, column};
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
        m_Table.putBall(row, col, ball);
        m_TableItemsStates[row][col] = CellState::STATE_CELL_CREATED;
        auto index = createIndex(row, col);
        emit dataChanged(index, index, {IconRole, StateRole});
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

    for(size_t row = 0; row < m_Table.getSize(); ++row) {
        for(size_t col = 0; col < m_Table.getSize(); ++col) {
            if(m_Table.getBall(row, col) == emptyImageIndex) {
                freeCellsIndices.append({row, col});
            }
        }
    }
    return freeCellsIndices;
}
