import QtQuick 2.15
import QtQuick.Controls 2.15

TableView {
    id: control

    delegate: Rectangle {
        implicitHeight: control.height / control.rows
        implicitWidth: control.width / control.columns
        Image {
            anchors.fill: parent
            source: model.icon
        }
    }
}
