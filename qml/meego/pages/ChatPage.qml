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
    property bool isCurrentServerConnected: ircModel.currentServer !== null ? ircModel.currentServer.serverSettings.isConnected : false
    property bool isCurrentServerConnecting: ircModel.currentServer !== null ? ircModel.currentServer.serverSettings.isConnecting : false

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
        anchors.bottom: messageToolBar.top
        anchors.left: parent.left
        anchors.right: channelNameBg.left

        interactive: true
        contentHeight: chatArea.height
        clip: true

        TextArea {
            id: chatArea
            width: parent.width
            readOnly: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.RichText
            enableSoftwareInputPanel: false
            text: ircModel.currentChannel !== null ? ircModel.currentChannel.channelText : ""
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            onTextChanged: {
                scrollToBottom()
            }
            Component.onCompleted: {
                // chatArea.children[0] is the border image inside the TextArea
                chatArea.children[0].anchors.topMargin = -600
                chatArea.children[0].anchors.leftMargin = -100
                chatArea.children[0].anchors.bottomMargin = -600
            }
            font.pixelSize: appSettings.fontSize

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

    // UI element for showing connected/disconnected status
    Rectangle {
        color: "#99000000"
        anchors.fill: chatFlickable
        visible: !isCurrentServerConnected

        MouseArea {
            anchors.fill: parent
            enabled: parent.visible
        }

        Label {
            visible: !isCurrentServerConnecting && ircModel.currentServer !== null
            anchors.centerIn: parent
            width: parent.width * 4 / 5
            horizontalAlignment: Text.AlignHCenter
            color: "#fff"
            text: "You are disconnected from this IRC server. Please establish a network connection to reconnect."
        }

        BusyIndicator {
            id: connectingIndicator
            visible: isCurrentServerConnecting || ircModel.currentServer === null
            running: visible
            anchors.centerIn: parent
            platformStyle: BusyIndicatorStyle {
                size: "large"
                inverted: true
            }
        }
        Label {
            visible: isCurrentServerConnecting && ircModel.currentServer !== null
            width: parent.width * 4 / 5
            horizontalAlignment: Text.AlignHCenter
            anchors {
                topMargin: 15
                top: connectingIndicator.bottom
                horizontalCenter: parent.horizontalCenter
            }
            color: "#fff"
            text: "Connecting to this IRC server, please wait..."
        }
        Label {
            visible: ircModel.currentServer === null
            width: parent.width * 4 / 5
            horizontalAlignment: Text.AlignHCenter
            anchors {
                topMargin: 15
                top: connectingIndicator.bottom
                horizontalCenter: parent.horizontalCenter
            }
            color: "#fff"
            text: "Connecting, please wait..."
        }
    }

    Rectangle {
        id: channelNameBg
        color: "#f9a300"
        width: 60
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: messageToolBar.top
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
    ToolBar {
        id: messageToolBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        tools: ToolBarLayout {

            ToolIcon {
                id: autoCompleteToolIcon
                platformIconId: "toolbar-reply"
                enabled: messageField.enabled
                onClicked: {
                    messageField.forceActiveFocus()
                    ircModel.currentChannel.autoCompleteNick()
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
            }
            TextField {
                id: messageField
                enabled: ircModel.currentServer !== null ? ircModel.currentServer.serverSettings.isConnected : true
                placeholderText: enabled ? "Type a message" : "Disconnected"
                text: ircModel.currentChannel !== null ? ircModel.currentChannel.currentMessage : ""
                platformSipAttributes: mySipAttributes
                onTextChanged: {
                    if (ircModel.currentChannel !== null) {
                        if (shouldUpdateCurrentMessage && ircModel.currentChannel.currentMessage !== messageField.text) {
                            ircModel.currentChannel.currentMessage = text
                        }
                    }
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: autoCompleteToolIcon.right
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
                text: "Settings"
                onClicked: appWindow.pageStack.push(settingsPage)
            }
            MenuItem {
                text: "Manage servers"
                onClicked: appWindow.pageStack.push(manageServersPage)
            }
            MenuItem {
                text: "About"
                onClicked: aboutDialog.open()
            }
        }
    }
    JoinSheet {
        id: joinSheet
        visualParent: chatPage
        onAccepted: {
            ircModel.currentServer.joinChannel(joinSheet.channelName, joinSheet.channelKey)
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
