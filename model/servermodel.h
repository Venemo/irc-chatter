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

#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QObject>
#include <QHash>

#include "util.h"
#include "qobjectlistmodel.h"
#include "channelmodel.h"
#include "settings/appsettings.h"

class AbstractIrcClient;
class IrcModel;

class ServerModel : public QObject
{
    Q_OBJECT

    GENPROPERTY_R(QString, _url, url)
    Q_PROPERTY(QString url READ url)

    QHash<QString, ChannelModel*> _channels;
    AbstractIrcClient *_ircClient;
    AppSettings *_settings;
    ChannelModel *_defaultChannel;
    QStringList _autoJoinChannels;

    friend class IrcModel;
    friend class AppSettings;

protected:
    explicit ServerModel(IrcModel *parent, const QString &url, AbstractIrcClient *_ircClient);
    ChannelModel *findOrCreateChannel(const QString &channelName);

public:
    ~ServerModel();
    Q_INVOKABLE void joinChannel(const QString &channelName);
    Q_INVOKABLE void partChannel(const QString &channelName);
    Q_INVOKABLE void displayError(const QString &error);

signals:
    void channelsChanged();
    void kickReceived(const QString &channelName, const QString &reason);

private slots:
    void addModelForChannel(const QString &channelName);
    void removeModelForChannel(const QString &channelName);

    // Messages corresponding to the server itself.
    void connectedToServer();
    void disconnectedFromServer();
    void receiveQuit(const QString &userName, const QString &message);
    void receiveNickChange(const QString &oldNick, const QString &newNick);
    void receiveMotd(const QString &motd);
    void receiveError(const QString &error);

    // Messages corresponding to a single channel.
    void receiveUserNames(const QString &channelName, const QStringList &userNames);
    void receiveMessage(const QString &channelName, const QString &userName, const QString &message);
    void receiveCtcpRequest(const QString &userName, const QString &message);
    void receiveCtcpReply(const QString &userName, const QString &message);
    void receiveCtcpAction(const QString &channelName, const QString &userName, const QString &message);
    void receivePart(const QString &channelName, const QString &userName, const QString &message);
    void receiveJoin(const QString &channelName, const QString &userName);
    void receiveTopic(const QString &channelName, const QString &topic);
    void receiveKick(const QString &channelName, const QString &userName, const QString &kickedUserName, const QString &message);
    void receiveModeChange(const QString &channelName, const QString &mode, const QString &arguments);
};

#endif // SERVERMODEL_H
