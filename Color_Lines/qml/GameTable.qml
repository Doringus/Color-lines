import QtQuick 2.15
import QtQuick.Controls 2.15

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
            onSourceChanged: console.log(source)
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
