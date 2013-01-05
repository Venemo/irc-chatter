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
// Copyright (C) 2012, Timur Kristóf <venemo@fedoraproject.org>

import QtQuick 2.0
import "../components"

Dialog {
    id: queryDialog

    property alias text: theText.text
    property alias textFormat: theText.textFormat

    title: "Are you sure?"
    acceptButtonText: "Yes"
    rejectButtonText: "No"
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#cc222222" }
        GradientStop { position: 0.4; color: "#dd444444" }
    }

    Text {
        id: theText
        width: parent.width * 0.5
        wrapMode: Text.Wrap
        color: "#fff"
        onLinkActivated: {
            if (link == "back://back") {
                queryDialog.close();
            }
            else {
                Qt.openUrlExternally(link);
            }
        }
    }
}
