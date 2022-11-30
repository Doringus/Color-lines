#pragma once

#include <QObject>
#include <QAbstractItemModel>

constexpr int ROWS_COUNT = 9;
constexpr int COLUMNS_COUNT = 9;

class GameTableModel : public QAbstractItemModel {
    Q_OBJECT
public:

    enum Roles {
        IconRole = Qt::UserRole + 1
    };

public:
    explicit GameTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const noexcept override;
    int columnCount(const QModelIndex& parent) const noexcept override;

    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const noexcept override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

signals:

private:
    int m_RowsCount, m_ColumnsCount;
    QList<QList<int>> m_Data;
    QStringList m_BallImagePaths;
    QHash<int, QByteArray> m_RoleNames;
};

