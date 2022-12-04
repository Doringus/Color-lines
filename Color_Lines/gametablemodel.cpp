#include "gametablemodel.h"

#include "pathfinder.h"
#include "gamesaver.h"
#include "gameloader.h"

#include <QDebug>

GameTableModel::GameTableModel(QObject *parent) : QAbstractItemModel(parent) {
    GameLoader loader("game.db");
    m_IsGameActive = true;
    auto [table, score] = loader.load();
    m_Table = std::move(table);
    setScore(score);

    connect(m_Table.get(), &GameTable::ballRemoved, [this](int row, int col){
        auto index = createIndex(row, col);
        m_TableItemsStates[row][col] = CellState::STATE_CELL_REMOVED;
        emit dataChanged(index, index, {StateRole});
    });

    m_UserTurn = {0, 0, 0};

    m_RoleNames[IconRole] = "icon";
    m_RoleNames[StateRole] = "state";
    QList<int> column;
    column.reserve(TABLE_SIZE);
    for(size_t row = 0; row < m_Table->getSize(); ++row) {
        for(size_t col = 0; col < m_Table->getSize(); ++col) {
            column.append(CellState::EMPTY_STATE);
        }
        m_TableItemsStates.append(column);
        column.clear();
    }

    m_BallImagePaths << "" << "qrc:/res/black.png" << "qrc:/res/red.png" << "qrc:/res/yellow.png" << "qrc:/res/green.png";

    // Place initial balls
    computerTurn(generateBallsForComputerTurn());
}

GameTableModel::~GameTableModel() {
    GameSaver saver("game.db");
    saver.saveGame({std::move(m_Table), m_Score});
}

int GameTableModel::rowCount(const QModelIndex& parent) const noexcept {
    Q_UNUSED(parent);
    return m_Table->getSize();
}

int GameTableModel::columnCount(const QModelIndex& parent) const noexcept {
    Q_UNUSED(parent)
    return m_Table->getSize();
}

QVariant GameTableModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    switch (role) {
        case IconRole: {
            return m_BallImagePaths.at(m_Table->getBall(index.row(), index.column()));
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
            break;
        }
        case StateRole: {
            m_TableItemsStates[index.row()][index.column()] = value.toInt();
            break;
        }
    }
    emit dataChanged(index, index, {role});
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
    if(m_UserTurn.selectedBall != 0 && m_Table->getBall(row, column) == 0) {
        if(m_Table->moveBall({m_UserTurn.row, m_UserTurn.col}, {row, column}, m_UserTurn.selectedBall)) {
            // animate move
            m_TableItemsStates[row][column] = CellState::STATE_CELL_CREATED;
            m_TableItemsStates[m_UserTurn.row][m_UserTurn.col] = CellState::STATE_CELL_REMOVED;
            emit itemMoved(row, column);
            emit dataChanged(createIndex(m_UserTurn.row, m_UserTurn.col), createIndex(m_UserTurn.row, m_UserTurn.col), {IconRole, StateRole});
            m_UserTurn.row = row;
            m_UserTurn.col = column;
        }
    } else {
        m_UserTurn = {m_Table->getBall(row, column), row, column};
        emit itemClicked(row, column, m_BallImagePaths.at(m_Table->getBall(row, column)));
    }
}

void GameTableModel::startNewGame() noexcept {
    beginResetModel();
    for(size_t row = 0; row < m_TableItemsStates.size(); ++row) {
        std::for_each(m_TableItemsStates[row].begin(), m_TableItemsStates[row].begin(), [](int& item){
            item = CellState::EMPTY_STATE;
        });
    }
    m_Table->resetTable();
    endResetModel();
    setScore(0);
    setIsGameActive(true);
    computerTurn(generateBallsForComputerTurn());
}

void GameTableModel::fakeItemMoved() noexcept {
    emit dataChanged(createIndex(m_UserTurn.row, m_UserTurn.col), createIndex(m_UserTurn.row, m_UserTurn.col), {IconRole, StateRole});

    m_UserTurn.selectedBall = 0;

    setScore(m_Score + m_Table->removeLines(5));

    if(!computerTurn(generateBallsForComputerTurn())) {
        setIsGameActive(false);
    }
    setScore(m_Score + m_Table->removeLines(5));

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
        m_Table->putBall(row, col, ball);
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

    for(size_t row = 0; row < m_Table->getSize(); ++row) {
        for(size_t col = 0; col < m_Table->getSize(); ++col) {
            if(m_Table->getBall(row, col) == emptyImageIndex) {
                freeCellsIndices.append({row, col});
            }
        }
    }
    return freeCellsIndices;
}

void GameTableModel::setScore(int score) noexcept {
    if(m_Score != score) {
        m_Score = score;
        emit scoreChanged();
    }
}

void GameTableModel::setIsGameActive(bool active) noexcept {
    if(m_IsGameActive != active) {
        m_IsGameActive = active;
        emit gameActiveChanged();
    }
}
