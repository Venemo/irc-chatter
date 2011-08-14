import QtQuick 1.1
import com.meego 1.0
import com.meego.extras 1.0

Page {
    id: firstrunPage

    property bool isValid: serverUrlField.text.length > 0 && nicknameField.text.length > 0

    tools: ToolBarLayout {
            id: commonToolbar

            ToolIcon {
                platformIconId: "toolbar-done"
                onClicked: {
                    if (isValid) {
                        appWindow.pageStack.push(progressPage);
                        ircModel.connectToServer(serverUrlField.text, passwordField.text, nicknameField.text, identField.text, realNameField.text, autojoinField.text);
                    }
                    else {
                        invalidBanner.show();
                    }
                }

            }
            ToolIcon {
                platformIconId: "toolbar-view-menu"
                onClicked: (commonMenu.status == DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
            }
        }

    Flickable {
        id: configFlickable
        anchors.fill: parent

        interactive: true
        contentWidth: parent.width
        contentHeight: configColumn.height + 30
        clip: true

        Column {
            id: configColumn
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            spacing: 10

            TitleLabel {
                text: "Welcome to IRC Chatter!"
            }
            Label {
                text: "This version is a v0.1 PREVIEW and it only allows you to connect to one server. This will be fixed in the final release."
                width: parent.width
            }
            TitleLabel {
                text: "Server settings"
            }
            Label {
                text: "Server URL"
            }
            TextField {
                id: serverUrlField
                width: parent.width
                text: "irc.freenode.net"
                placeholderText: "Enter a server URL"
            }
            Label {
                text: "Password"
            }
            TextField {
                id: passwordField
                width: parent.width
                text: ""
                placeholderText: "If it's needed, enter a password"
                echoMode: TextInput.Password
            }
            Label {
                text: "Autojoin Channels"
            }
            TextField {
                id:autojoinField
                width: parent.width
                text: "#harmattan, #irc-chatter"
                placeholderText: "Enter channels to autojoin"
            }
            TitleLabel {
                text: "User settings"
            }
            Label {
                text: "Your nickname"
            }
            TextField {
                id: nicknameField
                width: parent.width
                text: ""
                placeholderText: "Enter your nickname"
            }
            Label {
                text: "Your ident"
            }
            TextField {
                id: identField
                width: parent.width
                text: "irc-chatter"
                placeholderText: "Enter your ident"
            }
            Label {
                text: "Your Real Name"
            }
            TextField {
                id: realNameField
                width: parent.width
                text: ""
                placeholderText: "Enter your real name"
            }
        }
    }

    InfoBanner {
        id: invalidBanner
        text: "The data you entered is invalid. Please fix it and press the Done button again."
    }
}
