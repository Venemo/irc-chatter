// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

import QtQuick 1.1
import com.meego 1.0
import net.venemo.ircchatter 1.0

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
    function scrollToBottom() {
        chatFlickable.contentY = Math.max(0,  chatArea.height - chatFlickable.height)
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
            visible: ircModel.currentChannel === null ? false : (ircModel.currentChannel.name.charAt(0) === '#')
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

        interactive: true
        contentHeight: chatArea.height
        clip: true
        onHeightChanged: scrollToBottom()

        property int lastSupposedContentY: 0

        TextArea {
            id: chatArea
            width: parent.width
            height: Math.max(chatFlickable.height, implicitHeight)
            readOnly: true
            wrapMode: TextEdit.WordWrap
            textFormat: TextEdit.RichText
            text: ircModel.currentChannel !== null ? ircModel.currentChannel.channelText : ""
            font:

            onTextChanged: {
                var should = Math.max(0,  chatArea.height - chatFlickable.height);
                if (chatFlickable.contentY >= chatFlickable.lastSupposedContentY - 72)
                    chatFlickable.lastSupposedContentY = chatFlickable.contentY = should;
            }

            Connections {
                target: chatArea.children[3]
                onLinkActivated: {
                    Qt.openUrlExternally(link);
                }
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
                        color: isCurrent ? "white" : (hasNewMessageWithUserNick ? "red" : (hasNewMessage ? "blue" : "lightgrey"))
                        font.bold: isCurrent ? true : false

                        property bool hasNewMessage: false
                        property bool hasNewMessageWithUserNick: false
                        property bool isCurrent: ircModel.currentChannelIndex == index

                        Connections {
                            target: ircModel.allChannels.getItem(index)
                            onNewMessageReceived: {
                                if (!isCurrent)
                                    hasNewMessage = true;
                            }
                            onNewMessageWithUserNickReceived: {
                                if (!isCurrent)
                                    hasNewMessageWithUserNick = true;
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                hasNewMessage = false;
                                hasNewMessageWithUserNick = false;
                                switchChannel(index);
                                scrollToBottom();
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
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
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
                onClicked: quitDialog.open()
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
        titleText: "User list of " + (ircModel.currentChannel === null ? "?" : ircModel.currentChannel.name)
        model: ircModel.currentChannel === null ? null : ircModel.currentChannel.users
        searchFieldVisible: true
    }

}
