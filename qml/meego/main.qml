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
import net.venemo.ircchatter 1.0
import "pages"
import "sheets"
import "components"

PageStackWindow {
    id: appWindow
    initialPage: firstrunPage
    onOrientationChangeFinished: {
        showStatusBar = inPortrait

        if (pageStack.currentPage === chatPage)
            chatPage.adjustForOrientationChange()
    }
    Component.onCompleted: {
        showStatusBar = inPortrait
    }

    // Pages

    FirstrunPage {
        id: firstrunPage
    }

    ProgressPage {
        id: progressPage
    }

    ChatPage {
        id: chatPage
    }

    SettingsPage {
        id: settingsPage
    }

    // Common components

    QueryDialog {
        id: aboutDialog
        titleText: "About IRC Chatter"
        message: "Brought to you by Timur Kristóf and Hiemanshu Sharma.\nCurrent version: " + appVersion + "\n\nIRC Chatter is the fist IRC client for MeeGo. It uses Qt, QML, and libcommuni."
        acceptButtonText: "Close"
    }

    QueryDialog {
        id: quitDialog
        titleText: "Are you sure?"
        message: "Do you want to quit the app?\nThis will disconnect you from IRC."
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: Qt.quit()
    }

    Menu {
        id: commonMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "About"
                onClicked: aboutDialog.open()
            }
            MenuItem {
                text: "Settings"
                onClicked: appWindow.pageStack.push(settingsPage)
            }
            MenuItem {
                text: "Quit app"
                onClicked: quitDialog.open()
            }
        }
    }
}
