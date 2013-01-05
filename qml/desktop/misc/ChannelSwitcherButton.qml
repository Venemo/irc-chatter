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
import "../components"
import net.venemo.ircchatter 1.0

Button {
    id: button

    property variant channel: null
    readonly property bool isServer: channel.channelType === 0
    readonly property bool isChannel: channel.channelType === 1
    readonly property bool isQuery: channel.channelType === 2

    readonly property bool isConnected: channel.server.serverSettings.isConnected
    readonly property bool isConnecting: channel.server.serverSettings.isConnecting

    property bool isCurrent: false
    property bool hasNewMessage: false
    property bool hasNewMessageWithUserNick: false

    color: "transparent"
    textCenter: false
    width: parent.width
    useGradientOverlay: false
    hoverEnabled: true
    text: model.object.name
    textColor: isCurrent ? "white" : (hasNewMessageWithUserNick ? "red" : (hasNewMessage ? "blue" : "black"))
    font.bold: isCurrent ? true : false

    onClicked: {
        hasNewMessage = false;
        hasNewMessageWithUserNick = false;
    }
    onEntered: {
        //console.log("button entered");
        font.underline = true;
        channelMenuEnterArea.visible = true;
        channelMenuEnterArea.enabled = true;
        channelMenuHideTimer.stop();
        channelMenuTimer.start();
    }
    onExited: {
        //console.log("button exited");
        font.underline = false;
        channelMenuTimer.stop();
        channelMenuHideTimer.start();

        if (channelMenu.opacity === 0) {
            channelMenuEnterArea.visible = false;
            channelMenuEnterArea.enabled = false;
        }
    }

    Timer {
        id: channelMenuTimer
        repeat: false
        running: false
        triggeredOnStart: false
        interval: 300
        onTriggered: {
            //console.log("opening channelMenu");
            channelMenu.open();
        }
    }
    Timer {
        id: channelMenuHideTimer
        repeat: false
        running: false
        triggeredOnStart: false
        interval: 100
        onTriggered: {
            channelMenu.close();
            channelMenuEnterArea.enabled = false;
        }
    }
    MouseArea {
        id: channelMenuEnterArea
        z: button.z + 1
        enabled: false
        visible: false
        anchors.right: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 0
        height: channelMenu.height
        width: channelMenu.width
        hoverEnabled: true
        onEntered: {
            //console.log("channelMenuEnterArea entered")
            channelMenuHideTimer.stop();
        }
        onExited: {
            //console.log("channelMenuEnterArea exited")
            channelMenuHideTimer.start();
        }
        Menu {
            id: channelMenu
            z: button.z + 1
            visible: false
            showRightTab: true
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            onVisibleChanged: {
                if (!channelMenu.visible) {
                    channelMenuEnterArea.visible = false;
                    channelMenuEnterArea.enabled = false;
                }
            }

            Text {
                text: "Connecting..."
                color: "#fff"
                visible: isConnecting
                font.bold: true
            }
            Text {
                text: "Not connected"
                color: "#fff"
                visible: !isConnected && !isConnecting
                font.bold: true
            }
            MenuButton {
                text: "User list (138)"
                visible: isChannel && isConnected
                // TODO: onClicked
            }
            MenuButton {
                text: "Part"
                visible: isChannel && isConnected
                onClicked: {
                    channel.server.partChannel(channel.name);
                }
            }
            MenuButton {
                text: "Close"
                visible: isQuery && isConnected
                onClicked: {
                    channel.server.partChannel(channel.name);
                }
            }
            MenuButton {
                text: "Disconnect"
                visible: isServer && isConnected
                onClicked: {
                    channel.server.disconnectFromServer();
                }
            }
            MenuButton {
                text: "Reconnect"
                visible: isServer && !isConnected && !isConnecting
                onClicked: {
                    channel.server.connectToServer();
                }
            }
            MenuButton {
                text: "Join / query"
                visible: isServer && isConnected
                // TODO: onClicked
            }
        }
    }
}
