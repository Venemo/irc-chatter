// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import net.venemo.ircchatter 1.0

Page {
    property string color: appSettings.sidebarColor

    id: settingsPage
    tools: ToolBarLayout {
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop()
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (commonMenu.status == DialogStatus.Closed) ? commonMenu.open() : commonMenu.close()
        }
    }

    Flickable {
        id: settingsFlickable
        anchors.fill: parent

        interactive: true
        contentWidth: parent.width
        contentHeight: settingsColumn.height + 30
        clip: true

        Column {
            id: settingsColumn
            spacing: 10
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            /*TitleLabel {
                text: "Display"
            }
            Label {
                text: "Misc events"
                width: parent.width
                height: miscEventsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: miscEventsSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            Label {
                text: "Timestamps"
                width: parent.width
                height: timestampsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: timestampsSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            Label {
                text: "Font size"
                width: parent.width
                height: fontSizeTumblerButton.height
                verticalAlignment: Text.AlignVCenter

                TumblerButton {
                    id: fontSizeTumblerButton
                    anchors.right: parent.right
                    text: "24"
                    width: 140

                    onClicked: fontSizeSelectionDialog.open()
                }
            }
            TitleLabel {
                text: "Send notifications for"
            }
            Label {
                text: "Messages in queries"
                width: parent.width
                height: messagesInQueriesSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: messagesInQueriesSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            Label {
                text: "Messages containing your nick"
                width: parent.width
                height: messagesInQueriesSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: messagesContainingYourNickSwitch
                    anchors.right: parent.right
                    checked: true
                }
            }
            TitleLabel {
                text: "Configuration"
            }
            Button {
                width: parent.width - 100
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Configure servers"
            }*/
            TitleLabel {
                text: "User Configuration"
            }
            Label {
                text: "Quit Message"
            }
            TextField {
                id: quitField
                width: parent.width
                text: appSettings.quitMessage
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            Binding {
                target: appSettings
                property: "quitMessage"
                value: quitField.text
            }
            Label {
                text: "Part Message"
            }
            TextField {
                id: partField
                width: parent.width
                text: appSettings.partMessage
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            Binding {
                target: appSettings
                property: "partMessage"
                value: partField.text
            }
            Label {
                text: "Kick Message"
            }
            TextField {
                id: kickField
                width: parent.width
                text: appSettings.kickMessage
                inputMethodHints: Qt.ImhNoPredictiveText
            }
            Binding {
                target: appSettings
                property: "kickMessage"
                value: kickField.text
            }
            CheckBox {
                id: autoFocusCheckBox
                text: "Auto focus text field after switching"
                checked: appSettings.autoFocusTextField
            }
            Binding {
                target: appSettings
                property: "autoFocusTextField"
                value: autoFocusCheckBox.checked
            }
            Label {
                text: "Font Size"
            }
            Row {
                spacing: 10
                width: parent.width
                Slider {
                    id: fontSlider
                    minimumValue: 12
                    maximumValue: 40
                    stepSize: 1
                    width: parent.width - 60
                    valueIndicatorVisible: true
                    value: appSettings.fontSize
                }
                Binding {
                    target: appSettings
                    property: "fontSize"
                    value: fontSlider.value
                }
                Label {
                    text: fontSlider.value
                    width: 100
                }
            }
            Row {
                spacing: 10
                width: parent.width
                Label {
                    id: monospaceField
                    text: "Use Monospace Font?"
                }
                CheckBox {
                    id: monospaceCheckbox
                    checked: false
                    anchors.right: parent.right
                    anchors.verticalCenter: monospaceField.verticalCenter
                }
            }
            Binding {
                target: appSettings
                property: "fontMonospace"
                value: monospaceCheckbox.checked
            }
            Row {
                width: parent.width
                spacing: 10
                Label {
                    id: sidebarLabel
                    text: "Sidebar Color"
                }
                Button {
                    id: colorButton
                    text: "Orange"
                    width: parent.width / 2
                    anchors.right: parent.right
                    anchors.verticalCenter: sidebarLabel.verticalCenter
                    onClicked: colorSelectionDialog.open()
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: settingsFlickable
    }
    WorkingSelectionDialog {
        id: colorSelectionDialog
        titleText: "Sidebar Color"
        searchFieldVisible: false
        model: ListModel {
            ListElement {
                name: "Orange (Default)"
            }
            ListElement {
                name: "Green"
            }
            ListElement {
                name: "Red"
            }
            ListElement {
                name: "Blue"
            }
            ListElement {
                name: "Grey"
            }
            ListElement {
                name: "Purple"
            }
            ListElement {
                name: "Yellow"
            }
        }
        onAccepted: {
            if (colorSelectionDialog.selectedIndex == 0) {
                color = "#f9a300"
                colorButton.text = "Orange"
            }
            else if (colorSelectionDialog.selectedIndex == 1) {
                colorButton.text = "Green"
                color = "#73ba0e"
            }
            else if (colorSelectionDialog.selectedIndex == 2) {
                colorButton.text = "Red"
                color = "#ff0000"
            }
            else if (colorSelectionDialog.selectedIndex == 3) {
                colorButton.text = "Blue"
                color = "#0000ff"
            }
            else if (colorSelectionDialog.selectedIndex == 4) {
                colorButton.text = "Grey"
                color = "#999999"
            }
            else if (colorSelectionDialog.selectedIndex == 5) {
                colorButton.text = "Purple"
                color = "#400758"
            }
            else {
                colorButton.text = "Yellow"
                color = "#cec700"
            }
        }
    }
    Binding {
        target: appSettings
        property: "sidebarColor"
        value: color
    }
    WorkingSelectionDialog {
        id: fontSizeSelectionDialog
        model: ListModel { }
        titleText: "Font size"
        searchFieldVisible: false
        Component.onCompleted: {
            for (var i = 10; i <= 40; i++)
                model.append({ name: i + " pixels", pixelSize: i });
        }
    }
}
