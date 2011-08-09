import QtQuick 1.1
import com.meego 1.0

Page {
    id: mainPage

    property bool shouldUpdateCurrentMessage: true

    function sendCurrentMessage() {
        if (ircModel.currentChannel !== null) {
            shouldUpdateCurrentMessage = false;
            ircModel.currentChannel.sendCurrentMessage();
            shouldUpdateCurrentMessage = true;
        }
    }
    function switchChannel(index) {
        shouldUpdateCurrentMessage = false;
        ircModel.currentChannelIndex = index;
        shouldUpdateCurrentMessage = true;
    }

    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            platformIconId: "toolbar-add";
            onClicked: joinSheet.open()
        }
        ToolIcon {
            platformIconId: "toolbar-send-chat";
            onClicked: channelSelectorDialog.open();
        }
        ToolIcon {
            platformIconId: "toolbar-addressbook";
            onClicked: userSelectorDialog.open();
        }
        ToolIcon {
            platformIconId: "toolbar-settings";
            onClicked: appWindow.pageStack.push(settingsPage)
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu";
            onClicked: (chatMenu.status == DialogStatus.Closed) ? chatMenu.open() : chatMenu.close()
        }
    }

    Flickable {
        id: chatFlickable
        anchors.top: parent.top
        anchors.bottom: messageField.top
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
                model: ircModel.currentChannel !== null ? ircModel.currentChannel.messages : null
                delegate: Text {
                    wrapMode: TextEdit.WordWrap
                    textFormat: TextEdit.RichText
                    font.pixelSize: 24 // TODO: bind this to setting
                    text: model.timestamp + " <span style='color: " + model.userNameColor + "'>" + model.userName + "</span>: " + model.text;
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
        anchors.bottom: messageField.top

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
                    model: ircModel.allChannels
                    delegate: Label {
                        id: myLabel
                        text: model.name
                        height: channelNameBg.width
                        verticalAlignment: Text.AlignVCenter
                        color: (ircModel.currentChannel !== null && ircModel.currentChannelIndex == index) ? "white" : "lightgrey"
                        font.bold: (ircModel.currentChannel !== null && ircModel.currentChannelIndex == index) ? true : false

                        MouseArea {
                            anchors.fill: parent
                            onClicked: switchChannel(index)
                        }
                    }
                }
            }
        }
        ScrollDecorator {
            flickableItem: channelSwitcherFlickable
        }
    }

    TextField {
        id: messageField
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        width: parent.width
        placeholderText: "Type a message"
        text: ircModel.currentChannel !== null ? ircModel.currentChannel.currentMessage : ""
        platformStyle: TextFieldStyle {
            paddingLeft: tabButton.width
            paddingRight: sendButton.width
        }
        onTextChanged: {
            if (ircModel.currentChannel !== null && ircModel.currentChannel.currentMessage !== messageField.text && shouldUpdateCurrentMessage)
                ircModel.currentChannel.currentMessage = text;
        }
        Keys.onReturnPressed: sendCurrentMessage()

        ToolIcon {
            id: tabButton
            anchors.verticalCenter: messageField.verticalCenter
            anchors.left: parent.left
            platformIconId: "toolbar-reply"
            onClicked: {
                messageField.forceActiveFocus();
                ircModel.currentChannel.autoCompleteNick();
            }
        }
        ToolIcon {
            id: sendButton
            anchors.verticalCenter: messageField.verticalCenter
            anchors.right: parent.right
            platformIconId: "toolbar-send-sms"
            onClicked: {
                messageField.forceActiveFocus();
                sendCurrentMessage()
            }
        }
    }

    Menu {
        id: chatMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Close / Part"
                onClicked: {
                    if (ircModel.currentChannel.name.charAt(0) === '#')
                        ircModel.currentServer.partChannel(ircModel.currentChannel.name)
                    else
                        ircModel.currentServer.closeUser(ircModel.currentChannel.name)
                }
            }
            MenuItem {
                text: "Quit app"
                onClicked: Qt.quit();
            }
        }
    }

    JoinSheet {
        id: joinSheet
        visualParent: chatPage
        onAccepted: {
            if (isChannel)
                ircModel.currentServer.joinChannel(joinText);
            else if (isQuery)
                ircModel.currentServer.queryUser(joinText);
        }
    }

    WorkingSelectionDialog {
        id: channelSelectorDialog
        titleText: "Switch channel"
        model: ircModel.allChannels
        onSelectedIndexChanged: switchChannel(selectedIndex);
        onStatusChanged: {
            if (status == DialogStatus.Opening)
                selectedIndex = ircModel.currentChannelIndex;
        }
        searchFieldVisible: true
    }

    WorkingSelectionDialog {
        id: userSelectorDialog
        titleText: "User list of " + ircModel.currentChannel.name
        model: ircModel.currentChannel.users
    }

}
