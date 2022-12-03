import QtQuick 2.15
import QtQuick.Controls 2.15

import GameModel 1.0

TableView {
    id: control

    interactive: false

    property int cellWidth: control.width / control.columns
    property int cellHeight: control.height / control.rows

    signal itemClicked(int row, int column)

    /// Starts animation to simulate moving balls
    function animateFakeItem(toRow, toCol) {
        fakeMovingItem.visible = true
        movingAnimationX.to = toCol * cellWidth
        movingAnimationY.to = toRow * cellHeight
        movingAnimation.start()
    }

    /// Simply moves fake image to last clicked ball
    /// and changes fake item image to image of last clicked ball
    function moveFakeItem(toRow, toCol, ball) {
        fakeMovingItem.x = toCol * control.width / control.columns
        fakeMovingItem.y = toRow * control.height / control.rows
        fakeMovingItem.source = ball
    }

    delegate: Rectangle {
        implicitHeight: cellHeight
        implicitWidth: cellWidth
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
            NumberAnimation on opacity {
                running: model.state === CellState.STATE_CELL_CREATED
                from: 0.0
                to: 1.0
                duration: 1300
            }
            NumberAnimation on opacity {
                running: model.state === CellState.STATE_CELL_REMOVED
                from: 1.0
                to: 0.0
                duration: 1300
                onFinished: {
                    model.state = 0
                    model.icon = ""
                }
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

    Image {
        id: fakeMovingItem
        z: 3
        source: ""
        visible: false
        height:cellHeight
        width: cellWidth

        ParallelAnimation {
            id: movingAnimation

            /// After fake animation was finished
            /// we should hide it and show the real one
            onFinished: {
                fakeMovingItem.visible = false
                control.model.fakeItemMoved()
            }

            NumberAnimation {
                id: movingAnimationX
                target: fakeMovingItem
                property: "x"
                to: 400
                duration: 1000
            }
            NumberAnimation {
                id: movingAnimationY
                target: fakeMovingItem
                property: "y"
                to: 400
                duration: 1000
            }
        }
    }
}
