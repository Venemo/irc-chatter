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

Dialog {
    id: serverSettingsDialog

    property bool isValid: hostnameField.acceptableInput && nicknameField.acceptableInput && serverPortField.acceptableInput
    property bool isNewServer: true
    property ServerSettings serverSettings: null

    title: "Server settings"
    acceptButtonText: "Save"
    rejectButtonText: isNewServer ? "Cancel" : ""
    cantAcceptTooltipText: "The settings are incorrect. Cannot save the server."
    canAccept: visible && isValid

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
            text: serverSettings === null ? "" : serverSettings.serverUrl
            tooltipText: "The hostname of the server you want to connect to."
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            KeyNavigation.tab: serverPortField

            Binding {
                target: serverSettings
                property: "serverUrl"
                value: hostnameField.text
                when: canAccept
            }
        }

        // Server port
        Text {
            text: "Port"
            color: "#fff"
            height: serverPortField.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField {
            id: serverPortField
            width: appWindow.width * 0.35
            enableTextValidation: true
            text: serverSettings === null ? "" : serverSettings.serverPort
            tooltipText: "Depends on the server. Default is 6667 or 7000 (used by most servers)."
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            KeyNavigation.tab: sslSwitch
            validator: IntValidator { }

            Binding {
                target: serverSettings
                property: "serverPort"
                value: serverPortField.text
                when: canAccept && serverPortField.text != serverSettings.serverPort
            }
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
                if (value && serverSettings.serverPort === 6667) {
                    serverSettings.serverPort = 7000;
                }
                else if (!value && serverSettings.serverPort === 7000) {
                    serverSettings.serverPort = 6667;
                }
            }
            value: serverSettings === null ? false : serverSettings.serverSSL
            border.width: 0
            KeyNavigation.tab: nicknameField

            Binding {
                target: serverSettings
                property: "serverSSL"
                value: sslSwitch.value
                when: canAccept
            }
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
            text: serverSettings === null ? "" : serverSettings.userNickname
            tooltipText: "Your nick name. This will appear to everyone else."
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            KeyNavigation.tab: realNameField

            Binding {
                target: serverSettings
                property: "userNickname"
                value: nicknameField.text
                when: canAccept
            }
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
            text: serverSettings === null ? "" : serverSettings.userRealName
            tooltipText: "Your real name. This will appear if someone uses the whois command on you."
            inputMethodHints: Qt.ImhNoPredictiveText
            KeyNavigation.tab: identField

            Binding {
                target: serverSettings
                property: "userRealName"
                value: realNameField.text
                when: canAccept
            }
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
            text: serverSettings === null ? "" : serverSettings.userIdent
            tooltipText: "Username used for authenticating on the server. Some servers require it."
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            KeyNavigation.tab: passwordField

            Binding {
                target: serverSettings
                property: "userIdent"
                value: identField.text
                when: canAccept
            }
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
            text: serverSettings === null ? "" : serverSettings.serverPassword
            tooltipText: "Password for authenticating on the server. Some servers require it."
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            KeyNavigation.tab: hostnameField
            echoMode: TextInput.Password

            Binding {
                target: serverSettings
                property: "serverPassword"
                value: passwordField.text
                when: canAccept
            }
        }
    }
}
