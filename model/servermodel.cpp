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
#include <IrcSession>
#include <IrcSender>
#include <IrcUtil>
#include <IrcGlobal>
#include <IrcMessage>
#include <IrcCommand>
#include <Irc>

#include "servermodel.h"
#include "ircmodel.h"
#include "appsettings.h"

ServerModel::ServerModel(IrcModel *parent, const QString &url, IrcSession *backend) :
    QObject((QObject*)parent),
    _channels(new QObjectListModel<ChannelModel>(this)),
    _url(url),
    _backend(backend)
{
    _settings = new AppSettings(this);
    if (_backend)
    {
        connect(_backend, SIGNAL(connected()), this, SLOT(backendConnectedToServer()));
        connect(_backend, SIGNAL(disconnected()), this, SLOT(backendDisconnectedFromServer()));
        connect(_backend, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(backendReceivedMessage(IrcMessage*)));
        _backend->setHost(url);
        _backend->open();
    }
}

ServerModel::~ServerModel()
{
    if (_backend)
    {
        _backend->sendCommand(IrcCommand::createQuit(_settings->quitMessage()));
        _backend->deleteLater();
    }
}

void ServerModel::backendConnectedToServer()
{
    qDebug() << "backend of " << url() << " is now connected to server";
}

void ServerModel::backendDisconnectedFromServer()
{
    qDebug() << "backend for " << url() << " has been disconnected from the server";
}

void ServerModel::backendReceivedMessage(IrcMessage *message)
{
    switch (message->type())
    {
    case IrcMessage::Numeric:
        processNumericMessage((IrcNumericMessage*)message);
        break;
    case IrcMessage::Private:
        if (((IrcPrivateMessage*)message)->isAction())
        {
            // This is a CTCP action
        }
        else if (((IrcPrivateMessage*)message)->isRequest())
        {
            // This is a CTCP request
        }
        else
        {
            // This is a normal private message
        }
        break;
    case IrcMessage::Join:
        // This is a join message
        break;
    case IrcMessage::Part:
        // This is a part message
        break;
    case IrcMessage::Nick:
        // This is a nick change message
        break;
    case IrcMessage::Quit:
        // This is a quit message
        break;
    case IrcMessage::Invite:
        // This is an invite message
        break;
    case IrcMessage::Topic:
    case IrcMessage::Kick:
    case IrcMessage::Mode:
    case IrcMessage::Notice:
    case IrcMessage::Ping:
    case IrcMessage::Pong:
    case IrcMessage::Error:
        break;
    case IrcMessage::Unknown:
    default:
        qDebug() << "Unknown message received from" << _backend->host();
        break;
    }
}

void ServerModel::addModelForSender(const QString &sender)
{
    foreach (ChannelModel *channel, _channels->getList())
        if (channel->name() == sender)
            return;

    _channels->addItem(new ChannelModel(this, sender, _backend));
}

void ServerModel::removeModelForSender(const QString &sender)
{
    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == sender)
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

void ServerModel::processNumericMessage(IrcNumericMessage *message)
{
    if (message->code() == Irc::RPL_ENDOFNAMES)
    {
        foreach (ChannelModel *channel, _channels->getList())
        {
            if (channel->name() == message->parameters()[1])
            {
                channel->updateUserList();
                break;
            }
        }
    }
    else if (message->code() == Irc::ERR_NICKNAMEINUSE)
    {
        QString newNick = _backend->nickName() + "_";
        displayError("The nickname '" + _backend->nickName() + "'' is already in use. Trying '" + newNick + "'.");
        changeNick(newNick);
    }
    else if (message->code() == Irc::ERR_NICKCOLLISION)
    {
        displayError("Nick name collision!");
    }
    else if (message->code() == Irc::ERR_BANLISTFULL)
    {
        displayError("Ban list is full.");
    }
    else if (message->code() == Irc::ERR_BANNEDFROMCHAN)
    {
        displayError("You are banned from this channel.");
    }
    else if (message->code() == Irc::ERR_CANNOTSENDTOCHAN)
    {
        displayError("You can't send messages to this channel.");
    }
    else if (message->code() == Irc::ERR_CHANNELISFULL)
    {
        displayError("Channel is full.");
    }
    else if (message->code() == Irc::ERR_CHANOPRIVSNEEDED)
    {
        displayError("Channel operator privileges are needed.");
    }
    else if (message->code() == Irc::ERR_INVITEONLYCHAN)
    {
        displayError("You can only join this channel if you're invited.");
    }
    else if (message->code() == Irc::ERR_NOSUCHCHANNEL)
    {
        displayError("There is no such channel.");
    }
    else if (message->code() == Irc::ERR_NOSUCHNICK)
    {
        displayError("There is no such nickname.");
    }
    else if (message->code() == Irc::ERR_UNKNOWNCOMMAND)
    {
        displayError("Unknown command.");
    }
    else if (message->code() >= 400)
    {
        displayError("An error occoured! Error code is: " + QString::number(message->code()));
    }
}

void ServerModel::joinChannel(const QString &channelName)
{
    qDebug() << "joining channel " << channelName;

    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == channelName)
        {
            qDebug() << "Already connected to this channel";
        }
    }

    _backend->sendCommand(IrcCommand::createJoin(channelName));
}

void ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel " << channelName;
    _backend->sendCommand(IrcCommand::createPart(channelName, _settings->partMessage()));
}

void ServerModel::queryUser(const QString &userName)
{
    qDebug() << "querying user " << userName;
    addModelForSender(userName);
}

void ServerModel::closeUser(const QString &userName)
{
    qDebug() << "closing user " << userName;
    removeModelForSender(userName);
}

void ServerModel::changeNick(const QString &nick)
{
    qDebug() << "changing nick to " << nick;
    _backend->setNickName(nick);
}

void ServerModel::msgUser(const QString &userName, const QString &msg)
{
    qDebug() << "msging user " << userName;
    _backend->sendCommand(IrcCommand::createMessage(userName, msg));
}

void ServerModel::kickUser(const QString &user, const QString &channel, const QString &message)
{
    qDebug() << "kick user" << user << " from " << channel;
    if (message.length())
        _backend->sendCommand(IrcCommand::createKick(channel, user, message));
    else
        _backend->sendCommand(IrcCommand::createKick(channel, user, _settings->kickMessage()));
}
