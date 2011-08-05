import QtQuick 1.1
import com.meego 1.0
import com.meego.extras 1.0

Page {
    id: settingsPage
    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop()
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Flickable {
        id: settingsFlickable
        anchors.fill: parent

        interactive: true
        contentWidth: parent.width
        contentHeight: settingsColumn.height + 30
        clip: true

        Column {
            id: settingsColumn
            spacing: 10
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            TitleLabel {
                text: "Display"
            }
            Row {
                spacing: 10
                width: parent.width
                Label {
                    text: "Misc events"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    anchors.right: parent.right
                    checked: true
                }
            }
            Row {
                spacing: 10
                width: parent.width
                Label {
                    text: "Timestamps"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    anchors.right: parent.right
                    checked: true
                }
            }
            Row {
                spacing: 10
                width: parent.width
                Label {
                    text: "Font size"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TumblerButton {
                    anchors.right: parent.right
                    text: "24"
                    width: 140
                }
            }
            TitleLabel {
                text: "Send notifications for"
            }
            Row {
                spacing: 10
                width: parent.width
                Label {
                    text: "Messages in queries"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    anchors.right: parent.right
                    checked: true
                }
            }
            Row {
                spacing: 10
                width: parent.width
                Label {
                    text: "Messages containing your nick"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    anchors.right: parent.right
                    checked: true
                }
            }
            TitleLabel {
                text: "Configuration"
            }
            Button {
                width: parent.width - 100
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Configure servers"
            }
        }
    }
    ScrollDecorator {
        flickableItem: settingsFlickable
    }
}
