import QtQuick 1.1
import com.meego 1.0

Row {
    id: titleLabel
    spacing: 10
    width: parent.width
    anchors.bottomMargin: 10

    property string text: "Your title here"

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        height: 1
        color: "#000000"
        width: (parent.width - title1.width) / 2 - 10
    }
    Label {
        id: title1
        text: titleLabel.text
        font.bold: true
    }
    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        height: 1
        color: "#000000"
        width: (parent.width - title1.width) / 2 - 10
    }
}
