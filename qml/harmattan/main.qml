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
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: firstrunPage

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
        id: prereleaseDialog
        titleText: "This is not working"
        message: "This part of the GUI is currently 100% NON-functional.\nSorry guys."
        acceptButtonText: "Ok, no problem"
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
                text: "Quit app"
                onClicked: quitDialog.open()
            }
        }
    }
}
