import QtQuick 2.0
import QtQuick.Controls 1.3

Item {
    id: item1
    TextArea {
        id: noteContent
        x: 200
        y: 165
    }

    TextField {
        id: noteTitle
        x: 269
        y: 137
        placeholderText: qsTr("Text Field")
    }

    GroupBox {
        id: noteList
        x: 80
        y: 90
        width: 107
        height: 300
        title: qsTr("Group Box")
    }

    ComboBox {
        id: notebookList
        x: 258
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 62
    }

}
