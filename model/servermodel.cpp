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
#include "appsettings.h"

ServerModel::ServerModel(IrcModel *parent, const QString &url, Irc::Session *backend) :
    QObject((QObject*)parent),
    _channels(new QObjectListModel<ChannelModel>(this)),
    _url(url),
    _backend(backend),
    _isDefaultBufferConnected(false)
{
    _settings = new AppSettings(this);
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
        _backend->quit(_settings->quitMessage());
        _backend->deleteLater();
    }
}

void ServerModel::backendConnectedToServer()
{
    qDebug() << "backend of " << url() << " is now connected to server";
    if (!_isDefaultBufferConnected)
        _isDefaultBufferConnected = connect(_backend->defaultBuffer(), SIGNAL(numericMessageReceived(QString,uint,QStringList)), this, SLOT(receiveNumericMessageFromBackend(QString,uint,QStringList)));
}

void ServerModel::backendDisconnectedFromServer()
{
    qDebug() << "backend for " << url() << " has been disconnected from the server";
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
    {
        QString newNick = _backend->nick() + "_";
        displayError("The nickname '" + _backend->nick() + "'' is already in use. Trying '" + newNick + "'.");
        changeNick(newNick);
    }
    else if (x == Irc::Rfc::ERR_NICKCOLLISION)
    {
        displayError("Nick name collision!");
    }
    else if (x == Irc::Rfc::ERR_BANLISTFULL)
    {
        displayError("Ban list is full.");
    }
    else if (x == Irc::Rfc::ERR_BANNEDFROMCHAN)
    {
        displayError("You are banned from this channel.");
    }
    else if (x == Irc::Rfc::ERR_CANNOTSENDTOCHAN)
    {
        displayError("You can't send messages to this channel.");
    }
    else if (x == Irc::Rfc::ERR_CHANNELISFULL)
    {
        displayError("Channel is full.");
    }
    else if (x == Irc::Rfc::ERR_CHANOPRIVSNEEDED)
    {
        displayError("Channel operator privileges are needed.");
    }
    else if (x == Irc::Rfc::ERR_INVITEONLYCHAN)
    {
        displayError("You can only join this channel if you're invited.");
    }
    else if (x == Irc::Rfc::ERR_NOSUCHCHANNEL)
    {
        displayError("There is no such channel.");
    }
    else if (x == Irc::Rfc::ERR_NOSUCHNICK)
    {
        displayError("There is no such nickname.");
    }
    else if (x == Irc::Rfc::ERR_UNKNOWNCOMMAND)
    {
        displayError("Unknown command.");
    }
    else if (x >= 400)
    {
        displayError("An error occoured! Error code is: " + QString::number(x));
    }
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
    _backend->part(channelName, _settings->partMessage());
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

bool ServerModel::msgUser(const QString &userName, const QString &msg)
{
    qDebug() << "msging user " << userName;
    if (!_backend->buffer(userName))
        _backend->addBuffer(userName);
    _backend->buffer(userName)->message(msg);
    return true;
}

bool ServerModel::kickUser(const QString &user, const QString &channel, const QString &message)
{
    qDebug() << "kick user" << user << " from " << channel;
    if (message.length())
        _backend->kick(user, channel, message);
    else
        _backend->kick(user, channel, _settings->kickMessage());
    return true;
}
