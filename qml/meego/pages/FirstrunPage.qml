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
    property ServerSettings serverSettings: appSettings.serverSettings.getItem(0)

    onStatusChanged: {
        if (status === PageStatus.Activating) {
            if (appSettings.areSettingsDeleted)
                settingsDeletedBanner.show()
        }
    }

    tools: ToolBarLayout {
        id: commonToolbar

        ToolIcon {
            platformIconId: "toolbar-done"
            onClicked: {
                if (isValid) {
                    commonMenu.close()
                    appWindow.pageStack.push(progressPage)
                    appSettings.saveServerSettings()
                    ircModel.connectToServer(serverSettings, appSettings)
                }
                else {
                    invalidBanner.show()
                }
            }

        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status === DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
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
                text: "Server information"
            }
            Label {
                text: "Server hostname"
            }
            TextField {
                id: serverUrlField
                width: parent.width
                text: serverSettings.serverUrl
                placeholderText: "Enter a serverSettings URL"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "serverUrl"
                    value: serverUrlField.text
                }
            }
            Label {
                text: "Server port"
            }
            Row {
                spacing: 10
                width: parent.width

                TextField {
                    id: serverPortField
                    width: parent.width - sslCheckbox.width - parent.spacing
                    text: serverSettings.serverPort
                    placeholderText: "Enter the serverSettings port"
                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhDigitsOnly
                    validator: IntValidator { }

                    Binding {
                        target: serverSettings
                        property: "serverPort"
                        value: serverPortField.text
                    }
                }
                CheckBox {
                    id: sslCheckbox
                    text: "SSL"
                    checked: serverSettings.serverSSL
                    anchors.verticalCenter: serverPortField.verticalCenter
                    onClicked: {
                        if (sslCheckbox.checked)
                            serverPortField.text = 7000
                        else
                            serverPortField.text = 6667
                    }

                    Binding {
                        target: serverSettings
                        property: "serverSSL"
                        value: sslCheckbox.checked
                    }
                }
            }
            Label {
                text: "Autojoin channels (comma separated)"
            }
            TextField {
                id:autojoinField
                width: parent.width
                text: serverSettings.autoJoinChannelsInPlainString
                placeholderText: "Enter channels to autojoin"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "autoJoinChannelsInPlainString"
                    value: autojoinField.text
                }
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
                text: serverSettings.userNickname
                placeholderText: "Enter your nickname"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "userNickname"
                    value: nicknameField.text
                }
            }
            Label {
                text: "Your real name"
            }
            TextField {
                id: realNameField
                width: parent.width
                text: serverSettings.userRealName
                placeholderText: "If you wish, enter your real name"
                inputMethodHints: Qt.ImhNoPredictiveText

                Binding {
                    target: serverSettings
                    property: "userRealName"
                    value: realNameField.text
                }
            }
            TitleLabel {
                text: "Authentication"
            }
            Label {
                text: "Username (aka. ident)"
            }
            TextField {
                id: identField
                width: parent.width
                text: serverSettings.userIdent
                placeholderText: "Enter your ident"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "userIdent"
                    value: identField.text
                }
            }
            Label {
                text: "Password"
            }
            TextField {
                id: passwordField
                width: parent.width
                text: serverSettings.serverPassword
                placeholderText: "If it's needed, enter a password"
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "serverPassword"
                    value: passwordField.text
                }
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
