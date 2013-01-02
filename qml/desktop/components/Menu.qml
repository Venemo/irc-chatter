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

Bubble {
    id: menu
    showRightTab: false
    color: "#222"
    spacing: 5
    padding: 10
    border.color: "#888"
    state: "closed"
    states: [
        State {
            name: "opened"
            when: menu.visible

            PropertyChanges {
                target: menu
                opacity: 1
                visible: true
            }
        },
        State {
            name: "closed"

            PropertyChanges {
                target: menu
                opacity: 0
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "closed"

            SequentialAnimation {
                NumberAnimation {
                    target: menu
                    duration: 150
                    property: "opacity"
                    from: 1
                    to: 0
                }
                PropertyAction {
                    target: menu
                    property: "visible"
                    value: false
                }
            }
        },
        Transition {
            from: "*"
            to: "opened"

            SequentialAnimation {
                PropertyAction {
                    target: menu
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: menu
                    duration: 150
                    property: "opacity"
                    from: 0
                    to: 1
                }
            }
        }
    ]

    function open() {
        state = "opened";
    }
    function close() {
        state = "closed";
    }
    function toggle() {
        if (state == "opened") {
            close();
        }
        else {
            open();
        }
    }
}
