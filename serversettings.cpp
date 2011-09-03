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

#include <QtCore>

#include "serversettings.h"

ServerSettings::ServerSettings(QObject *parent, const QString &url, const quint16 &port, bool ssl, const QString &password, const QStringList &autoJoinChannels) :
    QObject(parent),
    _serverUrl(url),
    _serverPort(port),
    _serverSSL(ssl),
    _serverPassword(password),
    _autoJoinChannels(autoJoinChannels)
{
    if (!_autoJoinChannels.length() && url == "irc.freenode.net")
    {
        _autoJoinChannels.append("#irc-chatter");
        _autoJoinChannels.append("#harmattan");
        _autoJoinChannels.append("#maemo");
        _autoJoinChannels.append("#meego");
    }
}

QString ServerSettings::autoJoinChannelsInPlainString() const
{
    return _autoJoinChannels.join(", ");
}

void ServerSettings::setAutoJoinChannelsInPlainString(const QString &value)
{
    _autoJoinChannels = value.split(",");
    for (int i = 0; i < _autoJoinChannels.count(); i++)
        _autoJoinChannels[i] = _autoJoinChannels[i].trimmed();
}

QDataStream &operator<<(QDataStream &stream, const ServerSettings &server)
{
    return stream << server.serverUrl() << server.serverPassword() << server.autoJoinChannels() << server.serverSSL() << server.serverPort();
}

QDataStream &operator>>(QDataStream &stream, ServerSettings &server)
{
    return stream >> server._serverUrl >> server._serverPassword >> server._autoJoinChannels >> server._serverSSL >> server._serverPort;
}
