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

Sheet {
    property bool isValid: serverUrlField.text.length > 0 && nicknameField.text.length > 0 && serverPortField.acceptableInput
    property bool canSave: status === DialogStatus.Open && isValid
    property bool isNewServer: true
    property ServerSettings serverSettings: null

    id: serverSettingsSheet
    acceptButtonText: "Save"
    rejectButtonText: isNewServer ? "Cancel" : ""
    onAccepted: {
        if (!isValid) {
            open()
            invalidBanner.show()
        }
    }
    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            configFlickable.contentY = 0
        }
    }
    content: Flickable {
        id: configFlickable
        interactive: true
        contentWidth: parent.width
        contentHeight: configColumn.height + 30
        clip: true
        anchors.fill: parent

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
                text: serverSettings === null ? "" : serverSettings.serverUrl
                placeholderText: "Enter a hostname"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "serverUrl"
                    value: serverUrlField.text
                    when: canSave
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
                    text: serverSettings === null ? "" : serverSettings.serverPort
                    placeholderText: "Enter the serverSettings port"
                    inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhDigitsOnly
                    validator: IntValidator { }

                    Binding {
                        target: serverSettings
                        property: "serverPort"
                        value: serverPortField.text
                        when: canSave
                    }
                }
                CheckBox {
                    id: sslCheckbox
                    text: "SSL"
                    checked: serverSettings === null ? false : serverSettings.serverSSL
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
                        when: canSave
                    }
                }
            }
            Label {
                text: "Autojoin channels (comma separated)"
            }
            TextField {
                id:autojoinField
                width: parent.width
                text: serverSettings === null ? "" : serverSettings.autoJoinChannelsInPlainString
                placeholderText: "Enter channels to autojoin"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "autoJoinChannelsInPlainString"
                    value: autojoinField.text
                    when: canSave
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
                text: serverSettings === null ? "" : serverSettings.userNickname
                placeholderText: "Enter your nickname"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "userNickname"
                    value: nicknameField.text
                    when: canSave
                }
            }
            Label {
                text: "Your real name"
            }
            TextField {
                id: realNameField
                width: parent.width
                text: serverSettings === null ? "" : serverSettings.userRealName
                placeholderText: "If you wish, enter your real name"
                inputMethodHints: Qt.ImhNoPredictiveText

                Binding {
                    target: serverSettings
                    property: "userRealName"
                    value: realNameField.text
                    when: canSave
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
                text: serverSettings === null ? "" : serverSettings.userIdent
                placeholderText: "Enter your ident"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "userIdent"
                    value: identField.text
                    when: canSave
                }
            }
            Label {
                text: "Password"
            }
            TextField {
                id: passwordField
                width: parent.width
                text: serverSettings === null ? "" : serverSettings.serverPassword
                placeholderText: "If it's needed, enter a password"
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                Binding {
                    target: serverSettings
                    property: "serverPassword"
                    value: passwordField.text
                    when: canSave
                }
            }
            TitleLabel {
                text: "Delete"
            }
            Button {
                text: "Delete server"
                onClicked: reject()
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }


    InfoBanner {
        id: invalidBanner
        text: "The data you entered is invalid. Please fix it and press the save button again."
    }
}
