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

Button {
    id: comboBox

    property variant value: 0
    property alias model: repeater.model

    textCenter: false
    onClicked: {
        if (menu.visible)
            menu.close();
        else
            menu.open();
    }

    Text {
        color: comboBox.textColor
        text: "<...>"
        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
    }

    Menu {
        id: menu
        showTopTab: true
        z: comboBox.z + 1
        visible: false
        anchors {
            top: comboBox.bottom
            left: comboBox.left
            right: comboBox.right
            topMargin: 10
        }
        Repeater {
            id: repeater
            model: ListModel {
                ListElement { text: "Example 1"; value: 0 }
                ListElement { text: "Example 2"; value: 1 }
            }
            delegate: MenuButton {
                text: model.text
                onClicked: {
                    comboBox.text = model.text;
                    comboBox.value = model.value;
                    menu.close();
                }
            }
        }
    }
}
