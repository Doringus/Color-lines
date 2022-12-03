import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.12

import GameModel 1.0

Window {
    width: 900
    height: 480
    visible: true
    title: qsTr("Color lines")

    RowLayout {
        anchors.fill: parent
        spacing: 0

        GameTable {
            id: gameTable
            Layout.preferredWidth: 640
            Layout.preferredHeight: 480

            model: GameTableModel {
                id: gameTableModel
                onItemMoved: gameTable.animateFakeItem(toRow, toCol)
                onItemClicked: gameTable.moveFakeItem(row, col, ballImage)
            }

            onItemClicked: {
                model.cellClicked(row, column)
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.margins: 10
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.alignment: Qt.AlignTop
                text: "Счет: " + gameTableModel.score
                font.pointSize: 14
            }
            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                font.pointSize: 14
                text: "Новая игра"
                onClicked: gameTableModel.startNewGame()
            }
        }

    }
}
