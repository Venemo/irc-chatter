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
            Label {
                text: "Misc events"
                width: parent.width
                height: miscEventsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: miscEventsSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            Label {
                text: "Timestamps"
                width: parent.width
                height: timestampsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: timestampsSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            Label {
                text: "Font size"
                width: parent.width
                height: fontSizeTumblerButton.height
                verticalAlignment: Text.AlignVCenter

                TumblerButton {
                    id: fontSizeTumblerButton
                    anchors.right: parent.right
                    text: "24"
                    width: 140

                    onClicked: fontSizeSelectionDialog.open()
                }
            }
            TitleLabel {
                text: "Send notifications for"
            }
            Label {
                text: "Messages in queries"
                width: parent.width
                height: messagesInQueriesSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: messagesInQueriesSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            Label {
                text: "Messages containing your nick"
                width: parent.width
                height: messagesInQueriesSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: messagesContainingYourNickSwitch
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

    WorkingSelectionDialog {
        id: fontSizeSelectionDialog
        model: ListModel { }
        titleText: "Font size"
        searchFieldVisible: false
        Component.onCompleted: {
            for (var i = 10; i <= 40; i++)
                model.append({ name: i + " pixels", pixelSize: i });
        }
    }
}
