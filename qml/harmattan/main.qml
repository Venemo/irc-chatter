import QtQuick 1.1
import com.meego 1.0

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

    Menu {
        id: commonMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Quit app"
                onClicked: Qt.quit();
            }
        }
    }
}
