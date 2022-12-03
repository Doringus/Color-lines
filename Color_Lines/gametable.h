#pragma once

#include <QList>
#include <QObject>

class GameTable : public QObject {
    Q_OBJECT
public:
    GameTable(size_t size, QObject* parent = nullptr) noexcept;

    void resetTable() noexcept;

    void putBall(size_t row, size_t col, int ball) noexcept;
    int getBall(int row, int col) const noexcept;
    bool moveBall(const QPair<int, int>& from, const QPair<int, int>& to, int ball) noexcept;
    int removeLines(size_t lineLength) noexcept;

    size_t getSize() const noexcept;

signals:
    // Signal emited when ball was removed by "burning" line
    void ballRemoved(size_t row, size_t col);
    void ballPlaced(size_t row, size_t col);
private:
    size_t m_Size;
    QList<QList<int>> m_Data;
};

