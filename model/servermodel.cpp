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

#include "servermodel.h"
#include "ircmodel.h"
#include "settings/appsettings.h"
#include "clients/abstractircclient.h"

ServerModel::ServerModel(IrcModel *parent, const QString &url, AbstractIrcClient *ircClient) :
    QObject((QObject*)parent),
    _url(url),
    _ircClient(ircClient),
    _defaultChannel(0)
{
    _settings = new AppSettings(this);

    connect(_ircClient, SIGNAL(connectedToServer()), this, SLOT(connectedToServer()));
    connect(_ircClient, SIGNAL(disconnectedFromServer()), this, SLOT(disconnectedFromServer()));

    connect(_ircClient, SIGNAL(receiveCtcpAction(QString,QString,QString)), this, SLOT(receiveCtcpAction(QString,QString,QString)));
    connect(_ircClient, SIGNAL(receiveCtcpReply(QString,QString)), this, SLOT(receiveCtcpReply(QString,QString)));
    connect(_ircClient, SIGNAL(receiveCtcpRequest(QString,QString)), this, SLOT(receiveCtcpRequest(QString,QString)));
    connect(_ircClient, SIGNAL(receiveError(QString)), this, SLOT(receiveError(QString)));
    connect(_ircClient, SIGNAL(receiveJoin(QString,QString)), this, SLOT(receiveJoin(QString,QString)));
    connect(_ircClient, SIGNAL(receiveKick(QString,QString,QString,QString)), this, SLOT(receiveKick(QString,QString,QString,QString)));
    connect(_ircClient, SIGNAL(receiveMessage(QString,QString,QString)), this, SLOT(receiveMessage(QString,QString,QString)));
    connect(_ircClient, SIGNAL(receiveModeChange(QString,QString,QString)), this, SLOT(receiveModeChange(QString,QString,QString)));
    connect(_ircClient, SIGNAL(receiveMotd(QString)), this, SLOT(receiveMotd(QString)));
    connect(_ircClient, SIGNAL(receiveNickChange(QString,QString)), this, SLOT(receiveMotd(QString)));
    connect(_ircClient, SIGNAL(receivePart(QString,QString,QString)), this, SLOT(receivePart(QString,QString,QString)));
    connect(_ircClient, SIGNAL(receiveQuit(QString,QString)), this, SLOT(receiveQuit(QString,QString)));
    connect(_ircClient, SIGNAL(receiveTopic(QString,QString)), this, SLOT(receiveTopic(QString,QString)));
    connect(_ircClient, SIGNAL(receiveUserNames(QString,QStringList)), this, SLOT(receiveUserNames(QString,QStringList)));

    connect(_ircClient, SIGNAL(joinedChannel(QString)), this, SLOT(addModelForChannel(QString)));
    connect(_ircClient, SIGNAL(queriedUser(QString)), this, SLOT(addModelForChannel(QString)));
    connect(_ircClient, SIGNAL(partedChannel(QString)), this, SLOT(removeModelForChannel(QString)));
    connect(_ircClient, SIGNAL(closedUser(QString)), this, SLOT(removeModelForChannel(QString)));
}

ServerModel::~ServerModel()
{
    if (_ircClient)
    {
        _ircClient->quit("Quitting. (with IRC Chatter)");
        _ircClient->deleteLater();
    }
}

void ServerModel::connectedToServer()
{
    qDebug() << "backend of " << url() << " is now connected to server";

    foreach (QString channelName, _autoJoinChannels)
    {
        addModelForChannel(channelName);
        _ircClient->joinChannel(channelName);
    }
}

void ServerModel::disconnectedFromServer()
{
    qDebug() << "backend for " << url() << " has been disconnected from the server";
}


void ServerModel::receiveUserNames(const QString &channelName, const QStringList &userNames)
{
    if (_channels.contains(channelName))
    {
        _channels[channelName]->receiveUserList(userNames);
    }
}

void ServerModel::receiveMessage(const QString &channelName, const QString &userName, const QString &message)
{

    findOrCreateChannel(channelName)->receiveMessage(userName, message);
}

void ServerModel::receiveCtcpRequest(const QString &userName, const QString &message)
{
    if (_defaultChannel)
        _defaultChannel->receiveCtcpRequest(userName, message);
}

void ServerModel::receiveCtcpReply(const QString &userName, const QString &message)
{
    if (_defaultChannel)
        _defaultChannel->receiveCtcpReply(userName, message);
}

