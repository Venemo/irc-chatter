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
import "../pages"
import "../sheets"
import "../components"

Page {
    function switchToChatPageIfPossible() {
        if (status === PageStatus.Active && isModelReady) {
            commonMenu.close();
            appWindow.pageStack.push(chatPage);
        }
    }

    property bool isModelReady: false

    id: progressPage
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status === DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
            anchors.right: parent.right
        }
    }
    onStatusChanged: {
        switchToChatPageIfPossible();
    }

    BusyIndicator {
        id: connectingIndicator
        anchors.centerIn: parent
        running: appWindow.pageStack.currentPage == progressPage
        platformStyle: BusyIndicatorStyle { size: "large" }
    }
    Label {
        text: ircModel.isOnline ? "Connecting, please wait..." : "Please establish an internet connection"
        anchors.top: connectingIndicator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
    }
    Connections {
        target: ircModel
        onReadyToDisplay: {
            isModelReady = true;
            switchToChatPageIfPossible();
        }
    }
}
