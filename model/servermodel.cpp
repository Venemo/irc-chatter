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
#include <Irc>
#include <IrcSession>
#include <IrcSender>
#include <IrcUtil>
#include <IrcGlobal>
#include <IrcMessage>
#include <IrcCommand>

#include "servermodel.h"
#include "ircmodel.h"
#include "appsettings.h"

ServerModel::ServerModel(IrcModel *parent, const QString &url, IrcSession *backend) :
    QObject((QObject*)parent),
    _url(url),
    _backend(backend),
    _defaultChannel(0)
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

    foreach (QString channelName, _autoJoinChannels)
        joinChannel(channelName);
}

void ServerModel::backendDisconnectedFromServer()
{
    qDebug() << "backend for " << url() << " has been disconnected from the server";
}

void ServerModel::backendReceivedMessage(IrcMessage *message)
{
    QString channelName;

    switch (message->type())
    {
    case IrcMessage::Private:
        channelName = static_cast<IrcPrivateMessage*>(message)->target().startsWith('#')
                ? static_cast<IrcPrivateMessage*>(message)->target() // This is a channel message
                : message->sender().name(); // This is a private message

        if (static_cast<IrcPrivateMessage*>(message)->isAction())
        {
            // This is a CTCP action
            findOrCreateChannel(channelName)->receiveCtcpAction(message->sender().name(), static_cast<IrcPrivateMessage*>(message)->message());
        }
        else if (((IrcPrivateMessage*)message)->isRequest())
        {
            // This is a CTCP request
            findOrCreateChannel(channelName)->receiveCtcpRequest(message->sender().name(), static_cast<IrcPrivateMessage*>(message)->message());
        }
        else
        {
            // This is a normal message
            findOrCreateChannel(channelName)->receiveMessage(message->sender().name(), static_cast<IrcPrivateMessage*>(message)->message());
        }
        break;
    case IrcMessage::Join:
        // This is a join message
        if (_channels.contains(static_cast<IrcJoinMessage*>(message)->channel()))
        {
            _channels[static_cast<IrcJoinMessage*>(message)->channel()]->receiveJoined(message->sender().name());
        }
        break;
    case IrcMessage::Part:
        // This is a part message
        if (_channels.contains(static_cast<IrcPartMessage*>(message)->channel()))
        {
            _channels[static_cast<IrcPartMessage*>(message)->channel()]->receiveParted(message->sender().name(), static_cast<IrcPartMessage*>(message)->reason());
        }
        break;
    case IrcMessage::Nick:
        // This is a nick change message
        foreach (ChannelModel *channel, _channels.values())
        {
            if (channel->_soFarReceivedUserNames.contains(message->sender().name()))
            {
                channel->receiveNickChange(message->sender().name(), static_cast<IrcNickMessage*>(message)->nick());
            }
        }
        break;
    case IrcMessage::Quit:
        // This is a quit message
        foreach (ChannelModel *channel, _channels.values())
        {
            if (channel->_soFarReceivedUserNames.contains(message->sender().name()))
            {
                channel->receiveQuit(message->sender().name(), static_cast<IrcQuitMessage*>(message)->reason());
            }
        }
        break;
    case IrcMessage::Topic:
        // This is a topic message
        if (_channels.contains(static_cast<IrcTopicMessage*>(message)->channel()))
        {
            _channels[static_cast<IrcTopicMessage*>(message)->channel()]->receiveTopic(static_cast<IrcTopicMessage*>(message)->topic());
        }
        break;
    case IrcMessage::Notice:
        if (static_cast<IrcNoticeMessage*>(message)->isReply())
        {
            // This is a CTCP reply message
            static_cast<IrcModel*>(parent())->currentChannel()->receiveCtcpReply(message->sender().name(), static_cast<IrcNoticeMessage*>(message)->message());
        }
        else if (static_cast<IrcNoticeMessage*>(message)->target().startsWith('#'))
        {
            // This is a channel message ???
            findOrCreateChannel(static_cast<IrcNoticeMessage*>(message)->target())->receiveMessage(message->sender().name(), static_cast<IrcNoticeMessage*>(message)->message());
        }
        else
        {
            // This is a private message
            findOrCreateChannel(message->sender().name())->receiveMessage(message->sender().name(), static_cast<IrcNoticeMessage*>(message)->message());
        }
        break;
    case IrcMessage::Invite:
        if (_channels.contains(static_cast<IrcInviteMessage*>(message)->channel()))
        {
            _channels[static_cast<IrcInviteMessage*>(message)->channel()]->receiveInvite(message->sender().name(), static_cast<IrcInviteMessage*>(message)->user());
        }
        else if (static_cast<IrcInviteMessage*>(message)->user() == _backend->userName())
        {
            emit inviteReceived(static_cast<IrcInviteMessage*>(message)->channel());
        }
    case IrcMessage::Kick:
        if (_channels.contains(static_cast<IrcKickMessage*>(message)->channel()))
        {
            if (static_cast<IrcKickMessage*>(message)->user() == _backend->userName())
            {
                removeModelForChannel(static_cast<IrcKickMessage*>(message)->channel());
                emit kickReceived(static_cast<IrcKickMessage*>(message)->channel(), static_cast<IrcKickMessage*>(message)->reason());
            }
            else
            {
                _channels[static_cast<IrcKickMessage*>(message)->channel()]->receiveKicked(message->sender().name(), static_cast<IrcKickMessage*>(message)->user(), static_cast<IrcKickMessage*>(message)->reason());
            }
        }
    case IrcMessage::Mode:
        if (_channels.contains(static_cast<IrcModeMessage*>(message)->target()))
        {
            _channels[static_cast<IrcModeMessage*>(message)->target()]->receiveModeChange(static_cast<IrcModeMessage*>(message)->mode(), static_cast<IrcModeMessage*>(message)->argument());
        }
    case IrcMessage::Error:
        // TODO? Errors are also appearing as numeric messages
    case IrcMessage::Ping:
    case IrcMessage::Pong:
        break;
    case IrcMessage::Numeric:
        processNumericMessage((IrcNumericMessage*)message);
        break;
    case IrcMessage::Unknown:
    default:
        qDebug() << "Unknown message received from" << _backend->host() << "command is" << message->command() << "parameters are" << message->parameters();
        break;
    }
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
        ChannelModel *channel = new ChannelModel(this, channelName, _backend);
        if (_channels.count() == 0)
        {
            _defaultChannel = channel;
            channel->_channelType = ChannelModel::Server;
        }
        else if (channelName.startsWith('#'))
        {
            channel->_channelType = ChannelModel::Channel;
        }
        else
        {
            channel->_channelType = ChannelModel::Query;
        }
        _channels[channelName] = channel;
        emit this->channelsChanged();
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

void ServerModel::displayError(const QString &error)
{
    if (_defaultChannel)
        _defaultChannel->appendError(error);
}

void ServerModel::processNumericMessage(IrcNumericMessage *message)
{
    if (message->code() == Irc::RPL_ENDOFNAMES)
    {
        _channels[message->parameters()[1]]->updateUserList();
    }
    else if (message->code() == Irc::RPL_NAMREPLY || message->code() == Irc::RPL_NAMREPLY_)
    {
        QStringList receivedNames = message->parameters().at(3).split(' ', QString::SkipEmptyParts),
                newNames;

        foreach (QString str, receivedNames)
        {
            if (str.startsWith('@') || str.startsWith('+'))
                newNames.append(str.remove(0, 1));
            else
                newNames.append(str);
        }

        _channels[message->parameters()[2]]->_soFarReceivedUserNames += newNames;
    }
    else if (message->code() == Irc::RPL_MOTD)
    {
        if (_defaultChannel)
            _defaultChannel->receiveMotd(message->parameters().at(1));
    }
    else if (message->code() == Irc::RPL_TOPIC)
    {
        if (_channels.contains(message->parameters().at(1)))
            _channels[message->parameters().at(1)]->receiveTopic(message->parameters().at(2));
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
    else
    {
        //qDebug() << message->code() << "received from" << url() << "command is" << message->command() << "parameters are" << message->parameters();
    }
}

void ServerModel::joinChannel(const QString &channelName)
{
    qDebug() << "joining channel " << channelName;

    if (!_channels.contains(channelName))
    {
        addModelForChannel(channelName);
        _backend->sendCommand(IrcCommand::createJoin(channelName));
        //_backend->sendCommand(IrcCommand::createTopic(channelName));
        //_backend->sendCommand(IrcCommand::createNames(channelName));
    }
}

void ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel " << channelName;

    if (_channels.contains(channelName))
    {
        removeModelForChannel(channelName);
        _backend->sendCommand(IrcCommand::createPart(channelName, _settings->partMessage()));
    }
}

void ServerModel::queryUser(const QString &userName)
{
    qDebug() << "querying user " << userName;
    addModelForChannel(userName);
}

void ServerModel::closeUser(const QString &userName)
{
    qDebug() << "closing user " << userName;
    removeModelForChannel(userName);
}

void ServerModel::changeNick(const QString &nick)
{
    qDebug() << "changing nick to " << nick;
    _backend->setNickName(nick);
}

void ServerModel::msgUser(const QString &userName, const QString &msg)
{
    qDebug() << "msging user " << userName;
    if (!_channels.contains(userName))
        addModelForChannel(userName);

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
