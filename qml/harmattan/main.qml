import QtQuick 1.1
import com.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: chatPage

    ChatPage {
        id: chatPage
    }

    SettingsPage {
        id: settingsPage
    }

    QueryDialog {
        id: prereleaseDialog
        titleText: "This is not working"
        message: "This part of the GUI is currently 100% NON-functional.\nSorry guys."
        acceptButtonText: "Ok, no problem"
    }
}
