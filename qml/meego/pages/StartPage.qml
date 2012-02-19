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
    id: startPage
    onStatusChanged: {
        if (status === PageStatus.Activating) {
            if (appSettings.areSettingsDeleted)
                settingsDeletedBanner.show()
        }
    }
    tools: ToolBarLayout {
        id: startPageToolbar

        ToolIcon {
            platformIconId: "toolbar-done"
            onClicked: {
                commonMenu.close()
                if (ircModel.anyServersToConnect()) {
                    appWindow.pageStack.push(progressPage)
                    ircModel.connectToServers()
                }
                else {
                    noServersToConnectBanner.show()
                }
            }

        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status === DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
        }
    }

    Flickable {
        id: startPageFlickable
        interactive: true
        contentWidth: parent.width
        contentHeight: serverSettingsColumn.height + 50
        clip: true
        anchors.fill: parent

        Column {
            id: serverSettingsColumn
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: parent.top

            TitleLabel {
                text: "Welcome to IRC Chatter!"
            }
            Label {
                text: "Please select which servers to connect to at startup."
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label {
                text: "Press the add button to add a new server."
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Repeater {
                id: serverSettingsRepeater
                model: appSettings.serverSettings
                width: parent.width
                delegate: Item {
                    id: serverSettingItem
                    width: serverSettingsRepeater.width
                    height: 150
                    anchors.margins: 10

                    TitleLabel {
                        id: serverTitleLabel
                        text: serverUrl
                        anchors.top: serverSettingItem.top
                        anchors.margins: 10
                    }
                    Label {
                        id: serverInfoLabel
                        text: "Server: " + serverUrl + ":" + serverPort + "\nNick: " + userNickname
                        anchors.margins: 10
                        anchors.left: parent.left
                        anchors.top: serverTitleLabel.bottom
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Button {
                        id: serverEditButton
                        text: "Edit server"
                        onClicked: {
                            serverSettingsSheet.isNewServer = false
                            serverSettingsSheet.serverSettings = appSettings.serverSettings.getItem(index)
                            serverSettingsSheet.open()
                        }
                        anchors.top: connectSwitch.bottom
                        anchors.right: parent.right
                        anchors.topMargin: 10
                    }
                    Label {
                        id: connectLabel
                        text: "Connect"
                        anchors.verticalCenter: connectSwitch.verticalCenter
                        anchors.right: connectSwitch.left
                        anchors.rightMargin: 10
                    }
                    Switch {
                        id: connectSwitch
                        checked: shouldConnect
                        anchors.top: serverTitleLabel.bottom
                        anchors.right: parent.right

                        Binding {
                            target: appSettings.serverSettings.getItem(index)
                            property: "shouldConnect"
                            value: connectSwitch.checked
                        }
                    }
                }
            }
            TitleLabel {
                text: "Other"
            }
            Button {
                id: newServerButton
                text: "Add a new server"
                onClicked: {
                    serverSettingsSheet.isNewServer = true
                    serverSettingsSheet.serverSettings = appSettings.newServerSettings()
                    serverSettingsSheet.open()
                }
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
    ServerSettingsSheet {
        id: serverSettingsSheet
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
            else {
                open()
                invalidBanner.show()
            }
        }
        InfoBanner {
            id: invalidBanner
            text: "The data you entered is invalid. Please fix it and press the save button again."
        }
    }
    InfoBanner {
        id: settingsDeletedBanner
        text: "The new version of the app is incompatible with the old, so your settings have been deleted. Click on this banner to dismiss."
        timerEnabled: false
    }
    InfoBanner {
        id: noServersToConnectBanner
        text: "You didn't select any servers to connect to."
    }
}
