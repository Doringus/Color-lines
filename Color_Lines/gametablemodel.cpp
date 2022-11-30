#include "gametablemodel.h"

GameTableModel::GameTableModel(QObject *parent) : QAbstractItemModel(parent), m_RowsCount(ROWS_COUNT), m_ColumnsCount(COLUMNS_COUNT) {
    m_RoleNames[IconRole] = "icon";
    m_BallImagePaths << "qrc:/res/black.png" << "qrc:/res/red.png" << "qrc:/res/yellow.png" << "";

    int emptyImageIndex = m_BallImagePaths.size() - 1;
    QList<int> column;
    column.reserve(m_ColumnsCount);
    for(int row = 0; row < m_RowsCount; ++row) {
        for(int col = 0; col < m_ColumnsCount; ++col) {
            column.append(m_RandomGenerator.generate(0, 3));
        }
        m_Data.append(column);
        column.clear();
    }
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
