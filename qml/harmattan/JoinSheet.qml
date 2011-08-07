import QtQuick 1.1
import com.meego 1.0

Sheet {
    id: joinSheet
    acceptButtonText: isChannel ? "Join" : (isQuery ? "Query" : "")
    rejectButtonText: "Cancel"

    property string joinText
    property bool autoJoin: false
    property bool isQuery: joinField.text.length > 0 && joinField.text.charAt(0) !== '#'
    property bool isChannel: joinField.text.length > 1 && joinField.text.charAt(0) === '#'

    content: Column {
        anchors.centerIn: parent
        spacing: 10
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Channel name to join or user name to query"
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "(Begin channel names with #)"
        }
        TextField {
            width: parent.width
            id: joinField
            placeholderText: "Channel or user name..."
            onTextChanged: joinText = text
        }
        CheckBox {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Join on connect"
            visible: isChannel
            onCheckedChanged: autoJoin = checked
        }

    }
}
