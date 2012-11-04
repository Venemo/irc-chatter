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
import "../sheets"
import "../components"

// This is the page which appears when the app is started.
// Provides user-friendly screen when the app is started for the first time,
// and a convenient way to select which servers to connect to.
Page {
    id: startPage
    onStatusChanged: {
        if (status === PageStatus.Activating) {
            if (appSettings.areSettingsDeleted)
                settingsDeletedBanner.show()
        }
    }

    // Custom toolbar layout for this page
    tools: ToolBarLayout {
        id: startPageToolbar

        // Adds a new server
        ToolIcon {
            platformIconId: "toolbar-add"
            visible: appSettings.serverSettings.itemCount !== 0
            onClicked: serverSettingsList.showAddNewServer()
        }

        // Connects to the selected servers
        Button {
            text: "Connect!"
            anchors.centerIn: parent
            visible: appSettings.serverSettings.itemCount !== 0
            width: 200

            onClicked: {
                commonMenu.close()
                //appWindow.pageStack.push(chatPage)
                if (ircModel.anyServersToConnect()) {
                    appWindow.pageStack.push(chatPage)
                    ircModel.connectToServers()
                }
                else {
                    noServersToConnectBanner.show()
                }
            }
        }

        // Menu button
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status === DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
        }
    }

    // Contains the application title bar
    Rectangle {
        id: titleRectangle
        color: "#f9a300"
        height: 70
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Title bar text
        TextEdit {
            activeFocusOnPress: false
            color: "#ffffff"
            text: "IRC Chatter"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 30
        }
    }

    // Firstrun experience - user friendly message when no servers are set up
    Item {
        anchors {
            top: titleRectangle.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        visible: appSettings.serverSettings.itemCount === 0

        Column {
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: parent.top
            spacing: 15

            Label {
                text: "Welcome!"
                font {
                   bold: true
                   pixelSize: 30
                }
            }
            Label {
                text: "You haven't set up any servers yet. To add a server, click on the button below."
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Button {
            text: "Add new server"
            anchors.centerIn: parent
            onClicked: serverSettingsList.showAddNewServer()
        }
    }

    // Startup experience - provides UI for selecting servers to connect to
    Flickable {
        id: startPageFlickable
        interactive: contentHeight > height
        contentWidth: parent.width
        contentHeight: serverSettingsColumn.height + 50
        clip: true
        visible: appSettings.serverSettings.itemCount !== 0
        anchors {
            top: titleRectangle.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Column {
            id: serverSettingsColumn
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: parent.top
            spacing: 15

            Label {
                text: "Welcome!"
                font {
                   bold: true
                   pixelSize: 30
                }
            }
            Label {
                text: "Please select which servers to connect to at startup. Press the add button to add a new server."
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
            }
            // This is where the user can select servers to connect to
            ServerSettingsList {
                id: serverSettingsList
                onServerChosen: {
                    serverSettingsSheet.isNewServer = isNewServer
                    serverSettingsSheet.serverSettings = server
                    serverSettingsSheet.open()
                }
                onServerConnectionChanged: {
                    server.shouldConnect = connectToServer
                }
            }
        }
    }
    // Server settings sheet (must be here, otherwise appears incorrectly)
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
    // Show when the current settings version is incompatible with the old
    InfoBanner {
        id: settingsDeletedBanner
        text: "The new version of the app is incompatible with the old, so your settings have been deleted. Click on this banner to dismiss."
        timerEnabled: false
    }
    // Show when no servers are selected
    InfoBanner {
        id: noServersToConnectBanner
        text: "You didn't select any servers to connect to."
    }
}
