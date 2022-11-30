#include "gametablemodel.h"

GameTableModel::GameTableModel(QObject *parent) : QAbstractItemModel(parent), m_RowsCount(ROWS_COUNT), m_ColumnsCount(COLUMNS_COUNT) {
    m_SelectedBall = 0;
    m_RoleNames[IconRole] = "icon";
    m_BallImagePaths << "" << "qrc:/res/black.png" << "qrc:/res/red.png" << "qrc:/res/yellow.png";

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
    if(m_SelectedBall != 0 && m_Data.at(row).at(column) == 0) {
        placeBall(row, column, m_SelectedBall);
        // Check if we can move ball
        // Move ball
        // Check lines
        // Computer make turn
        computerTurn(generateBallsForComputerTurn());
        m_SelectedBall = 0;
    } else {
        m_SelectedBall = m_Data.at(row).at(column);
    }
}

void GameTableModel::placeBall(int row, int col, int ball) noexcept {
    m_Data[row][col] = ball;
    auto index = createIndex(row, col);
    emit dataChanged(index, index, {IconRole});
}

void GameTableModel::computerTurn(const QList<int>& balls) noexcept {
    QList<QPair<int, int>> freeCells = getFreeCellsIndices();
    std::for_each(balls.begin(), balls.end(), [this, &freeCells](const int& ball){
        int cellIndex = m_RandomGenerator.generate(0, freeCells.size() - 1);
        const int row = freeCells.at(cellIndex).first;
        const int col = freeCells.at(cellIndex).second;
        placeBall(row, col, ball);
        freeCells.removeAt(cellIndex);
    });
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
