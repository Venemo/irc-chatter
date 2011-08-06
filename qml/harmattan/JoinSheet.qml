import QtQuick 1.1
import com.meego 1.0

Sheet {
    id: joinSheet
    acceptButtonText: (joinField.text.length === 0 || (joinField.text.charAt(0) === '#' && joinField.text.length < 2)) ? "" : (joinField.text.charAt(0) === '#' ? "Join" : "Query")
    rejectButtonText: "Cancel"

    property string channelName
    property bool autoJoin: false

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
            onTextChanged: channelName = text
        }
        CheckBox {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Join on connect"
            visible: joinField.text.length > 0 && joinField.text.charAt(0) === '#'
            onCheckedChanged: autoJoin = checked
        }

    }
}