void ServerModel::receiveCtcpAction(const QString &channelName, const QString &userName, const QString &message)
{
    findOrCreateChannel(channelName)->receiveCtcpAction(userName, message);
}

void ServerModel::receivePart(const QString &channelName, const QString &userName, const QString &message)
{
    if (_channels.contains(channelName))
    {
        _channels[channelName]->receiveParted(userName, message);
    }
}

void ServerModel::receiveQuit(const QString &userName, const QString &message)
{
    foreach (ChannelModel *channel, _channels.values())
    {
        if (channel->userNames().contains(userName))
        {
            channel->receiveQuit(userName, message);
        }
    }
}

void ServerModel::receiveJoin(const QString &channelName, const QString &userName)
{
    if (_channels.contains(channelName))
    {
        _channels[channelName]->receiveJoined(userName);
    }
}

void ServerModel::receiveTopic(const QString &channelName, const QString &topic)
{
    if (_channels.contains(channelName))
    {
        _channels[channelName]->receiveTopic(topic);
    }
}

void ServerModel::receiveKick(const QString &channelName, const QString &userName, const QString &kickedUserName, const QString &message)
{
    if (_channels.contains(channelName))
    {
        if (kickedUserName == _ircClient->currentNick())
        {
            removeModelForChannel(channelName);
            emit kickReceived(channelName, message);
        }
        else
        {
            _channels[channelName]->receiveKicked(userName, kickedUserName, message);
        }
    }
}

void ServerModel::receiveModeChange(const QString &channelName, const QString &mode, const QString &arguments)
{
    if (_channels.contains(channelName))
    {
        _channels[channelName]->receiveModeChange(mode, arguments);
    }
}

void ServerModel::receiveNickChange(const QString &oldNick, const QString &newNick)
{
    foreach (ChannelModel *channel, _channels.values())
    {
        if (channel->userNames().contains(oldNick))
        {
            channel->receiveNickChange(oldNick, newNick);
        }
    }
}

void ServerModel::receiveMotd(const QString &motd)
{
    if (_defaultChannel)
        _defaultChannel->receiveMotd(motd);
}

void ServerModel::receiveError(const QString &error)
{
    if (_defaultChannel)
        _defaultChannel->appendError(error);
}

ChannelModel *ServerModel::findOrCreateChannel(const QString &channelName)
{
    if (!_channels.contains(channelName))
        addModelForChannel(channelName);
    return _channels[channelName];
}

void ServerModel::addModelForChannel(const QString &channelName)
{
    if (!_channels.contains(channelName))
    {
        ChannelModel *channel = new ChannelModel(this, channelName, _ircClient);
        if (_channels.count() == 0)
        {
            _defaultChannel = channel;
            channel->setChannelType(ChannelModel::Server);
        }
        else if (channelName.startsWith('#'))
        {
            channel->setChannelType(ChannelModel::Channel);
        }
        else
        {
            channel->setChannelType(ChannelModel::Query);
        }

        _channels[channelName] = channel;
        emit this->channelsChanged();

        IrcModel *ircModel = static_cast<IrcModel*>(parent());

        if (ircModel->currentChannelIndex() == -1)
            ircModel->setCurrentChannelIndex(0);
    }
}

void ServerModel::removeModelForChannel(const QString &channelName)
{
    if (_channels.contains(channelName))
    {
        if (_channels[channelName] == static_cast<IrcModel*>(parent())->currentChannel())
            static_cast<IrcModel*>(parent())->setCurrentChannelIndex(static_cast<IrcModel*>(parent())->currentChannelIndex() - 1);
        _channels.remove(channelName);
        emit this->channelsChanged();
    }
}

void ServerModel::joinChannel(const QString &channelName)
{
    qDebug() << "joining channel or querying user " << channelName;

    if (!_channels.contains(channelName))
    {
        addModelForChannel(channelName);

        if (channelName.startsWith('#'))
            _ircClient->joinChannel(channelName);
        else
            _ircClient->queryUser(channelName);
    }
}

void ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel or closing user " << channelName;

    if (_channels.contains(channelName))
    {
        removeModelForChannel(channelName);

        if (channelName.startsWith('#'))
            _ircClient->partChannel(channelName, "Parting this channel. (with IRC Chatter)");
        else
            _ircClient->closeUser(channelName);
    }
}

void ServerModel::displayError(const QString &error)
{
    if (_defaultChannel)
        _defaultChannel->appendError(error);
}
