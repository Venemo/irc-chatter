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
    id: dialog

    property bool allowBackgroundClick: false
    default property alias inner: dialogContents.children
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text
    property alias title: titleText.text

    signal accepted
    signal rejected

    function open() {
        opacity = 1;
    }

    onAccepted: {
        opacity = 0;
    }
    onRejected: {
        opacity = 0;
    }

    anchors.fill: parent
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ff222222" }
        GradientStop { position: 0.4; color: "#ff444444" }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 300
            easing: Easing.InOutQuad
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (dialog.allowBackgroundClick) {
                dialog.rejected();
            }
        }
    }
    Text {
        id: titleText
        text: ""
        anchors.left: dialogContents.left
        anchors.bottom: dialogContents.top
        anchors.bottomMargin: 20
        color: "#fff"
        font.pointSize: 24
        visible: text.length > 0
    }
    Item {
        id: dialogContents
        anchors.centerIn: parent
        width: parent.width / 2

        Component.onCompleted: {
            if (dialogContents.children.length > 1) {
                console.log("warning: The Dialog component is designed to have only one child item!");
            }
            else if (dialogContents.children.length === 1) {
                dialogContents.height = dialogContents.children[0].height;
            }
        }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dialogContents.bottom
        anchors.topMargin: 20
        spacing: 5

        Button {
            id: acceptButton
            width: 130
            text: ""
            color: "#9fce00"
            textColor: "#000"
            visible: text.length > 0
            onClicked: {
                dialog.accepted();
            }
        }
        Button {
            id: rejectButton
            width: 130
            text: ""
            color: "#ff0000"
            textColor: "#fff"
            visible: text.length > 0
            onClicked: {
                dialog.rejected();
            }
        }
    }
}
