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
// Copyright (C) 2013, Timur Kristóf <venemo@fedoraproject.org>

import QtQuick 2.0
import "../components"
import "../dialogs"
import "../misc"

Page {
    id: settingsPage

    Column {
        id: settingsColumn
        width: settingsPage.width * 0.5
        spacing: 25
        anchors {
            top: settingsPage.top
            topMargin: 20
            horizontalCenter: settingsPage.horizontalCenter
        }

        Text {
            color: "#fff"
            font.pointSize: 24
            text: "Settings"
        }
        // SETTING GROUP
        TitleLabel {
            color: "#fff"
            text: "Appearance"
        }
        // SETTING: show misc events
        Text {
            color: "#fff"
            text: "Show join/part/quit/nick events"
            width: parent.width

            Switch {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        // SETTING: show timestamps
        Text {
            color: "#fff"
            text: "Show timestamps"
            width: parent.width

            Switch {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        // SETTING: monospace font
        Text {
            color: "#fff"
            text: "Use monospace font for chat"
            width: parent.width

            Switch {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        // SETTING: autofocus text field
        Text {
            color: "#fff"
            text: "Autofocus text field"
            width: parent.width

            Switch {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        // SETTING: color of the sidebar
        Text {
            color: "#fff"
            text: "Sidebar color"
            width: parent.width
            z: 1

            ComboBox {
                width: 350
                text: "Orange (default)"
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                model: ListModel {
                    ListElement { text: "Orange (Default)"; value: "#f9a300" }
                    ListElement { text: "Green";            value: "#9fce00" }
                    ListElement { text: "Red";              value: "#ff0000" }
                    ListElement { text: "Blue";             value: "#0000ff" }
                    ListElement { text: "Grey";             value: "#999999" }
                    ListElement { text: "Purple";           value: "#400758" }
                    ListElement { text: "Yellow";           value: "#cec700" }
                }
            }
        }
        // SETTING GROUP
        TitleLabel {
            color: "#fff"
            text: "Send notifications for"
        }
        // SETTING: notifications / queries
        Text {
            color: "#fff"
            text: "Messages in queries"
            width: parent.width

            Switch {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        // SETTING: notifications / messages with your nick
        Text {
            color: "#fff"
            text: "Messages containing your nick"
            width: parent.width

            Switch {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
            }
        }
        // SETTING GROUP
        TitleLabel {
            color: "#fff"
            text: "Customizations"
        }
        // SETTING: Quit message
        Text {
            color: "#fff"
            text: "Quit message"
            width: parent.width

            TextField {
                width: 350
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                tooltipText: "It will appear when you quit from the app."
            }
        }
        // SETTING: Part message
        Text {
            color: "#fff"
            text: "Part message"
            width: parent.width

            TextField {
                width: 350
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                tooltipText: "This will appear when you leave a channel."
            }
        }
        // SETTING: Kick message
        Text {
            color: "#fff"
            text: "Kick message"
            width: parent.width

            TextField {
                width: 350
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                tooltipText: "This will appear when you kick someone from a channel."
            }
        }

    }

    Button {
        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: 10
        }
        text: "Back"
        color: "#676767"
        textColor: "#fff"
        width: 130
        font.bold: true
        onClicked: {
            startPage.animateIn();
            settingsPage.animateOut();
        }
    }
}
