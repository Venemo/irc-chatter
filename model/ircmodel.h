
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
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

#ifndef IRCMODEL_H
#define IRCMODEL_H

#include <QObject>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>

#include "channelmodel.h"
#include "servermodel.h"
#include "qobjectlistmodel.h"

class ServerSettings;
class AppSettings;

typedef QPair<ServerSettings*, AppSettings*> IrcSettingPair;

class IrcModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* allChannels READ allChannels NOTIFY allChannelsChanged)
    GENPROPERTY(int, _currentChannelIndex, currentChannelIndex, setCurrentChannelIndex, currentChannelIndexChanged)
    Q_PROPERTY(int currentChannelIndex READ currentChannelIndex WRITE setCurrentChannelIndex NOTIFY currentChannelIndexChanged)
    Q_PROPERTY(QObject* currentChannel READ currentChannel NOTIFY currentChannelIndexChanged)
    Q_PROPERTY(QObject* currentServer READ currentServer NOTIFY currentChannelIndexChanged)
    GENPROPERTY(bool, _isAppInFocus, isAppInFocus, setIsAppInFocus, isAppInFocusChanged)
    Q_PROPERTY(int isAppInFocus READ isAppInFocus WRITE setIsAppInFocus NOTIFY isAppInFocusChanged)
    GENPROPERTY(bool, _isWaitingForConnection, isWaitingForConnection, setIsWaitingForConnection, isWaitingForConnectionChanged)
    Q_PROPERTY(int isWaitingForConnection READ isWaitingForConnection WRITE setIsWaitingForConnection NOTIFY isWaitingForConnectionChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY isOnlineChanged)

    QNetworkConfigurationManager *_networkConfigurationManager;
    QNetworkSession *_networkSession;
    QList<IrcSettingPair> _queue;
    QList<ServerModel*> _servers;
    QObjectListModel<ChannelModel> _allChannels;

public:
    explicit IrcModel(QObject *parent = 0);
    inline QObjectListModel<ChannelModel> *allChannels() { return &_allChannels; }
    inline ChannelModel *currentChannel() { return _servers.count() ? static_cast<ChannelModel*>(allChannels()->getItem(_currentChannelIndex)) : 0; }
    inline ServerModel *currentServer() { return currentChannel() ? static_cast<ServerModel*>(currentChannel()->parent()) : 0; }

    Q_INVOKABLE void connectToServer(ServerSettings *server, AppSettings *settings);
    Q_INVOKABLE bool isOnline() const;

public slots:
    Q_INVOKABLE void attemptConnection();
    Q_INVOKABLE void attemptConnectionLater();
    void refreshChannelList();

private slots:
    void backendsConnectedToServer();
    void onlineStateChanged(bool online);
    void networkSessionError(QNetworkSession::SessionError error);
    void networkSessionStateChanged(QNetworkSession::State state);

signals:
    void allChannelsChanged();
    void serversChanged();
    void currentChannelIndexChanged();
    void readyToDisplay();
    void isAppInFocusChanged();
    void isWaitingForConnectionChanged();
    void isOnlineChanged();

};

#endif // IRCMODEL_H
