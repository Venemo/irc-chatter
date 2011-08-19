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

#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QObject>

#include "util.h"
#include "qobjectlistmodel.h"
#include "channelmodel.h"

namespace Irc { class Session; }
class IrcModel;

class ServerModel : public QObject
{
    Q_OBJECT

    GENPROPERTY_R(QObjectListModel<ChannelModel>*, _channels, channels)
    Q_PROPERTY(QObject* channels READ channels NOTIFY channelsChanged)
    GENPROPERTY_R(QString, _url, url)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged)

    QString _password;
    Irc::Session *_backend;

    friend class IrcModel;

    void removeModelForBuffer(Irc::Buffer *buffer);

protected:
    explicit ServerModel(IrcModel *parent, const QString &url, Irc::Session *backend);

public:
    ~ServerModel();
    Q_INVOKABLE bool joinChannel(const QString &channelName);
    Q_INVOKABLE bool partChannel(const QString &channelName);
    Q_INVOKABLE bool queryUser(const QString &userName);
    Q_INVOKABLE bool closeUser(const QString &userName);
    Q_INVOKABLE bool changeNick(const QString &nick);
    Q_INVOKABLE void displayError(const QString &error);
    Q_INVOKABLE bool msgUser(const QString &userName, const QString &msg);
    Q_INVOKABLE bool kickUser(const QString &user, const QString &channel, const QString &message);

signals:
    void channelsChanged();
    void urlChanged();

private slots:
    void backendConnectedToServer();
    void backendAddedBuffer(Irc::Buffer *buffer);
    void backendRemovedBuffer(Irc::Buffer *buffer);
    void receiveNumericMessageFromBackend(const QString &name, uint x, const QStringList &message);
    void backendDisconnectedFromServer();

};

#endif // SERVERMODEL_H
