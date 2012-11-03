
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

#ifndef CHANNELMODELCOLLECTION_H
#define CHANNELMODELCOLLECTION_H

#include <QHash>
#include <QString>
#include <QList>

class ChannelModel;

class ChannelModelCollection
{
    QHash<QString, ChannelModel*> _hash;

public:
    ChannelModelCollection();
    ~ChannelModelCollection();

    ChannelModel *operator[](const QString &key);
    QList<ChannelModel*> values();
    void remove(const QString &key);
    bool contains(const QString &key);
    void insert(const QString &key, ChannelModel *value);

};

#endif // CHANNELMODELCOLLECTION_H
