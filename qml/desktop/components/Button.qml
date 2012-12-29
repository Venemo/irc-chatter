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

Rectangle {
    id: button

    property real padding: 10
    property bool textCenter: true
    property alias text: theText.text
    property alias textColor: theText.color
    property alias font: theText.font

    implicitHeight: theText.height + padding * 2
    implicitWidth: theText.width + padding * 2
    color: "#444"
    radius: 10

    Text {
        id: theText
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: textCenter ? parent.horizontalCenter : undefined
            left: textCenter ? undefined : parent.left
            leftMargin: button.padding
        }
        color: "#eee"
    }
}
