
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
// Copyright (C) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>
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

class IrcModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* allChannels READ allChannels NOTIFY allChannelsChanged)
    GENPROPERTY_F(int, _currentChannelIndex, currentChannelIndex, setCurrentChannelIndex, currentChannelIndexChanged)
    Q_PROPERTY(int currentChannelIndex READ currentChannelIndex WRITE setCurrentChannelIndex NOTIFY currentChannelIndexChanged)
    Q_PROPERTY(QObject* currentChannel READ currentChannel NOTIFY currentChannelIndexChanged)
    Q_PROPERTY(QObject* currentServer READ currentServer NOTIFY currentChannelIndexChanged)
    GENPROPERTY_F(bool, _isAppInFocus, isAppInFocus, setIsAppInFocus, isAppInFocusChanged)
    Q_PROPERTY(int isAppInFocus READ isAppInFocus WRITE setIsAppInFocus NOTIFY isAppInFocusChanged)
    GENPROPERTY_PTR_R(AppSettings*, _appSettings, appSettings)
    GENPROPERTY_F(bool, _isOnline, isOnline, setIsOnline, isOnlineChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY isOnlineChanged)

    QNetworkConfigurationManager *_networkConfigurationManager;
    QList<ServerSettings*> _queue;
    QList<ServerModel*> _servers;
    QObjectListModel<ChannelModel> _allChannels;
    QString _lastNetConfigId;

public:
    explicit IrcModel(QObject *parent, AppSettings *appSettings);
    inline QObjectListModel<ChannelModel> *allChannels() { return &_allChannels; }
    inline ChannelModel *currentChannel() { return _servers.count() ? static_cast<ChannelModel*>(allChannels()->getItem(_currentChannelIndex)) : 0; }
    inline ServerModel *currentServer() { return currentChannel() ? static_cast<ServerModel*>(currentChannel()->parent()) : 0; }
    int getChannelIndex(const QString &currentChannelName, const QString &currentServerName);
    void setCurrentChannel(const QString &currentChannelName, const QString &currentServerName);

    Q_INVOKABLE void connectToServer(ServerSettings *serverSettings);
    Q_INVOKABLE void disconnectFromServer(ServerSettings *serverSettings);
    Q_INVOKABLE void connectToServers();
    Q_INVOKABLE void disconnectFromServers();
    Q_INVOKABLE bool anyServersToConnect();

public slots:
    void refreshChannelList();

private slots:
    void onlineStateChanged(bool online);
    void networkConfigurationChanged(QNetworkConfiguration);

signals:
    void allChannelsChanged();
    void serversChanged();
    void currentChannelIndexChanged();
    void isAppInFocusChanged();
    void isOnlineChanged();
};

#endif // IRCMODEL_H
