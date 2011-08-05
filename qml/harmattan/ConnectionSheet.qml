import QtQuick 1.1
import com.meego 1.0

Sheet {
    id: connectionSheet
    acceptButtonText: "Connect"
    rejectButtonText: "Cancel"

    Column {
        spacing: 10

        Label {
            text: "Server address"
        }
        TextField {
        }
        Label {
            text: "Port"
        }
        TextField {
        }
        Label {
            text: "Use secure channel"
        }
        Switch {
            enabled: true
        }
        Label {
            text: "Nickname"
        }
        TextField {
        }
        Label {
            text: "Password"
        }
        TextField {
            echoMode: TextInput.Password
        }
    }
}
