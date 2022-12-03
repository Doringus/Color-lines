#pragma once

#include <QObject>
#include <QAbstractItemModel>

#include "randomgenerator.h"
#include "gametable.h"

constexpr int ROWS_COUNT = 9;
constexpr int COLUMNS_COUNT = 9;

namespace CellState {
    Q_NAMESPACE

    enum State {
        EMPTY_STATE = 0,
        STATE_CELL_CREATED,
        STATE_CELL_REMOVED
    };

    Q_ENUM_NS(State)
}

class GameTableModel : public QAbstractItemModel {
    Q_OBJECT
public:

    enum Roles {
        IconRole = Qt::UserRole + 1,
        StateRole
    };

public:
    explicit GameTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const noexcept override;
    int columnCount(const QModelIndex& parent) const noexcept override;

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) noexcept override;
    QHash<int, QByteArray> roleNames() const noexcept override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    Q_INVOKABLE void cellClicked(int row, int column);
private:
    bool computerTurn(const QList<int>& balls) noexcept;
    QList<int> generateBallsForComputerTurn() noexcept;
    QList<QPair<int, int>> getFreeCellsIndices() const noexcept;

signals:

private:
    struct userTurn_t {
        int selectedBall;
        int row;
        int col;
    };

private:
    int m_Score;
    GameTable m_Table;
    userTurn_t m_UserTurn;
    QList<QList<int>> m_TableItemsStates;
    QStringList m_BallImagePaths;
    QHash<int, QByteArray> m_RoleNames;
    RandomGenerator m_RandomGenerator;
};

