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

    Rectangle {
        id: titleRectangle
        color: "#f9a300"
        height: 70
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

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
    Flickable {
        id: startPageFlickable
        interactive: contentHeight > height
        contentWidth: parent.width
        contentHeight: serverSettingsColumn.height + 50
        clip: true
        anchors.top: titleRectangle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Column {
            id: serverSettingsColumn
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: parent.top

            Label {
                text: "Please select which servers to connect to at startup. Press the add button to add a new server."
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
            }
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
