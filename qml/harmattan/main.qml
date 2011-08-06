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

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "About"
            }
            MenuItem {
                text: "Donate"
            }
            MenuItem {
                text: "Connect to..."
            }
            MenuItem {
                text: "Change nick..."
            }
        }
    }

    JoinSheet {
        id: joinSheet
        onAccepted: console.log("haha");
        visualParent: chatPage
    }

}
