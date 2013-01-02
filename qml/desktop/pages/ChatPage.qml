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
            bottom: chatField.top
            margins: 15
        }
        contentHeight: chatView.height

        MouseArea {
            anchors.fill: chatView
            cursorShape: Qt.IBeamCursor
        }

        TextEdit {
            id: chatView
            textFormat: TextEdit.RichText
            selectByMouse: true
            width: chatFlickable.width
            text: "<span class='time'>07:17</span> <span style='color: green'>Venemo</span>: Hello world!<br />
<span class='time'>07:20</span> <span style='color: blue'>Stskeeps</span>: moo<br />
<span class='time'>07:21</span> <span style='color: red'>faenil</span>: Behold the new IRC Chatter"
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
            anchors.fill: parent
            anchors.margins: 15
            spacing: 0

            ChannelSwitcherButton { text: "irc.freenode.net" }
            ChannelSwitcherButton { text: "#mer" }
            ChannelSwitcherButton { text: "#nemomobile" }
            ChannelSwitcherButton { text: "#irc-chatter" }
            ChannelSwitcherButton { text: "#mer-meeting" }
        }

        Rectangle {
            id: moreOptionsContainer
            color: "#55444444"
            height: chatField.height
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
                        // TODO: confirmation dialog
                        startPage.animateIn();
                        chatPage.animateOut();
                    }
                }
                MenuButton {
                    text: "Manage servers"
                }
                MenuButton {
                    text: "Settings"
                }
                MenuButton {
                    text: "Enable away status"
                }
            }
        }
    }

    // Text field for chatting
    TextField {
        id: chatField
        padding: 15
        rightPadding: padding + sendButton.width + sendButton.anchors.rightMargin + sendButton.anchors.leftMargin
        text: "Good morning captain!"
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sidebar.left
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
        }
    }
}
