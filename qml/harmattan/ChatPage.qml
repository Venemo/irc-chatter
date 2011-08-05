import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage
    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            platformIconId: "toolbar-add";
        }
        ToolIcon {
            platformIconId: "toolbar-send-chat";
        }
        ToolIcon {
            platformIconId: "toolbar-addressbook";
        }
        ToolIcon {
            platformIconId: "toolbar-settings";
            onClicked: appWindow.pageStack.push(settingsPage)
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu";
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Flickable {
        id: chatFlickable
        anchors.top: parent.top
        anchors.bottom: bottomRow.top
        anchors.left: parent.left
        anchors.right: channelNameBg.left
        anchors.bottomMargin: 10

        interactive: true
        contentHeight: chatArea.height
        clip: true

        TextEdit {
            id: chatArea
            width: parent.width
            readOnly: true
            wrapMode: TextEdit.WordWrap
            textFormat: TextEdit.RichText
            font.pixelSize: 24
            text: "13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: <a href='http://meego.com'>http://meego.com</a><br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed<br />13:34 <span style='color: green'>Venemo</span>: This is an example message\n<br />13:35 <span style='color: red'>DocScrutinizer</span>: Indeed"
        }
    }
    ScrollDecorator {
        flickableItem: chatFlickable
    }
    Rectangle {
        id: channelNameBg
        color: "green"
        width: 60
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: 10

        Flickable {
            id: channelSwitcherFlickable
            anchors.fill: parent
            contentHeight: channelSwitcherRow.width + 30
            interactive: true
            clip: true
            Row {
                id: channelSwitcherRow
                spacing: 10
                anchors.top: parent.top
                anchors.topMargin: 60
                transform: Rotation {
                    angle: 90
                    origin.x: 45
                }
                Repeater {
                    model: channelList
                    delegate: Label {
                        text: model.name
                        color: "lightgray"
                    }
                }
            }
        }
        ScrollDecorator {
            flickableItem: channelSwitcherFlickable
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        id: bottomRow
        width: parent.width
        spacing: 5

        ToolIcon {
            id: tabButton
            anchors.verticalCenter: messageField.verticalCenter
            platformIconId: "toolbar-reply"

        }
        TextField {
            id: messageField
            width: parent.width - tabButton.width - sendButton.width - 10
            placeholderText: "Type a message"
        }
        ToolIcon {
            id: sendButton
            anchors.verticalCenter: messageField.verticalCenter
            platformIconId: "toolbar-send-sms"
        }
    }
}
