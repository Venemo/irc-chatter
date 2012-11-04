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
// Copyright (C) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import net.venemo.ircchatter 1.0
import "../pages"
import "../sheets"
import "../components"

// This is the settings page
// Allows the user to configure the app settings,
// but not the server settings.
Page {
    property string color: appSettings.sidebarColor

    id: settingsPage

    // Simple custom toolbar here
    tools: ToolBarLayout {
        visible: true

        // Back button
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop()
        }
    }

    // Flickable area for all the settings
    Flickable {
        id: settingsFlickable
        anchors.fill: parent

        interactive: true
        contentWidth: parent.width
        contentHeight: settingsColumn.height + 30
        clip: true

        // Layout for all the settings
        Column {
            id: settingsColumn
            spacing: 10
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            // ANATOMY OF A SETTING
            // --------------------
            //
            //Label { // Label which contains the setting
            //    text: "Setting text"
            //    width: parent.width
            //    height: timestampsSwitch.height // Height of the contained UI element
            //    verticalAlignment: Text.AlignVCenter
            //
            //    Switch { // UI element for manipulating a setting
            //        id: timestampsSwitch
            //        anchors.right: parent.right
            //        checked: appSettings.displayTimestamps // Binding against appSettings
            //
            //        Binding { // Back binding against appSettings
            //            target: appSettings
            //            property: "displayTimestamps"
            //            value: timestampsSwitch.checked
            //        }
            //    }
            //}

            // SETTING GROUP
            TitleLabel {
                text: "Appearance"
            }
            // SETTING: Whether or not to show misc events in the channel text areas
            Label {
                text: "Show join/part/quit/nick events"
                width: parent.width
                height: miscEventsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: miscEventsSwitch
                    anchors.right: parent.right
                    checked: appSettings.displayMiscEvents

                    Binding {
                        target: appSettings
                        property: "displayMiscEvents"
                        value: miscEventsSwitch.checked
                    }
                }
            }
            // SETTING: Show time stamps on messages
            Label {
                text: "Show timestamps"
                width: parent.width
                height: timestampsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: timestampsSwitch
                    anchors.right: parent.right
                    checked: appSettings.displayTimestamps

                    Binding {
                        target: appSettings
                        property: "displayTimestamps"
                        value: timestampsSwitch.checked
                    }
                }
            }
            // SETTING: Use monospace font for channel text area
            Label {
                text: "Use monospace font"
                width: parent.width
                height: timestampsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: monospaceCheckbox
                    anchors.right: parent.right
                    checked: false

                    Binding {
                        target: appSettings
                        property: "fontMonospace"
                        value: monospaceCheckbox.checked
                    }
                }
            }
            // SETTING: Autofocus the channel text field after switching channels
            Label {
                text: "Autofocus text field"
                width: parent.width
                height: timestampsSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: autoFocusCheckBox
                    anchors.right: parent.right
                    checked: appSettings.autoFocusTextField

                    Binding {
                        target: appSettings
                        property: "autoFocusTextField"
                        value: autoFocusCheckBox.checked
                    }
                }
            }
            // SETTING: Color of the sidebar in the chat window
            Label {
                text: "Sidebar color"
                width: parent.width
                height: colorButton.height
                verticalAlignment: Text.AlignVCenter

                TumblerButton {
                    id: colorButton
                    text: "Orange"
                    anchors.right: parent.right
                    onClicked: colorSelectionDialog.open()
                }
            }
            // SETTING: Font size in the chat text area
            Label {
                text: "Font size"
                width: parent.width
                height: fontSlider.height
                verticalAlignment: Text.AlignVCenter

                Slider {
                    id: fontSlider
                    minimumValue: 12
                    maximumValue: 40
                    stepSize: 1
                    width: 300
                    valueIndicatorVisible: true
                    value: appSettings.fontSize
                    anchors.right: fontSliderLabel.left

                    Binding {
                        target: appSettings
                        property: "fontSize"
                        value: fontSlider.value
                    }
                }
                Label {
                    id: fontSliderLabel
                    text: fontSlider.value
                    width: 50
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            // SETTING GROUP
            TitleLabel {
                text: "Send notifications for"
            }
            // SETTING: Send OS notifications for messages in queries
            Label {
                text: "Messages in queries"
                width: parent.width
                height: messagesInQueriesSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: messagesInQueriesSwitch
                    anchors.right: parent.right
                    checked: appSettings.notifyOnPrivmsg

                    Binding {
                        target: appSettings
                        property: "notifyOnPrivmsg"
                        value: messagesInQueriesSwitch.checked
                    }
                }
            }
            // SETTING: Send OS notifications for messages in channels which contain the user's nickname
            Label {
                text: "Messages containing your nick"
                width: parent.width
                height: messagesContainingYourNickSwitch.height
                verticalAlignment: Text.AlignVCenter

                Switch {
                    id: messagesContainingYourNickSwitch
                    anchors.right: parent.right
                    checked: appSettings.notifyOnNick

                    Binding {
                        target: appSettings
                        property: "notifyOnNick"
                        value: messagesContainingYourNickSwitch.checked
                    }
                }
            }
            // SETTING GROUP
            TitleLabel {
                text: "Customizations"
            }
            // SETTING: Quit message which is sent when the user disconnects
            Label {
                text: "Quit Message"
            }
            TextField {
                id: quitField
                width: parent.width
                text: appSettings.quitMessage
                inputMethodHints: Qt.ImhNoPredictiveText

                Binding {
                    target: appSettings
                    property: "quitMessage"
                    value: quitField.text
                }
            }
            // SETTING: Message which is sent when the user parts a channel
            Label {
                text: "Part Message"
            }
            TextField {
                id: partField
                width: parent.width
                text: appSettings.partMessage
                inputMethodHints: Qt.ImhNoPredictiveText

                Binding {
                    target: appSettings
                    property: "partMessage"
                    value: partField.text
                }
            }
            // SETTING: Message which is sent when the user kicks someone from a channel
            Label {
                text: "Kick Message"
            }
            TextField {
                id: kickField
                width: parent.width
                text: appSettings.kickMessage
                inputMethodHints: Qt.ImhNoPredictiveText

                Binding {
                    target: appSettings
                    property: "kickMessage"
                    value: kickField.text
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: settingsFlickable
    }
    // Selection dialog for selecting the sidebar color
    WorkingSelectionDialog {
        id: colorSelectionDialog
        titleText: "Sidebar Color"
        searchFieldVisible: false

        model: ListModel {

            // This function is what interprets the selection when the user selects a color
            function interpretColor() {
                var item = colorListModel.get(colorSelectionDialog.selectedIndex);
                color = item.color
                colorButton.text = item.name
            }

            id: colorListModel
            ListElement {
                name: "Orange (Default)"
                color: "#f9a300"
            }
            ListElement {
                name: "Green"
                color: "#9fce00"
            }
            ListElement {
                name: "Red"
                color: "#ff0000"
            }
            ListElement {
                name: "Blue"
                color: "#0000ff"
            }
            ListElement {
                name: "Grey"
                color: "#999999"
            }
            ListElement {
                name: "Purple"
                color: "#400758"
            }
            ListElement {
                name: "Yellow"
                color: "#cec700"
            }
        }
        onAccepted: colorListModel.interpretColor()
    }
    // This event handler will set the dialog and the tumbler button to the name of the selected color
    Component.onCompleted: {
        for (var i = 0; i < colorListModel.count; i++) {
            var item = colorListModel.get(i)
            if (item.color === appSettings.sidebarColor) {
                colorSelectionDialog.selectedIndex = i
                colorListModel.interpretColor();
                break;
            }
        }
    }
    Binding {
        target: appSettings
        property: "sidebarColor"
        value: color
    }
}
