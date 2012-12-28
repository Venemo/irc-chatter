
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

#include "channelmodelcollection.h"
#include "model/channelmodel.h"

ChannelModelCollection::ChannelModelCollection()
{
}

ChannelModelCollection::~ChannelModelCollection()
{
}

ChannelModel *ChannelModelCollection::operator[](const QString &key)
{
    QString keyLower = key.toLower();

    if (_hash.contains(keyLower))
        return _hash[key.toLower()];

    return 0;
}

QList<ChannelModel*> ChannelModelCollection::values()
{
    return _hash.values();
}

void ChannelModelCollection::remove(const QString &key)
{
    _hash.remove(key.toLower());
}

bool ChannelModelCollection::contains(const QString &key)
{
    return _hash.contains(key.toLower());
}

void ChannelModelCollection::insert(const QString &key, ChannelModel *value)
{
    _hash.insert(key.toLower(), value);
}
