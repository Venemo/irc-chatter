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
import "../dialogs"
import "../misc"

Page {
    id: startPage

    property bool hasServersConfigured: appSettings.serverSettings.itemCount !== 0

    Text {
        id: welcomeText
        text: "Welcome to IRC Chatter"
        anchors.left: serversRow.left
        anchors.bottom: serversRow.top
        anchors.bottomMargin: 20
        color: "#fff"
        font.pointSize: 24
    }
    Column {
        id: serversRow
        anchors.centerIn: parent
        width: parent.width / 2
        spacing: 5

        Text {
            text: "You haven't configured any servers yet. Press the button below to add one."
            color: "#fff"
            width: serversRow.width
            wrapMode: Text.WordWrap
            visible: !hasServersConfigured
        }

        Text {
            text: "Your configured servers"
            color: "#fff"
            visible: hasServersConfigured
        }
        Rectangle {
            width: serversRow.width
            height: 1
            color: "#fff"
            visible: hasServersConfigured
        }
        Repeater {
            model: appSettings.serverSettings
            width: parent.width
            delegate: ServerSettingEntry {
                id: serverSettingEntry
                serverName: model.object.serverUrl + ":" + model.object.serverPort
                userName: model.object.userNickname
                serverEnabled: model.object.shouldConnect
                onClicked: {
                    serverSettingsDialog.serverSettings = model.object;
                    serverSettingsDialog.open();
                }

                Binding {
                    target: model.object
                    property: "shouldConnect"
                    value: serverSettingEntry.serverEnabled
                }
            }
        }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: serversRow.bottom
        anchors.topMargin: 20
        spacing: 5

        Button {
            width: 130
            text: "Connect!"
            color: "#9fce00"
            textColor: "#000"
            onClicked: {
                startPage.animateOut();
                chatPage.animateIn();
            }
            visible: hasServersConfigured
        }
        Button {
            width: 130
            text: "Add server"
            color: "#ddd"
            textColor: "#000"
            onClicked: {
                // TODO: this is a memory leak, fix it
                serverSettingsDialog.serverSettings = appSettings.newServerSettings();
                serverSettingsDialog.open();
            }
        }
        Button {
            width: 130
            text: "Settings"
            color: "#ddd"
            textColor: "#000"
            visible: hasServersConfigured
        }
    }
    ServerSettingsDialog {
        id: serverSettingsDialog
        onAccepted: {
            if (isValid) {
                if (isNewServer) {
                    appSettings.appendServerSettings(serverSettings)
                }
                else {
                    appSettings.serverSettings.reset()
                }
                appSettings.saveServerSettings()
            }
        }
//        onRejected: {
//            if (!isNewServer) {
//                areYouSureToDeleteServerDialog.serverSettingsSheet = serverSettingsSheet
//                areYouSureToDeleteServerDialog.open()
//            }
//        }
    }
}
