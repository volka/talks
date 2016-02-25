import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2

ApplicationWindow {
    id: mainWindow
    visible: true

    GridLayout {
        id: mainLayout
        rows: 2
        columns: 2
        anchors.fill: parent

        RowLayout {
            id: notebookLayout
            width: 100
            height: 100
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.columnSpan: 2

            ComboBox {
                id: notebookList
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            ToolButton {
                id: addNotebook
                text: "Add Notebook"
                iconSource: "/icons/list-add-symbolic.symbolic.png"
            }

            ToolButton {
                id: delNotebook
                text: "Delete Notebook"
                iconSource: "/icons/list-remove-symbolic.symbolic.png"
            }
        }

        Component {
            id: notesListHeader
            Text {
                font.bold: true
                text: qsTr("Notes")
            }
        }

        ListModel {
            id: noteListModel

            ListElement {
                name: qsTr("Einkaufsliste")
            }
            ListElement {
                name: qsTr("Vortrag SQL")
            }
        }

        ListView {
            id: noteList
            model: noteListModel
            header: notesListHeader
            delegate: Component {
                Text { text: name }
            }
            Layout.fillHeight: true
            Layout.fillWidth: true
            width: 100
            footer: Component {
            ColumnLayout {
                    id: noteListLayout
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.fillHeight: true
                    Button {
                        id: addNote
                        text: "Add Note"
                        iconSource: "../icons/list-add-symbolic.symbolic.png"
                        Layout.fillWidth: true
                    }

                    Button {
                        id: delNote
                        text: "Delete Note"
                        iconSource: "../icons/list-remove-symbolic.symbolic.png"
                        Layout.fillWidth: true
                    }
                }
            }
        }

        ColumnLayout {
            id: noteLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            TextField {
                id: noteTitle
                Layout.fillWidth: true
                placeholderText: qsTr("Text Field")
            }

            TextArea {
                id: noteContent
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }





    }

}
