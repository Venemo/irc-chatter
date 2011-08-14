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
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
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
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Label {
                text: "Autojoin Channels"
            }
            TextField {
                id:autojoinField
                width: parent.width
                text: "#harmattan, #irc-chatter"
                placeholderText: "Enter channels to autojoin"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
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
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Label {
                text: "Your ident"
            }
            TextField {
                id: identField
                width: parent.width
                text: "irc-chatter"
                placeholderText: "Enter your ident"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Label {
                text: "Your Real Name"
            }
            TextField {
                id: realNameField
                width: parent.width
                text: ""
                placeholderText: "If you wish, enter your real name"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
        }
    }

    InfoBanner {
        id: invalidBanner
        text: "The data you entered is invalid. Please fix it and press the Done button again."
    }
}
