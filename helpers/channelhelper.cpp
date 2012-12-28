
// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program  is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2012, Timur Kristóf <venemo@fedoraproject.org>
//
// Authors of code borrowed from Konversation (colorForNick method):
// Copyright (C) 2006 Michael Kreitzer <mrgrim@gr1m.org>

#include <QtCore>
#include "channelhelper.h"

QList<QString> *_colors = 0;
QString _ownNickColor("#000000");

// This code is copypasted from Konversation (and modified by Timur Kristóf) - I hereby thank its authors
const QString &ChannelHelper::colorForNick(const QString &nick, const QString &ownNick)
{
    if (!_colors)
    {
        _colors = new QList<QString>();
        // Red
        _colors->append("#ff0000");
        // Green
        _colors->append("#00ff00");
        // Blue
        _colors->append("#0000ff");
        // Dark purple
        _colors->append("#400758");
        // Dark green
        _colors->append("#0c5807");
        // Dark orange
        _colors->append("#ba770e");
        // Yellowish green (my favourite colour)
        _colors->append("#73ba0e");
        // Yellow
        _colors->append("#cec700");
    }

    if (nick == ownNick)
        return _ownNickColor;

    int nickvalue = 0;

    for (int index = 0; index < nick.length(); index++)
        nickvalue += nick[index].unicode();

    return (*_colors)[nickvalue % _colors->count()];
}
