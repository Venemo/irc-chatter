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
    id: textField

    property real padding: 10
    property alias text: textInput.text

    implicitHeight: textInput.height + padding * 2
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#777" }
        GradientStop { position: 0.4; color: "#ddd" }
        GradientStop { position: 1.0; color: "#fff" }
    }

    // Uncomment this to see the area of the TextInput
    //    Rectangle {
    //        color: "red"
    //        anchors.fill: textInput
    //    }
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
    }

    TextInput {
        id: textInput
        height: font.pixelSize * 3 / 2
        anchors.centerIn: parent
        width: parent.width - textField.padding * 2
        verticalAlignment: TextInput.AlignVCenter
        selectByMouse: true
    }
}
