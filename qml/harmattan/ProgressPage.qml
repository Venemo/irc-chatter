import QtQuick 1.1
import com.meego 1.0
import com.meego.extras 1.0

Page {
    id: progressPage
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: {
                appWindow.pageStack.pop();
                cancelledBanner.show();
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status == DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
        }
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
    InfoBanner {
        id: cancelledBanner
        parent: firstrunPage
        text: "Connection cancelled!"
    }
    InfoBanner {
        id: connectedBanner
        parent: chatPage
        text: "Connection established successfully!"
    }
    Connections {
        target: ircModel
        onReadyToDisplay: {
            appWindow.pageStack.push(chatPage);
            connectedBanner.show();
        }
    }
}
