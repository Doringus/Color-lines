#include "gametablemodel.h"

GameTableModel::GameTableModel(QObject *parent) : QAbstractItemModel(parent), m_RowsCount(ROWS_COUNT), m_ColumnsCount(COLUMNS_COUNT) {
    m_IsFirstClick = true;
    m_RoleNames[IconRole] = "icon";
    m_BallImagePaths << "qrc:/res/black.png" << "qrc:/res/red.png" << "qrc:/res/yellow.png" << "";

    const int emptyImageIndex = m_BallImagePaths.size() - 1;
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
    placeBalls(generateBallsForComputerTurn());
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
    if(m_IsFirstClick) {
        // пользователь выбирает ячейку
        m_IsFirstClick = !m_IsFirstClick;
    } else {
        // шар перемещается, компьютер делает ход
        m_IsFirstClick = !m_IsFirstClick;
        placeBalls(generateBallsForComputerTurn());
    }

}

void GameTableModel::placeBalls(const QList<int>& balls) noexcept {
    QList<QPair<int, int>> freeCells = getFreeCellsIndices();
    std::for_each(balls.begin(), balls.end(), [this, &freeCells](const int& ball){
        int cellIndex = m_RandomGenerator.generate(0, freeCells.size() - 1);
        const int row = freeCells.at(cellIndex).first;
        const int col = freeCells.at(cellIndex).second;
        m_Data[freeCells.at(cellIndex).first][freeCells.at(cellIndex).second] = ball;
        freeCells.removeAt(cellIndex);

        auto index = createIndex(row, col);
        emit dataChanged(index, index, {IconRole});
    });
}

QList<int> GameTableModel::generateBallsForComputerTurn() noexcept {
    QList<int> balls;
    const int emptyImageIndex = m_BallImagePaths.size() - 1;
    const int ballsToPlace = 3;
    for(int i = 0; i < ballsToPlace; ++i) {
        balls.append(m_RandomGenerator.generate(0, emptyImageIndex - 1));
    }
    return balls;
}

QList<QPair<int, int>> GameTableModel::getFreeCellsIndices() const noexcept {
    QList<QPair<int, int>> freeCellsIndices;
    const int emptyImageIndex = m_BallImagePaths.size() - 1;

    for(int row = 0; row < m_RowsCount; ++row) {
        for(int col = 0; col < m_ColumnsCount; ++col) {
            if(m_Data.at(row).at(col) == emptyImageIndex) {
                freeCellsIndices.append({row, col});
            }
        }
    }
    return freeCellsIndices;
}
