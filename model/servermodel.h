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

#include <QtCore/QObject>

#include "helpers/util.h"
#include "helpers/qobjectlistmodel.h"
#include "model/channelmodel.h"
#include "model/channelmodelcollection.h"
#include "model/settings/appsettings.h"

class AbstractIrcClient;
class IrcModel;
class ServerSettings;

class ServerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QObject* serverSettings READ serverSettings NOTIFY serverSettingsChanged)
    Q_PROPERTY(QObject* defaultChannel READ defaultChannel NOTIFY defaultChannelChanged)

    ChannelModelCollection _channels;
    AbstractIrcClient *_ircClient;
    ServerSettings *_serverSettings;
    ChannelModel *_defaultChannel;

    friend class AppSettings;

protected:

public:
    explicit ServerModel(IrcModel *parent, ServerSettings *serverSettings, AbstractIrcClient *_ircClient);
    ~ServerModel();

    const QString &url() const;
    ServerSettings *serverSettings() const;
    ChannelModel *defaultChannel() const;

    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void disconnectFromServer();
    ChannelModel *findOrCreateChannel(const QString &channelName);
    ChannelModelCollection &channels();

    Q_INVOKABLE void joinChannel(const QString &channelName, const QString &channelKey = QString());
    Q_INVOKABLE void partChannel(const QString &channelName);
    Q_INVOKABLE void displayError(const QString &error);

signals:
    void channelsChanged();
    void serverSettingsChanged();
    void defaultChannelChanged();
    void kickReceived(const QString &channelName, const QString &reason);

private slots:
    void socketConnected();
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
