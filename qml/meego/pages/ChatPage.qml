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
// Copyright (C) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import net.venemo.ircchatter 1.0
import "../pages"
import "../sheets"
import "../components"

Page {
    property bool shouldUpdateCurrentMessage: true

    function sendCurrentMessage() {
        if (ircModel.currentChannel !== null) {
            shouldUpdateCurrentMessage = false
            ircModel.currentChannel.sendCurrentMessage()
            shouldUpdateCurrentMessage = true
        }
    }
    function switchChannel(index) {
        shouldUpdateCurrentMessage = false
        ircModel.currentChannelIndex = index
        shouldUpdateCurrentMessage = true
        adjustChatAreaHeight()

        if (appSettings.autoFocusTextField)
            messageField.forceActiveFocus()
    }
    function scrollToBottom() {
        chatFlickable.contentY = Math.max(0,  chatArea.height - chatFlickable.height)
    }
    function adjustForOrientationChange() {
        scrollToBottom()
        chatFlickable.lastSupposedContentY = chatFlickable.contentY
    }
    function adjustChatAreaHeight() {
        chatArea.height = Math.max(channelNameBg.height, chatArea.implicitHeight)
        scrollToBottom()
    }

    id: mainPage
    onStatusChanged: {
        if (mainPage.status === PageStatus.Active) {
            commonMenu.close()
            adjustChatAreaHeight()

            chatArea.font.pixelSize = appSettings.fontSize

            if(appSettings.fontMonospace)
                chatArea.font.family = "Monospace"
            else
                chatArea.font.family = "Nokia Pure"

            channelNameBg.color = appSettings.sidebarColor
        }
    }

    Flickable {
        id: chatFlickable
        anchors.top: parent.top
        anchors.bottom: chatRectangle.top
        anchors.left: parent.left
        anchors.right: channelNameBg.left

        interactive: true
        contentHeight: chatArea.height
        clip: true

        property int lastSupposedContentY: 0

        TextArea {
            id: chatArea
            width: parent.width
            readOnly: true
            wrapMode: TextEdit.WordWrap
            textFormat: TextEdit.RichText
            text: ircModel.currentChannel !== null ? ircModel.currentChannel.channelText : ""
            font.pixelSize: appSettings.fontSize

            onTextChanged: {
                var should = Math.max(0,  chatArea.height - chatFlickable.height)
                if (chatFlickable.contentY >= chatFlickable.lastSupposedContentY - 220)
                    chatFlickable.lastSupposedContentY = chatFlickable.contentY = should
            }

            Connections {
                // chatArea.children[3] is the TextEdit inside the TextArea
                target: chatArea.children[3]
                onLinkActivated: {
                    // User queries
                    if (!link.indexOf("user://")) {
                        areYouSureToQueryDialog.queryableUserName = link.replace("user://", "")
                        areYouSureToQueryDialog.open()
                    }
                    // Normal links
                    else if (link.indexOf("://") || !(link.indexOf("www.")))
                        Qt.openUrlExternally(link)
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: chatFlickable
    }
    Rectangle {
        id: channelNameBg
        color: isPreRelease ? "#73ba0e" : "#f9a300"
        width: 60
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: chatRectangle.top
        onHeightChanged: {
            adjustChatAreaHeight()
        }

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
                        color: isCurrent ? "white" : (hasNewMessageWithUserNick ? "red" : (hasNewMessage ? "blue" : "black"))
                        font.bold: isCurrent ? true : false

                        property bool hasNewMessage: false
                        property bool hasNewMessageWithUserNick: false
                        property bool isCurrent: ircModel.currentChannelIndex === index

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

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                hasNewMessage = false
                                hasNewMessageWithUserNick = false
                                switchChannel(index)
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
    Rectangle {
        id: chatRectangle
        gradient: Gradient {
            GradientStop {
                color: "#777777"
                position: 0
            }
            GradientStop {
                color: "#bbbbbb"
                position: 0.5
            }
        }
        height: messageField.implicitHeight * 1.2
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ToolIcon {
            id: tabButton
            platformIconId: "toolbar-reply"
            onClicked: {
                messageField.forceActiveFocus()
                ircModel.currentChannel.autoCompleteNick()
            }
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
        }
        TextField {
            id: messageField
            placeholderText: "Type a message"
            text: ircModel.currentChannel !== null ? ircModel.currentChannel.currentMessage : ""
            onTextChanged: {
                if (ircModel.currentChannel !== null && ircModel.currentChannel.currentMessage !== messageField.text && shouldUpdateCurrentMessage)
                    ircModel.currentChannel.currentMessage = text
            }
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            platformSipAttributes: mySipAttributes
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: tabButton.right
            anchors.right: menuToolIcon.left
            Keys.onReturnPressed: {
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
            SipAttributes {
                id: mySipAttributes
                actionKeyEnabled: true
                actionKeyHighlighted: true
                actionKeyLabel: "Send!"
            }
        }
        ToolIcon {
            id: menuToolIcon
            platformIconId: "toolbar-view-menu"
            onClicked: (chatMenu.status === DialogStatus.Closed) ? chatMenu.open() : chatMenu.close()
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
        }
    }
    Menu {
        id: chatMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: (ircModel.currentChannel !== null && ircModel.currentChannel.name.charAt(0)) === '#' ? "Part" : "Close"
                onClicked:  areYouSureToPartDialog.open()
            }
            MenuItem {
                text: "Join/Query"
                onClicked: {
                    chatMenu.close()
                    joinSheet.open()
                }
            }
            MenuItem {
                text: "User list"
                visible: ircModel.currentChannel === null ? false : (ircModel.currentChannel.name.charAt(0) === '#')
                onClicked: {
                    chatMenu.close()
                    userSelectorDialog.open()
                }
            }
            MenuItem {
                text: "About"
                onClicked: aboutDialog.open()
            }
            MenuItem {
                text: "Settings"
                onClicked: appWindow.pageStack.push(settingsPage)
            }
            MenuItem {
                text: "Disconnect all"
                onClicked: {
                    chatMenu.close()
                    areYouSureToDisconnectAllDialog.open()
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
            ircModel.currentServer.joinChannel(joinSheet.channelName)
        }
    }
    QueryDialog {
        id: areYouSureToPartDialog
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        titleText: "Are you sure?"
        onAccepted: {
            ircModel.currentServer.partChannel(ircModel.currentChannel.name)
        }
        onStatusChanged: {
            if (ircModel.currentChannel !== null) {
                if (ircModel.currentChannel.name.charAt(0) === '#')
                    message = "Do you want to part this channel?"
                else
                    message = "Do you want to close this conversation?"
            }
        }
    }
    QueryDialog {
        property string queryableUserName: ""
        id: areYouSureToQueryDialog
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        titleText: "Are you sure?"
        message: "Would you like to query user " + areYouSureToQueryDialog.queryableUserName + " ?"
        onAccepted: {
            ircModel.currentServer.joinChannel(areYouSureToQueryDialog.queryableUserName)
        }
    }
    QueryDialog {
        id: areYouSureToDisconnectAllDialog
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        titleText: "Are you sure?"
        message: "Would you like to disconnect from all servers?"
        onAccepted: {
            ircModel.disconnectFromServers()
            appWindow.pageStack.pop(chatPage);
        }
    }
    WorkingSelectionDialog {
        id: userSelectorDialog
        titleText: "User list of " + (ircModel.currentChannel === null ? "?" : ircModel.currentChannel.name)
        model: ircModel.currentChannel === null ? null : ircModel.currentChannel.users
        onAccepted: {
            if (userSelectorDialog.selectedIndex >= 0) {
                areYouSureToQueryDialog.queryableUserName = ircModel.currentChannel.getUserNameFromIndex(userSelectorDialog.selectedIndex)
                areYouSureToQueryDialog.open()
            }
        }
        onStatusChanged: {
            if (status === DialogStatus.Opening)
                selectedIndex = -1
        }

        searchFieldVisible: true
    }
}
