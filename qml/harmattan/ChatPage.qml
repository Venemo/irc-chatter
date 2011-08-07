import QtQuick 1.1
import com.meego 1.0
import net.venemo.ircchatter 1.0

Page {
    id: mainPage


    property ChannelModel currentChannel: null
    property bool shouldUpdateCurrentMessage: true
    property int currentChannelIndex: -1

    function sendCurrentMessage() {
        if (currentChannel !== null) {
            shouldUpdateCurrentMessage = false;
            currentChannel.sendCurrentMessage();
            shouldUpdateCurrentMessage = true;
        }
    }
    function switchChannel(index) {
        shouldUpdateCurrentMessage = false;
        currentChannel = ircModel.channelList.getItem(index);
        currentChannelIndex = index;
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
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
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
                model: currentChannel !== null ? currentChannel.messages() : null
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
                    model: ircModel.channelList
                    delegate: Label {
                        id: myLabel
                        text: model.name
                        height: channelNameBg.width
                        verticalAlignment: Text.AlignVCenter
                        color: (currentChannel !== null && currentChannel.name == model.name) ? "white" : "lightgrey"
                        font.bold: (currentChannel !== null && currentChannel.name == model.name) ? true : false

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
        text: currentChannel !== null ? currentChannel.currentMessage : ""
        platformStyle: TextFieldStyle {
            paddingLeft: tabButton.width
            paddingRight: sendButton.width
        }
        onTextChanged: {
            if (currentChannel !== null && currentChannel.currentMessage !== messageField.text && shouldUpdateCurrentMessage)
                currentChannel.currentMessage = text;
        }
        Keys.onReturnPressed: sendCurrentMessage()

        ToolIcon {
            id: tabButton
            anchors.verticalCenter: messageField.verticalCenter
            anchors.left: parent.left
            platformIconId: "toolbar-reply"
            onClicked: {
                messageField.forceActiveFocus();
                currentChannel.autoCompleteNick();
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

    JoinSheet {
        id: joinSheet
        visualParent: chatPage
        onAccepted: {
            if (isChannel)
                ircModel.joinChannel(joinText);
            else if (isQuery)
                ircModel.queryUser(joinText);
        }
    }

    WorkingSelectionDialog {
        id: channelSelectorDialog
        titleText: "Switch channel"
        model: ircModel.channelList
        onSelectedIndexChanged: switchChannel(selectedIndex);
        onStatusChanged: {
            if (status == DialogStatus.Opening)
                selectedIndex = currentChannelIndex;
        }
    }

    WorkingSelectionDialog {
        id: userSelectorDialog
        titleText: "User list of " + currentChannel.name
        model: currentChannel.users
    }

}
