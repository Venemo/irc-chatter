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
    id: page
    anchors.fill: parent
    color: "#444"

    function animateOut() {
        outAnimation.start();
    }
    function animateIn() {
        inAnimation.start();
    }

    transform: [ Translate {
            id: translateTransform
        }, Scale {
            id: scaleTransform
            origin {
                x: page.width / 2
                y: page.height / 2
            }
        }]

    ParallelAnimation {
        id: outAnimation

        NumberAnimation {
            target: translateTransform
            property: "x"
            duration: 300
            from: 0
            to: -page.width * 3 / 2;
            easing.type: Easing.InOutQuad

        }
        NumberAnimation {
            target: scaleTransform
            properties: "yScale, xScale"
            duration: 300
            from: 1.0
            to: 0.8
            easing.type: Easing.InOutQuad
        }
    }

    ParallelAnimation {
        id: inAnimation

        NumberAnimation {
            target: translateTransform
            property: "x"
            duration: 300
            from: page.width * 3 / 2;
            to: 0
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: scaleTransform
            properties: "yScale, xScale"
            duration: 300
            from: 0.8
            to: 1.0
            easing.type: Easing.InOutQuad
        }
    }
}
