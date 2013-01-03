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

Dialog {
    id: serverSettingsDialog
    title: "Server settings"
    acceptButtonText: "Save"
    rejectButtonText: "Cancel"

    onOpened: {
        hostnameField.forceActiveFocus();
    }

    Grid {
        columns: 2
        spacing: 10
        anchors {
            left: parent.left
            right: parent.right
        }

        // Server hostname
        Text {
            text: "Hostname"
            color: "#fff"
            height: hostnameField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: hostnameField
            width: appWindow.width * 0.35
            enableTextValidation: true
            tooltipText: "The hostname of the server you want to connect to."
            KeyNavigation.tab: portField
        }

        // Server port
        Text {
            text: "Port"
            color: "#fff"
            height: portField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: portField
            width: appWindow.width * 0.35
            enableTextValidation: true
            tooltipText: "Depends on the server. Default is 6667 or 7000 (used by most servers)."
            KeyNavigation.tab: sslSwitch
            validator: IntValidator { }
        }

        // Whether to use SSL for the connection
        Text {
            text: "SSL"
            color: "#fff"
            height: sslSwitch.height
            verticalAlignment: Text.AlignVCenter
        }
        Switch {
            id: sslSwitch
            onValueChanged: {
                if (portField.text === "6667") {
                    portField.text = "7000";
                }
                else if (portField.text === "7000") {
                    portField.text = "6667";
                }
            }
            border.width: 0
            KeyNavigation.tab: nicknameField
        }

        // Poor attempt at a separator
        Rectangle {
            height: 20
            width: height
            color: "transparent"
        }
        Rectangle {
            height: 20
            width: height
            color: "transparent"
        }

        // Nickname
        Text {
            text: "Nickname"
            color: "#fff"
            height: nicknameField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: nicknameField
            width: appWindow.width * 0.35
            enableTextValidation: true
            tooltipText: "Your nick name. This will appear to everyone else."
            KeyNavigation.tab: realNameField
        }

        // Real name
        Text {
            text: "Real name"
            color: "#fff"
            height: realNameField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: realNameField
            width: appWindow.width * 0.35
            enableTextValidation: false
            tooltipText: "Your real name. This will appear if someone uses the whois command on you."
            KeyNavigation.tab: identField
        }

        // Poor attempt at a separator
        Rectangle {
            height: 20
            width: height
            color: "transparent"
        }
        Rectangle {
            height: 20
            width: height
            color: "transparent"
        }

        // Server username (aka ident)
        Text {
            text: "Username"
            color: "#fff"
            height: identField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: identField
            width: appWindow.width * 0.35
            enableTextValidation: false
            tooltipText: "Username used for authenticating on the server. Some servers require it."
            KeyNavigation.tab: passwordField
        }

        // Server password
        Text {
            text: "Password"
            color: "#fff"
            height: passwordField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: passwordField
            width: appWindow.width * 0.35
            enableTextValidation: false
            tooltipText: "Password for authenticating on the server. Some servers require it."
            KeyNavigation.tab: hostnameField
        }
    }
}
