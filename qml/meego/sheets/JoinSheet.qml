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
// Copyright (C) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import net.venemo.ircchatter 1.0
import "../pages"
import "../sheets"
import "../components"

Sheet {
    id: joinSheet
    acceptButtonText: isChannel ? "Join" : (isQuery ? "Query" : "")
    rejectButtonText: "Cancel"

    property string joinText
    property bool autoJoin: false
    property bool isQuery: joinField.text.length > 0 && joinField.text.charAt(0) !== '#'
    property bool isChannel: joinField.text.length > 1 && joinField.text.charAt(0) === '#'

    onStatusChanged: {
        if (status === DialogStatus.Opening)
            joinField.forceActiveFocus();
    }

    content: Column {
        anchors.centerIn: parent
        spacing: 10
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Channel name to join or user name to query"
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "(Begin channel names with #)"
        }
        TextField {
            id: joinField
            width: parent.width
            placeholderText: "Channel or user name..."
            text: "#"
            onTextChanged: {
                joinText = text
                channelCheckBox.checked = text.charAt(0) === '#'
            }
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            Keys.onReturnPressed: {
                if (isChannel || isQuery)
                    accept();
            }
        }
        CheckBox {
            id: channelCheckBox
            text: "Channel"
            checked: true
            onCheckedChanged: {
                if (checked) {
                    if (joinField.text.charAt(0) !== '#') {
                        joinField.text = "#" + joinField.text
                    }
                }
                else {
                    if (joinField.text.charAt(0) === '#') {
                        joinField.text = joinField.text.substring(1)
                    }
                }
            }
            anchors.horizontalCenter: parent.horizontalCenter
        }
// TODO
//        CheckBox {
//            anchors.horizontalCenter: parent.horizontalCenter
//            text: "Join on connect"
//            visible: isChannel
//            onCheckedChanged: autoJoin = checked
//        }

    }
}
