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

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import net.venemo.ircchatter 1.0
import "../sheets"
import "../components"

// This is the manage servers page.
// Purpose is to edit and toggle server settings
Page {
    id: manageServersPage

    // Custom toolbar
    tools: ToolBarLayout {
        id: startPageToolbar

        // Back button
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: {
                appWindow.pageStack.pop()
            }
        }

        // Adds a new server
        ToolIcon {
            platformIconId: "toolbar-add"
            onClicked: serverSettingsList.showAddNewServer()
        }

        // Menu button
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status === DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
        }
    }
    onStatusChanged: {
        if (status === PageStatus.Activating) {
            // Activate back bindings to make the UI reflect the actual settings
            serverSettingsList.bindConnectionBack = true
            // Deactivate back bindings to prevent binding loops
            serverSettingsList.bindConnectionBack = false
        }
    }

    // Flickable area for the server settings
    Flickable {
        id: manageServersPageFlickable
        interactive: contentHeight > height
        contentWidth: parent.width
        contentHeight: serverSettingsColumn.height + 50
        clip: true
        anchors.fill: parent

        // Contains all the server settings
        Column {
            id: serverSettingsColumn
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: parent.top

            // Displays the server settings list
            ServerSettingsList {
                id: serverSettingsList
                bindConnectionBack: false
                onServerChosen: {
                    if (!server.isConnected && !server.isConnecting) {
                        serverSettingsSheet.isNewServer = isNewServer
                        serverSettingsSheet.serverSettings = server
                        serverSettingsSheet.open()
                    }
                    else {
                        // User can only edit a server setting if it's disconnected
                        cantEditConnectedServerBanner.show()
                    }
                }
                onServerConnectionChanged: {
                    if (manageServersPage.status === PageStatus.Active) {
                        if (connectToServer) {
                            areYouSureToConnectDialog.currentServer = server
                            areYouSureToConnectDialog.open()
                        }
                        else if (!connectToServer) {
                            areYouSureToDisconnectDialog.currentServer = server
                            areYouSureToDisconnectDialog.open()
                        }
                    }
                }
            }
        }
    }
    // Shows are you sure to connect
    QueryDialog {
        property ServerSettings currentServer: null

        id: areYouSureToConnectDialog
        titleText: "Are you sure?"
        message: "Do you want to connect to this server?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: {
            currentServer.shouldConnect = true
            ircModel.connectToServer(currentServer)
        }
        onRejected: {
            serverSettingsList.bindConnectionBack = true;
            currentServer.shouldConnect = false
            serverSettingsList.bindConnectionBack = false;
        }
    }
    // Shows are you sure to disconnect
    QueryDialog {
        property ServerSettings currentServer: null

        id: areYouSureToDisconnectDialog
        titleText: "Are you sure?"
        message: "Do you want to disconnect from this server?"
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: {
            currentServer.shouldConnect = false
            ircModel.disconnectFromServer(currentServer)
        }
        onRejected: {
            serverSettingsList.bindConnectionBack = true;
            currentServer.shouldConnect = true
            serverSettingsList.bindConnectionBack = false;
        }
    }
    // Server settings sheet (should be here because otherwise it appears buggily)
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
        }
        onRejected: {
            if (!isNewServer) {
                areYouSureToDeleteServerDialog.serverSettingsSheet = serverSettingsSheet
                areYouSureToDeleteServerDialog.open()
            }
        }
    }
    // Shows info that you must disconnect before editing the server settings
    InfoBanner {
        id: cantEditConnectedServerBanner
        text: "You must disconnect from the server before editing its settings"
    }
}
