import QtQuick 1.1
import com.meego 1.0
import com.meego.extras 1.0
import net.venemo.ircchatter 1.0

Page {
    id: firstrunPage

    property bool isValid: serverUrlField.text.length > 0 && nicknameField.text.length > 0
    property ServerSettings server: appSettings.serverSettings.getItem(0)

    tools: ToolBarLayout {
        id: commonToolbar

        ToolIcon {
            platformIconId: "toolbar-done"
            onClicked: {
                if (isValid) {
                    appWindow.pageStack.push(progressPage);
                    appSettings.saveServerSettings();
                    ircModel.connectToServer(server, appSettings);
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
                text: server.serverUrl
                placeholderText: "Enter a server URL"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "serverUrl"
                value: serverUrlField.text
            }
            Label {
                text: "Server Port"
            }
            TextField {
                id: serverPortField
                width: parent.width
                text: server.serverPort
                placeholderText: "Enter the server port"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "serverPort"
                value: serverPortField.text
            }
            Label {
                text: "Password"
            }
            TextField {
                id: passwordField
                width: parent.width
                text: server.serverPassword
                placeholderText: "If it's needed, enter a password"
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "serverPassword"
                value: passwordField.text
            }
            Label {
                text: "Autojoin Channels"
            }
            TextField {
                id:autojoinField
                width: parent.width
                text: server.autoJoinChannelsInPlainString
                placeholderText: "Enter channels to autojoin"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: server
                property: "autoJoinChannelsInPlainString"
                value: autojoinField.text
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
                text: appSettings.userNickname
                placeholderText: "Enter your nickname"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: appSettings
                property: "userNickname"
                value: nicknameField.text
            }
            Label {
                text: "Your ident"
            }
            TextField {
                id: identField
                width: parent.width
                text: appSettings.userIdent
                placeholderText: "Enter your ident"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: appSettings
                property: "userIdent"
                value: identField.text
            }
            Label {
                text: "Your Real Name"
            }
            TextField {
                id: realNameField
                width: parent.width
                text: appSettings.userRealName
                placeholderText: "If you wish, enter your real name"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            }
            Binding {
                target: appSettings
                property: "userRealName"
                value: realNameField.text
            }
        }
    }

    InfoBanner {
        id: invalidBanner
        text: "The data you entered is invalid. Please fix it and press the Done button again."
    }
}
