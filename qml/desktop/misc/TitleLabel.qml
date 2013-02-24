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

Row {
    id: titleLabel
    spacing: 10
    width: parent.width
    anchors.bottomMargin: 10

    property alias text: title1.text
    property alias color: title1.color

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        height: 1
        color: titleLabel.color
        width: (parent.width - title1.width) - 10
    }
    Text {
        id: title1
        text: "Your title here"
        font.bold: true
    }
}
