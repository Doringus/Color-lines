#pragma once

#include <QList>
#include <QQueue>

#include <optional>

namespace  {

    template<typename T>
    using Table = QList<QList<T>>;

    std::optional<QPair<int, int>> canMoveRight(const Table<int>& table,  const QPair<int, int>& from, int freeCellValue) noexcept {
        if(from.second == table.at(from.first).size() - 1 || table.at(from.first).at(from.second + 1) != 0) {
            return std::nullopt;
        }
        return {{from.first, from.second + 1}};
    }

    std::optional<QPair<int, int>> canMoveTop(const Table<int>& table,  const QPair<int, int>& from, int freeCellValue) noexcept {
        if(from.first == 0 || table.at(from.first - 1).at(from.second) != 0) {
            return std::nullopt;
        }
        return {{from.first - 1, from.second}};
    }

    std::optional<QPair<int, int>> canMoveLeft(const Table<int>& table,  const QPair<int, int>& from, int freeCellValue) noexcept {
        if(from.second == 0 || table.at(from.first).at(from.second - 1) != 0) {
            return std::nullopt;
        }
        return {{from.first, from.second - 1}};
    }

    std::optional<QPair<int, int>> canMoveBottom(const Table<int>& table,  const QPair<int, int>& from, int freeCellValue) noexcept {
        if(from.first == table.size() - 1 || table.at(from.first + 1).at(from.second) != 0) {
            return std::nullopt;
        }
        return {{from.first + 1, from.second}};
    }

    bool canBuildPath(Table<int> table, const QPair<int, int>& from, const QPair<int, int>& to, int freeCellValue = 0) {
        QQueue<QPair<int, int>> cellsQueue;
        cellsQueue.enqueue(from);
        const int visitedSign = -1; // if cell was visited it has -1 value
        table[from.first][from.second] = visitedSign;
        while(!cellsQueue.empty()) {
            auto currentCell = cellsQueue.dequeue();
            if(currentCell == to) {
                return true;
            }
            if(auto neighbour = canMoveTop(table, currentCell, freeCellValue)) {
                cellsQueue.enqueue(*neighbour);
                table[(*neighbour).first][(*neighbour).second] = visitedSign;
            }
            if(auto neighbour = canMoveRight(table, currentCell, freeCellValue)) {
                cellsQueue.enqueue(*neighbour);
                table[(*neighbour).first][(*neighbour).second] = visitedSign;
            }
            if(auto neighbour = canMoveBottom(table, currentCell, freeCellValue)) {
                cellsQueue.enqueue(*neighbour);
                table[(*neighbour).first][(*neighbour).second] = visitedSign;
            }
            if(auto neighbour = canMoveLeft(table, currentCell, freeCellValue)) {
                cellsQueue.enqueue(*neighbour);
                table[(*neighbour).first][(*neighbour).second] = visitedSign;
            }
        }
        return false;
    }

}
