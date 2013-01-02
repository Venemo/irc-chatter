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
    id: theSwitch

    property real padding: 5
    property bool value: false

    implicitHeight: 40
    implicitWidth: 80

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#555" }
        GradientStop { position: 0.4; color: "#ddd" }
        GradientStop { position: 0.8; color: "#fff" }
    }
    border.color: button.color
    border.width: 0
    Keys.onSpacePressed: toggleSwitch();
    Keys.onEnterPressed: toggleSwitch();
    Keys.onReturnPressed: toggleSwitch();
    Keys.onLeftPressed: if (value) toggleSwitch();
    Keys.onRightPressed: if (!value) toggleSwitch();

    function toggleSwitch() {
        value = !value;
        if (value) {
            switchOffAnimation.stop();
            switchOnAnimation.start();
        }
        else {
            switchOnAnimation.stop();
            switchOffAnimation.start();
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            theSwitch.forceActiveFocus();
            toggleSwitch();
        }
    }

    Text {
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: padding
        }
        text: "ON"
        visible: value
    }

    Text {
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
            rightMargin: padding
        }
        text: "OFF"
        visible: !value
    }

    Button {
        id: button
        width: parent.width * 0.4
        anchors {
            top: parent.top
            bottom: parent.bottom
            margins: padding
        }
        x: padding
        color: value ? "#9fce00" : "#ff0000"

        Component.onCompleted: {
            button.x = value ? button.parent.width - button.width - padding : padding
        }

        onClicked: {
            toggleSwitch();
            theSwitch.forceActiveFocus();
        }

        ParallelAnimation {
            id: switchOnAnimation

            NumberAnimation {
                duration: 50
                target: button
                property: "x"
                from: button.x
                to: button.parent.width - button.width - padding
            }
            ColorAnimation {
                duration: 50
                target: button
                property: "color"
                from: button.color
                to: "#9fce00"
            }
        }

        ParallelAnimation {
            id: switchOffAnimation

            NumberAnimation {
                duration: 50
                target: button
                property: "x"
                from: button.x
                to: 5
            }
            ColorAnimation {
                duration: 50
                target: button
                property: "color"
                from: button.color
                to: "#ff0000"
            }
        }
    }

}
