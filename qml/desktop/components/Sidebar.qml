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
    id: sidebar
    color: "#f9a300"

    Rectangle {
        width: sidebar.height
        height: sidebar.width
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#33222222" }
            GradientStop { position: 0.7; color: "#33ffffff" }
        }
        anchors {
            top: sidebar.top
            left: sidebar.left
        }
        transform: [
            Rotation {
                angle: -90
            },
            Translate {
                y: sidebar.height
            }
        ]
    }
}
