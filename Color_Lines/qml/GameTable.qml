import QtQuick 2.15
import QtQuick.Controls 2.15

import GameModel 1.0

TableView {
    id: control

    signal itemClicked(int row, int column)

    interactive: false

    delegate: Rectangle {
        implicitHeight: control.height / control.rows
        implicitWidth: control.width / control.columns
        color: "#999999"

        Rectangle {
            anchors.fill: parent
            anchors.margins: 2
            color: "#CECECE"
        }

        Image {
            id: icon
            anchors.fill: parent
            source: model.icon
//            opacity: 0

            NumberAnimation on opacity {
                running: model.state === CellState.STATE_CELL_CREATED
                from: 0.0
                to: 1.0
                duration: 1000
            }
            NumberAnimation on opacity {
                running: model.state === CellState.STATE_CELL_REMOVED
                from: 1.0
                to: 0.0
                duration: 1000
                onFinished: model.state = 0
            }
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                control.itemClicked(row, column)
            }
            cursorShape: model.icon === "" ? "ArrowCursor" : "PointingHandCursor"
        }
    }
}
