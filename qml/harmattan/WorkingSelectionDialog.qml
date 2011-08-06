/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.meego 1.0

CommonDialog {
    id: selectorDialog
    property int selectedIndex: -1
    property alias model: selectorListView.model
    property bool searchFieldVisible: true

    content: Column {
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 10

        TextField {
            id: searchField
            placeholderText: "Search"
            width: parent.width
            visible: selectorDialog.searchFieldVisible
        }

        ListView {
            id: selectorListView
            height: 290
            width: parent.width
            interactive: true

            delegate: Component {
                id: defaultDelegate

                Item {
                    id: delegateItem
                    property bool selected: index == selectorDialog.selectedIndex;
                    property string displayableText: model.modelData !== undefined ? model.modelData : (model.display !== undefined ? model.display : (model.edit !== undefined ? model.edit : model.name))

                    height: selectorDialog.platformStyle.itemHeight
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: searchField.text.length === 0 || displayableText.toLowerCase().indexOf(searchField.text.toLowerCase()) > -1

                    MouseArea {
                        id: delegateMouseArea
                        anchors.fill: parent;
                        onPressed: selectorDialog.selectedIndex = index;
                        onClicked: selectorDialog.accept();
                    }
                    Rectangle {
                        id: backgroundRect
                        anchors.fill: parent
                        color: delegateItem.selected ? selectorDialog.platformStyle.itemSelectedBackgroundColor : selectorDialog.platformStyle.itemBackgroundColor
                    }
                    Text {
                        id: itemText
                        elide: Text.ElideRight
                        color: delegateItem.selected ? selectorDialog.platformStyle.itemSelectedTextColor : selectorDialog.platformStyle.itemTextColor
                        anchors.verticalCenter: delegateItem.verticalCenter
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: selectorDialog.platformStyle.itemLeftMargin
                        anchors.rightMargin: selectorDialog.platformStyle.itemRightMargin
                        text: delegateItem.displayableText
                        font: selectorDialog.platformStyle.itemFont
                    }
                }
            }
        }
    }
}

