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
#include <QHash>

#include "util.h"
#include "qobjectlistmodel.h"
#include "channelmodel.h"
#include "appsettings.h"

class IrcSession;
class IrcMessage;
class IrcNumericMessage;
class IrcModel;

class ServerModel : public QObject
{
    Q_OBJECT

    GENPROPERTY_R(QString, _url, url)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged)

    QHash<QString, ChannelModel*> _channels;
    IrcSession *_backend;
    AppSettings *_settings;
    ChannelModel *_defaultChannel;
    QStringList _autoJoinChannels;

    friend class IrcModel;
    friend class AppSettings;

    void addModelForChannel(const QString &channelName);
    void removeModelForChannel(const QString &channelName);
    void processNumericMessage(IrcNumericMessage *message);

protected:
    explicit ServerModel(IrcModel *parent, const QString &url, IrcSession *backend);
    ChannelModel *findOrCreateChannel(const QString &channelName);

public:
    ~ServerModel();
    Q_INVOKABLE void joinChannel(const QString &channelName);
    Q_INVOKABLE void partChannel(const QString &channelName);
    Q_INVOKABLE void queryUser(const QString &userName);
    Q_INVOKABLE void closeUser(const QString &userName);
    Q_INVOKABLE void changeNick(const QString &nick);
    Q_INVOKABLE void displayError(const QString &error);
    Q_INVOKABLE void msgUser(const QString &userName, const QString &msg);
    Q_INVOKABLE void kickUser(const QString &user, const QString &channel, const QString &message = QString());

signals:
    void channelsChanged();
    void urlChanged();

private slots:
    void backendConnectedToServer();
    void backendDisconnectedFromServer();
    void backendReceivedMessage(IrcMessage *message);
};

#endif // SERVERMODEL_H
