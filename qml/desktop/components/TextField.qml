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

    property bool enableTextValidation: false
    property real padding: 10
    property real rightPadding: padding
    property real leftPadding: padding
    property alias text: textInput.text
    property alias tooltipText: toolTipTextItem.text
    property alias inputMethodHints: textInput.inputMethodHints
    property alias validator: textInput.validator
    property alias acceptableInput: textInput.acceptableInput
    property alias passwordCharacter: textInput.passwordCharacter
    property alias echoMode: textInput.echoMode
    property alias enabled: textInput.enabled

    function forceActiveFocus() {
        textInput.forceActiveFocus();
    }

    implicitHeight: textInput.height + padding * 2
    border.width: enableTextValidation ? 2 : 0
    border.color: enableTextValidation ? (acceptableInput ? "#9fce00" : "#ff0000") : "#444"
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#777" }
        GradientStop { position: 0.4; color: "#ddd" }
        GradientStop { position: 1.0; color: "#fff" }
    }
    onActiveFocusChanged: {
        if (activeFocus) {
            textInput.forceActiveFocus();
        }
    }

    // Uncomment this to see the area of the TextInput
    //    Rectangle {
    //        color: "red"
    //        anchors.fill: textInput
    //    }
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        onClicked: {
            textInput.forceActiveFocus();
        }
    }

    TextInput {
        clip: true
        id: textInput
        height: font.pixelSize * 1.5
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
            leftMargin: leftPadding
            rightMargin: rightPadding
        }
        verticalAlignment: TextInput.AlignVCenter
        selectByMouse: true
        inputMethodHints: Qt.ImhDigitsOnly
        validator: RegExpValidator {
            regExp: enableTextValidation ? /..*/ : /.*/
        }
    }

    Bubble {
        id: tooltip
        visible: tooltipText.length > 0 && textInput.activeFocus
        anchors.left: textInput.right
        anchors.leftMargin: 17
        anchors.verticalCenter: parent.verticalCenter
        padding: 10
        showLeftTab: true

        Text {
            width: parent.width
            wrapMode: Text.Wrap
            id: toolTipTextItem
            text: ""
        }
    }
}
