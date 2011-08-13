import QtQuick 1.1
import com.meego 1.0
import com.meego.extras 1.0

Page {
    id: progressPage
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status == DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
            anchors.right: parent.right
        }
    }

    property bool isModelReady: false
    property bool canPushChatPage: status == PageStatus.Active && isModelReady

    onCanPushChatPageChanged: {
        if (canPushChatPage)
            appWindow.pageStack.push(chatPage);
    }

    BusyIndicator {
        id: connectingIndicator
        anchors.centerIn: parent
        running: appWindow.pageStack.currentPage == progressPage
        platformStyle: BusyIndicatorStyle { size: "large" }
    }
    Label {
        text: "Connecting, please wait..."
        anchors.top: connectingIndicator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
    }
    Connections {
        target: ircModel
        onReadyToDisplay: {
            isModelReady = true;
        }
    }
}
