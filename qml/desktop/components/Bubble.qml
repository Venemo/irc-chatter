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
    id: bubble

    default property alias inner: innerColumn.children
    property alias spacing: innerColumn.spacing
    property real padding: 10
    property real minHeight: 50
    property bool showRightTab: true
    property bool showLeftTab: false

    border.color: "#aaa"
    border.width: 2
    color: "#eee"
    radius: 0
    width: 250
    implicitHeight: Math.max(innerColumn.height + padding * 2, minHeight)

    // Right tab implementation
    Rectangle {
        visible: showRightTab
        anchors {
            left: parent.right
            leftMargin: -bubble.border.width
            verticalCenter: parent.verticalCenter
        }
        color: "transparent"
        clip: true
        width: 13 + bubble.border.width + 5
        height: parent.height

        Rectangle {
            id: rightTabRect
            width: 25
            height: width
            color: bubble.color
            border.color: bubble.border.color
            border.width: bubble.border.width
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            transform: [
                Rotation {
                    angle: 45
                    origin.x: rightTabRect.width / 2
                    origin.y: rightTabRect.width / 2
                },
                Translate {
                    x: - bubble.border.width - 10
                }
            ]
        }

    }
    // Left tab implementation
    Rectangle {
        visible: showLeftTab
        anchors {
            right: parent.left
            rightMargin: -bubble.border.width
            verticalCenter: parent.verticalCenter
        }
        color: "transparent"
        clip: true
        width: 13 + bubble.border.width + 5
        height: parent.height

        Rectangle {
            id: leftTabRect
            width: 25
            height: width
            color: bubble.color
            border.color: bubble.border.color
            border.width: bubble.border.width
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            transform: [
                Rotation {
                    angle: 45
                    origin.x: leftTabRect.width / 2
                    origin.y: leftTabRect.width / 2
                },
                Translate {
                    x: bubble.border.width + 5
                }

            ]
        }

    }

    Column {
        id: innerColumn
        anchors.centerIn: parent
        width: parent.width - padding * 2
        spacing: 10
    }
}
