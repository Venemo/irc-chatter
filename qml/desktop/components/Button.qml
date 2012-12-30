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

    property real padding: 5
    property bool textCenter: true
    property alias text: theText.text
    property alias textColor: theText.color
    property alias font: theText.font
    property alias useGradientOverlay: gradientOverlay.visible
    property alias interactive: mouseArea.enabled

    signal clicked
    signal pressed
    signal released

    implicitHeight: theText.height + padding * 2
    implicitWidth: theText.width * 1.2 + padding * 2
    color: "#ddd"
    radius: 0

    Rectangle {
        id: gradientOverlay
        anchors.fill: parent
        radius: parent.radius
        gradient: Gradient {
            GradientStop { id: lighterStop; position: 0.66; color: "#00ffffff" }
            GradientStop { id: darkerStop; position: 1.0; color: "#33222222" }
        }
    }

    ParallelAnimation {
        id: pressedAnimation

        ColorAnimation {
            duration: 50
            target: lighterStop
            property: "color"
            from: lighterStop.color
            to: "#22ffffff"
        }
        ColorAnimation {
            duration: 50
            target: darkerStop
            property: "color"
            from: darkerStop.color
            to: "#00ffffff"
        }
    }

    ParallelAnimation {
        id: releasedAnimation

        ColorAnimation {
            duration: 50
            target: lighterStop
            property: "color"
            from: lighterStop.color
            to: "#00ffffff"
        }
        ColorAnimation {
            duration: 50
            target: darkerStop
            property: "color"
            from: darkerStop.color
            to: "#33222222"
        }
    }

    Text {
        id: theText
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: textCenter ? parent.horizontalCenter : undefined
            left: textCenter ? undefined : parent.left
            leftMargin: button.padding * 2
        }
        color: "#000"
        height: font.pixelSize * 1.5
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.ArrowCursor
        onClicked: {
            button.clicked();
        }
        onPressed: {
            releasedAnimation.stop();
            pressedAnimation.start();
            button.pressed();
        }
        onReleased: {
            pressedAnimation.stop();
            releasedAnimation.start();
            button.released();
        }
    }
}
