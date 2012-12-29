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
    id: chatPage
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ddd" }
        GradientStop { position: 0.4; color: "#fff" }
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
            anchors.fill: parent
            anchors.margins: 15
            spacing: 0

            ChannelSwitcherButton { text: "irc.freenode.net" }
            ChannelSwitcherButton { text: "#mer" }
            ChannelSwitcherButton {
                text: "#nemomobile"
                font.underline: true

                Bubble {
                    color: "#444"
                    spacing: 1
                    padding: 5
                    radius: 5
                    anchors.right: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 10
                    border.color: "transparent"
                    inner: [
                        Button { radius: 0; color: "#666"; textColor: "#fff"; text: "Part"; width: parent.width; textCenter: false },
                        Button { radius: 0; color: "#666"; textColor: "#fff"; text: "User list"; width: parent.width; textCenter: false },
                        Button { radius: 0; color: "#666"; textColor: "#fff"; text: "View topic"; width: parent.width; textCenter: false }
                    ]
                }
            }
            ChannelSwitcherButton { text: "#irc-chatter" }
            ChannelSwitcherButton { text: "#mer-meeting" }
        }

        Rectangle {
            color: "#55444444"
            height: chatField.height
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            Text {
                text: "More options..."
                color: "#fff"
                anchors.centerIn: parent
            }
        }
    }

    // Text field for chatting
    TextField {
        id: chatField
        padding: 15
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sidebar.left
        }

        Button {
            text: "Send!"
            width: implicitWidth * 1.7
            color: "#9fce00"
            font.bold: true
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 8
            }
        }
    }
}
