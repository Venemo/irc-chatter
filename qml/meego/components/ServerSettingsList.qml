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

Column {
    signal serverChosen(variant server, bool isNewServer)
    signal serverConnectionChanged(variant server, bool connectToServer)

    property bool bindConnectionBack: false

    id: serverSettingsList
    width: parent.width
    spacing: 10

    TitleLabel {
        text: "Configured servers"
    }
    Label {
        text: "No servers configured yet"
        visible: appSettings.serverSettings.itemCount === 0
    }
    Repeater {
        id: serverSettingsRepeater
        model: appSettings.serverSettings
        width: parent.width
        delegate: Rectangle {
            width: serverSettingsList.width
            height: serverTitleLabel.height
            color: "transparent"

            Switch {
                id: connectSwitch
                checked: shouldConnect
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
                    if (!bindConnectionBack) {
                        serverConnectionChanged(appSettings.serverSettings.getItem(index), checked)
                    }
                }

                Binding {
                    target: connectSwitch
                    property: "checked"
                    value: appSettings.serverSettings.getItem(index) !== null ? appSettings.serverSettings.getItem(index).shouldConnect : false
                    when: bindConnectionBack
                }
            }
            Label {
                id: serverTitleLabel
                text: "<b>" + serverUrl + ":" + serverPort + "</b><br/>" + userNickname
                height: implicitHeight + 20
                verticalAlignment: Text.AlignVCenter
                textFormat: Text.RichText
                anchors.left: connectSwitch.right
                anchors.leftMargin: 20
                anchors.right: parent.right

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        serverChosen(appSettings.serverSettings.getItem(index), false)
                    }
                }
                BusyIndicator {
                    id: serverConnectingIndicator
                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: moreIndicator.left
                    anchors.rightMargin: 20

                    Connections {
                        target: appSettings.serverSettings.getItem(index) !== null ? appSettings.serverSettings.getItem(index) : undefined
                        onIsConnectingChanged: {
                            serverConnectingIndicator.visible = serverConnectingIndicator.running = appSettings.serverSettings.getItem(index).isConnecting
                        }
                    }
                }
                MoreIndicator {
                    id: moreIndicator
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                }
            }
        }
    }
    TitleLabel {
        text: "New server"
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
