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
    id: firstrunPage

    property bool isValid: serverUrlField.text.length > 0 && nicknameField.text.length > 0 && serverPortField.acceptableInput
    property ServerSettings server: appSettings.serverSettings.getItem(0)

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            if (appSettings.areSettingsDeleted)
                settingsDeletedBanner.show();
        }
    }

    tools: ToolBarLayout {
        id: commonToolbar

        ToolIcon {
            platformIconId: "toolbar-done"
            onClicked: {
                if (isValid) {
                    commonMenu.close();
                    appWindow.pageStack.push(progressPage);
                    appSettings.saveServerSettings();
                    ircModel.connectToServer(server, appSettings);
                }
                else {
                    invalidBanner.show();
                }
            }

        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status == DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
        }
    }

    Flickable {
        id: configFlickable
        anchors.fill: parent

        interactive: true
        contentWidth: parent.width
        contentHeight: configColumn.height + 30
        clip: true

        Column {
            id: configColumn
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            spacing: 10

            TitleLabel {
                text: "Server settings"
            }
            Label {
                text: "Server URL"
            }
            TextField {
                id: serverUrlField
                width: parent.width
                text: server.serverUrl
                placeholderText: "Enter a server URL"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "serverUrl"
                value: serverUrlField.text
            }
            Label {
                text: "Server Port"
            }
            Row {
                spacing: 10
                width: parent.width

                TextField {
                    id: serverPortField
                    width: parent.width - sslCheckbox.width - parent.spacing
                    text: server.serverPort
                    placeholderText: "Enter the server port"
                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhDigitsOnly
                    validator: IntValidator { }
                }
                Binding {
                    target: server
                    property: "serverPort"
                    value: serverPortField.text
                }
                CheckBox {
                    id: sslCheckbox
                    text: "SSL"
                    checked: server.serverSSL
                    anchors.verticalCenter: serverPortField.verticalCenter
                    onClicked: {
                        if (sslCheckbox.checked)
                            serverPortField.text = 7000
                        else
                            serverPortField.text = 6667
                    }
                }
                Binding {
                    target: server
                    property: "serverSSL"
                    value: sslCheckbox.checked
                }
            }
            Label {
                text: "Password"
            }
            TextField {
                id: passwordField
                width: parent.width
                text: server.serverPassword
                placeholderText: "If it's needed, enter a password"
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "serverPassword"
                value: passwordField.text
            }
            Label {
                text: "Autojoin Channels"
            }
            TextField {
                id:autojoinField
                width: parent.width
                text: server.autoJoinChannelsInPlainString
                placeholderText: "Enter channels to autojoin"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "autoJoinChannelsInPlainString"
                value: autojoinField.text
            }

            TitleLabel {
                text: "User settings"
            }
            Label {
                text: "Your nickname"
            }
            TextField {
                id: nicknameField
                width: parent.width
                text: appSettings.userNickname
                placeholderText: "Enter your nickname"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: appSettings
                property: "userNickname"
                value: nicknameField.text
            }
            Label {
                text: "Your ident"
            }
            TextField {
                id: identField
                width: parent.width
                text: appSettings.userIdent
                placeholderText: "Enter your ident"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: appSettings
                property: "userIdent"
                value: identField.text
            }
            Label {
                text: "Your Real Name"
            }
            TextField {
                id: realNameField
                width: parent.width
                text: appSettings.userRealName
                placeholderText: "If you wish, enter your real name"
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            Binding {
                target: appSettings
                property: "userRealName"
                value: realNameField.text
            }
        }
    }

    InfoBanner {
        id: invalidBanner
        text: "The data you entered is invalid. Please fix it and press the Done button again."
    }

    InfoBanner {
        id: settingsDeletedBanner
        text: "The new version of the app is incompatible with the old, so your settings have been deleted. Click on this banner to dismiss."
        timerEnabled: false
    }
}
