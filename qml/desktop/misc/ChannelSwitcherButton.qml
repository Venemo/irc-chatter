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
    color: "transparent"
    textColor: "#fff"
    textCenter: false
    font.bold: true
    width: parent.width
    useGradientOverlay: false
    hoverEnabled: true

    onEntered: {
        font.underline = true;
        channelMenuTimer.start();
    }
    onExited: {
        font.underline = false;
        channelMenuTimer.stop();
        channelMenu.close();
    }

    Timer {
        id: channelMenuTimer
        repeat: false
        running: false
        triggeredOnStart: false
        interval: 300
        onTriggered: {
            channelMenu.open();
        }
    }

    Menu {
        id: channelMenu
        visible: false
        showRightTab: true
        anchors.right: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 0

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
