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

Button {
    id: button
    color: "transparent"
    textColor: "#fff"
    textCenter: false
    font.bold: true
    width: parent.width
    useGradientOverlay: false
    hoverEnabled: true

    onEntered: {
        //console.log("button entered");
        font.underline = true;
        channelMenuEnterArea.visible = true;
        channelMenuEnterArea.enabled = true;
        channelMenuHideTimer.stop();
        channelMenuTimer.start();
        channelMenuEnterArea.z += 10
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

            MenuButton {
                text: "View topic"
            }
            MenuButton {
                text: "User list (138)"
            }
            MenuButton {
                text: "Part"
            }
        }
    }
}
