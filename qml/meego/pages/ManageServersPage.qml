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

Page {
    id: manageServersPage
    tools: ToolBarLayout {
        id: startPageToolbar

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: {
                // TODO: connect/disconnect
                appWindow.pageStack.pop()
            }
        }
    }

    Flickable {
        id: manageServersPageFlickable
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

            ServerSettingsList {
                id: serverSettingsList
                onServerChosen: {
                    serverSettingsSheet.isNewServer = isNewServer
                    serverSettingsSheet.serverSettings = server
                    serverSettingsSheet.open()
                }
            }
        }
    }
}
