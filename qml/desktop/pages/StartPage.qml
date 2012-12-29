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
import "../specialcomponents"

Page {
    id: startPage

    Text {
        id: welcomeText
        text: "Welcome to IRC Chatter"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: serversRow.top
        anchors.bottomMargin: 10
        color: "#fff"
        font.pointSize: 24
    }

    Column {
        id: serversRow
        anchors.centerIn: parent
        width: parent.width / 2
        spacing: 5

        Text {
            text: "Your configured servers"
            color: "#fff"
        }
        Rectangle {
            width: serversRow.width
            height: 1
            color: "#fff"
        }
        ServerSettingEntry {
            serverName: "irc.freenode.net:7000"
            userName: "Venemo"
            serverEnabled: true
        }
        ServerSettingEntry {
            serverName: "irc.gnome.org:6667"
            userName: "Venemo"
        }
    }

    Button {
        text: "Connect!"
        color: "#9fce00"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: serversRow.bottom
        anchors.topMargin: 10
        onClicked: {
            startPage.animateOut();
            chatPage.animateIn();
        }
    }

}
