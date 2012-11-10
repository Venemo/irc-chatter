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

#include <QtCore>
#include <QStringList>

#include <unistd.h>

#include "ircmodel.h"
#include "settings/appsettings.h"
#include "clients/communiircclient.h"

static bool channelLessThan(ChannelModel * m1, ChannelModel *m2)
{
    if (static_cast<ServerModel*>(m1->parent())->url() != static_cast<ServerModel*>(m2->parent())->url())
        return static_cast<ServerModel*>(m1->parent())->url() < static_cast<ServerModel*>(m2->parent())->url();

    if (m1->channelType() == m2->channelType())
        return m1->name() < m2->name();

    return m1->channelType() < m2->channelType();
}

IrcModel::IrcModel(QObject *parent, AppSettings *appSettings) :
    QObject(parent),
    _networkConfigurationManager(new QNetworkConfigurationManager(this)),
    _networkSession(0),
    _currentChannelIndex(-1),
    _isAppInFocus(true),
    _appSettings(appSettings),
    _isOnline(false)
{
    //connect(_networkConfigurationManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(onlineStateChanged(bool)));
}

void IrcModel::networkSessionOpened()
{
    qDebug() << "Network session opened";
    onlineStateChanged(true);
}

void IrcModel::networkSessionClosed()
{
    qDebug() << "Network session closed";
    onlineStateChanged(false);
}

void IrcModel::networkSessionStateChanged(QNetworkSession::State state)
{
    switch (state)
    {
    case QNetworkSession::Invalid:
    {
        qDebug() << "Network session state is Invalid";
        break;
    }
    case QNetworkSession::NotAvailable:
    {
        qDebug() << "Network session state is NotAvailable";
        break;
    }
    case QNetworkSession::Connecting:
    {
        qDebug() << "Network session state is Connecting";
        break;
    }
    case QNetworkSession::Connected:
    {
        qDebug() << "Network session state is Connected";
        break;
    }
    case QNetworkSession::Closing:
    {
        qDebug() << "Network session state is Closing";
        break;
    }
    case QNetworkSession::Disconnected:
    {
        qDebug() << "Network session state is Disconnected";
        break;
    }
    case QNetworkSession::Roaming:
    {
        qDebug() << "Network session state is Roaming";
        break;
    }
    default:
    {
        qDebug() << "unsupported QNetworkSession::State value!";
        break;
    }
    }
}

void IrcModel::connectToServers()
{
    if (_networkSession == 0)
    {
        _networkSession = new QNetworkSession(_networkConfigurationManager->defaultConfiguration(), _networkConfigurationManager);
        connect(_networkSession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(networkSessionStateChanged(QNetworkSession::State)));
        connect(_networkSession, SIGNAL(opened()), this, SLOT(networkSessionOpened()));
        connect(_networkSession, SIGNAL(closed()), this, SLOT(networkSessionClosed()));
    }

    if (!_networkSession->isOpen())
        _networkSession->open();

    foreach (ServerSettings *serverSettings, _appSettings->serverSettings()->getList())
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

    _queue.clear();
    _servers.clear();

    refreshChannelList();
    _networkSession->close();
}

bool IrcModel::anyServersToConnect()
{
    int i = 0;

    foreach (ServerSettings *serverSettings, _appSettings->serverSettings()->getList())
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
        currentChannelName = currentChannel()->name();
        currentServerName = currentServer()->url();
    }

    QList<ChannelModel*> *allChannelsList = new QList<ChannelModel*>(),
            *oldChannelsList = &_allChannels.getList();

    foreach (ServerModel *serverModel, _servers)
    {
        QList<ChannelModel*> serverChannels = serverModel->channels().values();
        qSort(serverChannels.begin(), serverChannels.end(), channelLessThan);
        *allChannelsList += serverChannels;
    }

    setCurrentChannelIndex(-1);
    _allChannels.setList(allChannelsList);
    if (currentServerName.length() && currentChannelName.length())
        setCurrentChannel(currentChannelName, currentServerName);
    delete oldChannelsList;
}

int IrcModel::getChannelIndex(const QString &currentChannelName, const QString &currentServerName)
{
    int i = 0;
    foreach (ChannelModel *channelModel, _allChannels.getList())
    {
        if (static_cast<ServerModel*>(channelModel->parent())->url() == currentServerName && channelModel->name() == currentChannelName)
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
    if (online == _isOnline)
        return;

    setIsOnline(online);

    if (online)
    {
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
