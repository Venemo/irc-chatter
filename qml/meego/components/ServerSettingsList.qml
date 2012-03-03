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

Column {
    signal serverChosen(variant server, bool isNewServer)

    width: parent.width

    Repeater {
        id: serverSettingsRepeater
        model: appSettings.serverSettings
        width: parent.width
        delegate: Item {
            id: serverSettingItem
            width: serverSettingsRepeater.width
            height: appWindow.inPortrait ? 220 : 150
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
                anchors.top: appWindow.inPortrait ? serverTitleLabel.bottom : undefined
                anchors.verticalCenter: appWindow.inPortrait ? undefined : parent.verticalCenter
            }
            Button {
                id: serverEditButton
                text: "Edit"
                onClicked: {
                    serverChosen(appSettings.serverSettings.getItem(index), false)
                }
                width: 200
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
                anchors.top: appWindow.inPortrait ? serverInfoLabel.bottom : serverTitleLabel.bottom
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
            serverChosen(appSettings.newServerSettings(), true)
        }
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
