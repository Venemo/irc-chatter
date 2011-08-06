import QtQuick 1.1
import com.meego 1.0
import net.venemo.ircchatter 1.0

Page {
    id: mainPage
    property ChannelModel currentChannel: null
    property bool shouldUpdateCurrentMessage: true
    Keys.forwardTo: [(returnKey), (mainPage)]
    Keys.onReturnPressed: sendCurrentMessage()

    function sendCurrentMessage() {
        if (currentChannel !== null) {
            shouldUpdateCurrentMessage = false;
            currentChannel.sendCurrentMessage();
            shouldUpdateCurrentMessage = true;
        }
    }

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
        contentHeight: chatColumn.height
        clip: true

        Column {
            id: chatColumn
            Repeater {
                id: chatView
                width: parent.width
                model: currentChannel !== null ? currentChannel.messages() : null
                delegate: Text {
                    wrapMode: TextEdit.WordWrap
                    textFormat: TextEdit.RichText
                    font.pixelSize: 24
                    text: model.timestamp + " <span style='color: green'>" + "mehmeh" + "</span>: " + model.text;
                }
                onCountChanged: {
                    var should = Math.max(0,  chatColumn.height - chatFlickable.height);
                    if (chatFlickable.contentY >= should - 50)
                        chatFlickable.contentY = should;
                }
                onModelChanged: chatFlickable.contentY = Math.max(0,  chatColumn.height - chatFlickable.height)
            }
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
                anchors.topMargin: 70
                transform: Rotation {
                    angle: 90
                    origin.x: channelNameBg.width
                }
                Repeater {
                    model: channelList
                    delegate: Label {
                        id: myLabel
                        text: model.name
                        height: channelNameBg.width
                        verticalAlignment: Text.AlignVCenter
                        color: (currentChannel !== null && currentChannel.name == model.name) ? "white" : "lightgrey"
                        font.bold: (currentChannel !== null && currentChannel.name == model.name) ? true : false

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                shouldUpdateCurrentMessage = false;
                                currentChannel = channelList.getItem(index)
                                shouldUpdateCurrentMessage = true;
                            }
                        }
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
            onClicked: {
                messageField.forceActiveFocus();
                // TODO
            }
        }
        TextField {
            id: messageField
            width: parent.width - tabButton.width - sendButton.width - 10
            placeholderText: "Type a message"
            text: currentChannel !== null ? currentChannel.currentMessage : ""
            Binding {
                target: currentChannel
                property: "currentMessage"
                value: messageField.text
                when: currentChannel !== null && shouldUpdateCurrentMessage
            }
        }
        ToolIcon {
            id: sendButton
            anchors.verticalCenter: messageField.verticalCenter
            platformIconId: "toolbar-send-sms"
            onClicked: {
                messageField.forceActiveFocus();
                sendCurrentMessage()
            }
        }
    }
}
