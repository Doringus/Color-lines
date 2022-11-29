#include "gametablemodel.h"

GameTableModel::GameTableModel(QObject *parent) : QAbstractItemModel(parent), m_RowsCount(9), m_ColumnsCount(9) {
    m_RoleNames[IconRole] = "icon";
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
            return 1;
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
