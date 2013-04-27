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
// Copyright (C) 2012, Timur Kristóf <venemo@fedoraproject.org>

import QtQuick 2.0
import QtWebKit 3.0
import "../components"
import "../misc"

Page {
    id: chatPage

    function sendCurrentMessage() {
        if (ircModel.currentChannel !== null) {
            messageField.shouldUpdateCurrentMessage = false;
            ircModel.currentChannel.sendCurrentMessage();
            messageField.shouldUpdateCurrentMessage = true;
        }
    }
    function switchChannel(index) {
        messageField.shouldUpdateCurrentMessage = false;
        ircModel.currentChannelIndex = index;
        messageField.shouldUpdateCurrentMessage = true;

        messageField.forceActiveFocus();

        scrollToBottom();
    }
    function scrollToBottom() {
        chatFlickable.contentY = Math.max(0,  chatArea.height - chatFlickable.height)
    }

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ddd" }
        GradientStop { position: 0.4; color: "#fff" }
    }

    // The chat message area
    Flickable {
        id: chatFlickable
        boundsBehavior: Flickable.StopAtBounds
        anchors {
            top: parent.top
            left: parent.left
            right: sidebar.left
            bottom: messageField.top
            margins: 15
        }
        contentHeight: chatArea.height

        // Removed because of https://bugreports.qt-project.org/browse/QTBUG-28930
        //        MouseArea {
        //            anchors.fill: chatArea
        //            cursorShape: Qt.IBeamCursor
        //        }

        TextEdit {
            id: chatArea
            width: chatFlickable.width
            selectByMouse: true
            textFormat: TextEdit.RichText
            readOnly: true
            wrapMode: TextEdit.Wrap
            text: ircModel.currentChannel !== null ? ircModel.currentChannel.channelText : ""
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            font.pixelSize: appSettings.fontSize
            font.family: appSettings.fontMonospace ? "Monospace" : appSettings.getDefaultFont()
            onTextChanged: {
                scrollToBottom();
            }
            onLinkActivated: {
                if (link.indexOf('http:') >= 0 || link.indexOf('https:') || link.indexOf('ftp:')) {
                    Qt.openUrlExternally(link);
                }
            }
        }
    }

    // Side bar of the chat UI - channel switcher and options
    Sidebar {
        id: sidebar
        width: 250
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        Column {
            anchors {
                margins: 15
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: moreOptionsContainer.top
            }
            spacing: 0

            Repeater {
                model: ircModel.allChannels
                delegate: ChannelSwitcherButton {
                    isCurrent: ircModel.currentChannelIndex === index
                    channel: model.object

                    onClicked: {
                        switchChannel(index)
                    }

                    Connections {
                        target: ircModel.allChannels.getItem(index)
                        onNewMessageReceived: {
                            if (!isCurrent)
                                hasNewMessage = true
                        }
                        onNewMessageWithUserNickReceived: {
                            if (!isCurrent)
                                hasNewMessageWithUserNick = true
                        }
                    }
                }
            }
        }

        Rectangle {
            id: moreOptionsContainer
            color: "#55444444"
            height: messageField.height
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            Text {
                id: moreOptionsText
                text: "More options..."
                color: "#fff"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: moreOptionsContainer
                onClicked: {
                    moreOptionsMenu.toggle();
                }
                hoverEnabled: true
                onEntered: moreOptionsText.font.underline = true
                onExited: moreOptionsText.font.underline = false;
            }

            Menu {
                id: moreOptionsMenu
                showBottomTab: true
                visible: false
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    bottomMargin: 50
                    margins: 5
                }

                MenuButton {
                    text: "Disconnect all"
                    onClicked: {
                        areYouSureToDisconnectAllDialog.open();
                    }
                }
                MenuButton {
                    text: "Manage servers"
                    // TODO: onClicked
                }
                MenuButton {
                    text: "Settings"
                    // TODO: onClicked
                }
                MenuButton {
                    text: "About"
                    onClicked: {
                        aboutDialog.open();
                    }
                }
            }
        }
    }

    // Text field for chatting
    TextField {
        id: messageField

        property bool shouldUpdateCurrentMessage: true

        padding: 15
        rightPadding: padding + sendButton.width + sendButton.anchors.rightMargin + sendButton.anchors.leftMargin
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sidebar.left
        }
        text: ircModel.currentChannel !== null ? ircModel.currentChannel.currentMessage : ""
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
        enabled: ircModel.currentServer !== null ? ircModel.currentServer.serverSettings.isConnected : true
        onTextChanged: {
            if (ircModel.currentChannel !== null) {
                if (shouldUpdateCurrentMessage && ircModel.currentChannel.currentMessage !== messageField.text) {
                    ircModel.currentChannel.currentMessage = text
                }
            }
        }
        Keys.onReturnPressed: {
            if (ircModel.currentChannel !== null)
                sendCurrentMessage()
        }
        Keys.onUpPressed: {
            if (ircModel.currentChannel !== null)
                ircModel.currentChannel.getSentMessagesUp()
        }
        Keys.onDownPressed: {
            if (ircModel.currentChannel !== null)
                ircModel.currentChannel.getSentMessagesDown()
        }
        Keys.onTabPressed: {
            if (ircModel.currentChannel !== null)
                ircModel.currentChannel.autoCompleteNick()
        }

        Button {
            id: sendButton
            text: "Send!"
            width: implicitWidth * 1.7
            color: "#666"
            textColor: "#fff"
            font.bold: true
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 8
            }
            onClicked: {
                if (ircModel.currentChannel !== null)
                    sendCurrentMessage();
            }
        }
    }

    // Dialog for disconnecting from all servers
    QueryDialog {
        id: areYouSureToDisconnectAllDialog
        text: "Do you want to disconnect from all servers?"
        onAccepted: {
            ircModel.disconnectFromServers();
            startPage.animateIn();
            chatPage.animateOut();
        }
    }
}
