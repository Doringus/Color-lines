import QtQuick 2.15
import QtQuick.Window 2.15

import GameModel 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Color lines")

    GameTable {
        id: gameTable
        width: 640
        height: 480
        model: GameTableModel {
            rowsCount: 9
            columnsCount: 9
        }
    }
}
