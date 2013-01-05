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
    id: aboutDialog
    title: "About IRC Chatter"

    Text {
        color: "#fff"
        textFormat: Text.RichText
        wrapMode: Text.Wrap
        width: parent.width
        onLinkActivated: {
            if (link == "back://back") {
                aboutDialog.close();
            }
            else {
                Qt.openUrlExternally(link);
            }
        }
        text: "IRC Chatter is a simple and easy-to-use IRC client focused on ease of use.
It is created by Timur Kristóf and licensed to you under the terms of the GPL (version 2).
Thank you very much for using IRC Chatter.
If you wish to know what's going on, <a style='color: #9fce00;' href='http://twitter.com/Venemo'>follow Timur on Twitter</a>.
All kind of feedback is welcome.<br />
<br />
<a style='color: #9fce00;' href='back://back'>&laquo; Back to app</a><br />
<br />
<b>Contributors</b><br />
Hiemanshu Sharma<br />
László Papp<br />
<br />
<b>Special thanks to</b><br />
Gary Birkett<br />
Robin Burchell<br />
J-P Nurmi<br />
Eike Hein<br />
Michael Kreitzer<br />
<br />
<b>Author of the icon</b><br />
Johannes Siipola<br />
"
    }
}
