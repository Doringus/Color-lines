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
            anchors.fill: parent
            source: model.icon
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                control.itemClicked(row, column)
            }
        }
    }
}
