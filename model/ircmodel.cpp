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

#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include "model/ircmodel.h"
#include "settings/appsettings.h"
#include "clients/communiircclient.h"

static bool channelLessThan(ChannelModel * m1, ChannelModel *m2)
{
    if (static_cast<ServerModel*>(m1->parent())->url() != static_cast<ServerModel*>(m2->parent())->url())
        return static_cast<ServerModel*>(m1->parent())->url() < static_cast<ServerModel*>(m2->parent())->url();

    if (m1->channelType() == m2->channelType())
        return m1->name().compare(m2->name(), Qt::CaseInsensitive) < 0;

    return m1->channelType() < m2->channelType();
}

IrcModel::IrcModel(QObject *parent, AppSettings *appSettings) :
    QObject(parent),
    _currentChannelIndex(-1),
    _isAppInFocus(true),
    _appSettings(appSettings),
    _isOnline(false),
    _networkConfigurationManager(new QNetworkConfigurationManager(this))
{
    _isOnline = _networkConfigurationManager->isOnline();

    connect(_networkConfigurationManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(onlineStateChanged(bool)));
    connect(_networkConfigurationManager, SIGNAL(configurationChanged(QNetworkConfiguration)), this, SLOT(networkConfigurationChanged(QNetworkConfiguration)));
}

void IrcModel::networkConfigurationChanged(QNetworkConfiguration config)
{
    qDebug() << Q_FUNC_INFO << "config details" << config.name() << config.identifier() << config.state();

    if (config.identifier() != _lastNetConfigId && config.state() == QNetworkConfiguration::Active)
    {
        onlineStateChanged(false);
        QTimer::singleShot(2000, this, SLOT(attemptReconnect()));
    }
}

void IrcModel::attemptReconnect()
{
    onlineStateChanged(true);
}

void IrcModel::connectToServers()
{
    foreach (ServerSettings *serverSettings, *(_appSettings->serverSettings()->getList<ServerSettings>()))
    {
        if (serverSettings->shouldConnect())
            connectToServer(serverSettings);
    }
}

void IrcModel::disconnectFromServers()
{
    foreach (ServerModel *serverModel, _servers)
    {
        qDebug() << "disconnecting from server " << serverModel->url();
        serverModel->disconnectFromServer();
        serverModel->deleteLater();
    }

    setCurrentChannelIndex(-1);
    _queue.clear();
    _servers.clear();

    refreshChannelList();
}

bool IrcModel::anyServersToConnect()
{
    int i = 0;

    foreach (ServerSettings *serverSettings, *(_appSettings->serverSettings()->getList<ServerSettings>()))
    {
        if (serverSettings->shouldConnect())
            i++;
    }

    return i > 0;
}

void IrcModel::connectToServer(ServerSettings *serverSettings)
{
    qDebug() << "trying to connect to" << serverSettings->serverUrl();

    if (isOnline())
    {
        serverSettings->setIsConnecting(true);

        AbstractIrcClient *ircClient = new CommuniIrcClient(this, serverSettings);
        ServerModel *serverModel = new ServerModel(this, serverSettings, ircClient);

        _servers.append(serverModel);
        connect(serverModel, SIGNAL(channelsChanged()), this, SLOT(refreshChannelList()));
        ircClient->connectToServer();
    }
    else
    {
        // Putting this IRC connection to the waiting queue
        qDebug() << "network is offline, adding this to the queue";
        _queue.append(serverSettings);
    }
}

void IrcModel::disconnectFromServer(ServerSettings *serverSettings)
{
    serverSettings->setIsConnected(false);
    serverSettings->setIsConnecting(false);

    ServerModel *serverModel = 0;

    foreach (ServerModel *sm, _servers)
    {
        if (sm->serverSettings() == serverSettings)
        {
            serverModel = sm;
            break;
        }
    }

    if (serverModel)
    {
        qDebug() << "disconnecting from server " << serverModel->url();
        serverModel->disconnectFromServer();

        _queue.removeAll(serverSettings);
        _servers.removeAll(serverModel);

        _currentChannelIndex = -1;
        refreshChannelList();

        serverModel->deleteLater();
    }
}

void IrcModel::refreshChannelList()
{
    QString currentChannelName;
    QString currentServerName;

    if (_currentChannelIndex != -1)
    {
        ChannelModel *channel = currentChannel();

        if (channel)
        {
            currentChannelName = currentChannel()->name();
            currentServerName = currentServer()->url();
        }
        else
        {
            setCurrentChannelIndex(-1);
        }
    }

    QList<ChannelModel*> *allChannelsList = new QList<ChannelModel*>();

    foreach (ServerModel *serverModel, _servers)
    {
        QList<ChannelModel*> serverChannels = serverModel->channels().values();
        qSort(serverChannels.begin(), serverChannels.end(), channelLessThan);
        *allChannelsList += serverChannels;
    }

    setCurrentChannelIndex(-1);
    // The QObjectListModel automatically deletes the old list, so this is not a memory leak
    _allChannels.setList(allChannelsList);

    if (currentServerName.length() && currentChannelName.length())
        setCurrentChannel(currentChannelName, currentServerName);
}

int IrcModel::getChannelIndex(const QString &currentChannelName, const QString &currentServerName)
{
    int i = 0;
    foreach (ChannelModel *channelModel, *(_allChannels.getList<ChannelModel>()))
    {
        if (static_cast<ServerModel*>(channelModel->parent())->url() == currentServerName && channelModel->name().toLower() == currentChannelName.toLower())
        {
            return i;
        }
        i++;
    }
    return -1;
}

void IrcModel::setCurrentChannel(const QString &currentChannelName, const QString &currentServerName)
{
    setCurrentChannelIndex(getChannelIndex(currentChannelName, currentServerName));
}

void IrcModel::onlineStateChanged(bool online)
{
    if (_isOnline == online)
        return;

    setIsOnline(online);

    if (online)
    {
        _lastNetConfigId = _networkConfigurationManager->defaultConfiguration().identifier();

        // If there are any connections waiting, let's connect them now
        qDebug() << "there are" << _queue.count() << "connections queued";
        qDebug() << "there are" << _servers.count() << "servers to which connection was lost";

        // Already connected but lost
        if (_servers.count())
        {
            foreach (ServerModel *serverModel, _servers)
            {
                qDebug() << "reconnecting to server " << serverModel->url();
                serverModel->connectToServer();
            }
        }

        // Queued, not yet connected
        if (_queue.count())
        {
            foreach (ServerSettings *serverModel, _queue)
            {
                _queue.removeAll(serverModel);
                connectToServer(serverModel);
            }
        }
    }
    else
    {
        qDebug() << "Network connection lost";

        foreach (ServerModel *serverModel, _servers)
        {
            qDebug() << "disconnecting from server " << serverModel->url();
            serverModel->disconnectFromServer();
        }
    }
}
