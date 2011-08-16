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
#include <ircclient-qt/IrcSession>
#include <ircclient-qt/IrcBuffer>
#include <ircclient-qt/IrcGlobal>
#include <ircclient-qt/IrcUtil>

#include "servermodel.h"
#include "ircmodel.h"

ServerModel::ServerModel(IrcModel *parent, const QString &url, Irc::Session *backend) :
    QObject((QObject*)parent),
    _channels(new QObjectListModel<ChannelModel>(this)),
    _url(url),
    _backend(backend)
{
    if (_backend)
    {
        connect(_backend, SIGNAL(connected()), this, SLOT(backendConnectedToServer()));
        connect(_backend, SIGNAL(bufferAdded(Irc::Buffer*)), this, SLOT(backendAddedBuffer(Irc::Buffer*)));
        connect(_backend, SIGNAL(bufferRemoved(Irc::Buffer*)), this, SLOT(backendRemovedBuffer(Irc::Buffer*)));
        connect(_backend, SIGNAL(disconnected()), this, SLOT(backendDisconnectedFromServer()));
        _backend->connectToServer(_url);
    }
}

ServerModel::~ServerModel()
{
    if (_backend)
    {
        _backend->quit("IRC Chatter (the first MeeGo IRC client) closed.");
        _backend->deleteLater();
    }
}

void ServerModel::backendConnectedToServer()
{
    connect(_backend->defaultBuffer(), SIGNAL(numericMessageReceived(QString,uint,QStringList)), this, SLOT(receiveNumericMessageFromBackend(QString,uint,QStringList)));
}

void ServerModel::backendAddedBuffer(Irc::Buffer *buffer)
{
    qDebug() << "backend added buffer " << buffer->receiver();
    _channels->addItem(new ChannelModel(this, buffer));
}

void ServerModel::backendRemovedBuffer(Irc::Buffer *buffer)
{
    qDebug() << "backend removed buffer " << buffer->receiver();
    removeModelForBuffer(buffer);
}

void ServerModel::removeModelForBuffer(Irc::Buffer *buffer)
{
    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == buffer->receiver())
        {
            if (channel == static_cast<IrcModel*>(parent())->currentChannel())
                static_cast<IrcModel*>(parent())->setCurrentChannelIndex(static_cast<IrcModel*>(parent())->currentChannelIndex() - 1);

            _channels->removeItem(channel);
            channel->deleteLater();
            break;
        }
    }
}

void ServerModel::displayError(const QString &error)
{
    static_cast<IrcModel*>(parent())->currentChannel()->appendError(error);
}

void ServerModel::receiveNumericMessageFromBackend(const QString &name, uint x, const QStringList &message)
{
    Q_UNUSED(name);

    if (x == Irc::Rfc::RPL_ENDOFNAMES)
    {
        foreach (ChannelModel *channel, _channels->getList())
        {
            if (channel->name() == message[1])
            {
                channel->updateUserList();
                break;
            }
        }
    }
    else if (x == Irc::Rfc::RPL_TOPIC)
    {
        foreach (ChannelModel *channel, _channels->getList())
        {
            if (channel->name() == message[1])
            {
                channel->setTopic(message[2]);
                break;
            }
        }
    }
    else if (x == Irc::Rfc::ERR_NICKNAMEINUSE)
        displayError("This nickname is already in use.");
    else if (x == Irc::Rfc::ERR_NICKCOLLISION)
        displayError("Nick name collision!");
    else if (x >= 400)
        displayError("An error occoured! Error code is: " + x);
}

bool ServerModel::joinChannel(const QString &channelName)
{
    qDebug() << "joining channel " << channelName;

    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == channelName)
        {
            qDebug() << "Already connected to this channel";
        }
    }

    _backend->join(channelName);
    return true;
}

bool ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel " << channelName;
    _backend->part(channelName, "Leaving this channel. (with IRC Chatter, the first MeeGo IRC client)");
    return true;
}

bool ServerModel::queryUser(const QString &userName)
{
    qDebug() << "querying user " << userName;
    _backend->addBuffer(userName);
    return true;
}

bool ServerModel::closeUser(const QString &userName)
{
    qDebug() << "closing user " << userName;
    removeModelForBuffer(_backend->buffer(userName));
    return true;
}

bool ServerModel::changeNick(const QString &nick)
{
    qDebug() << "changing nick to " << nick;
    _backend->setNick(nick);
    return true;
}
