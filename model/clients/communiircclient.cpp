
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
// Copyright (C) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include <QtCore>
#include <QSslSocket>

#include <Irc>
#include <IrcSession>
#include <IrcSender>
#include <IrcUtil>
#include <IrcGlobal>
#include <IrcMessage>
#include <IrcCommand>

#include "communiircclient.h"
#include "../settings/serversettings.h"

#define FIX_EMPTY_CHANNEL_NAME(channelName) channelName.length() > 0 ? channelName : _ircSession->host()

CommuniIrcClient::CommuniIrcClient(QObject *parent, ServerSettings *serverSettings) :
    AbstractIrcClient(parent, serverSettings)
{
    _ircSession = new IrcSession(this);
    _ircSession->setNickName(serverSettings->userNickname());
    _ircSession->setHost(serverSettings->serverUrl());

    if (serverSettings->userIdent().length())
        _ircSession->setUserName(serverSettings->userIdent());
    else
        _ircSession->setUserName(serverSettings->userNickname());

    if (serverSettings->userRealName().length())
        _ircSession->setRealName(serverSettings->userRealName());
    else
        _ircSession->setRealName(serverSettings->userNickname());

    _ircSession->setPort(serverSettings->serverPort());

    if (serverSettings->serverSSL())
    {
        QSslSocket *socket = new QSslSocket(_ircSession);
        socket->ignoreSslErrors();
        socket->setPeerVerifyMode(QSslSocket::VerifyNone);
        _ircSession->setSocket(socket);
    }

    connect(_ircSession, SIGNAL(password(QString*)), serverSettings, SLOT(backendAsksForPassword(QString*)));
    connect(_ircSession, SIGNAL(connected()), this, SIGNAL(connectedToServer()));
    connect(_ircSession, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromServer()));
    connect(_ircSession, SIGNAL(messageReceived(IrcMessage*)), this, SLOT(messageReceived(IrcMessage*)));
    connect(_ircSession, SIGNAL(socketError(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
}

void CommuniIrcClient::socketError(QAbstractSocket::SocketError error)
{
    qDebug() << Q_FUNC_INFO << "socket error:" << error << "trying to reopen session";
    emit this->socketErrorHappened(error);
    QTimer::singleShot(8000, _ircSession, SLOT(open()));
}

void CommuniIrcClient::messageReceived(IrcMessage *message)
{
    switch (message->type())
    {
    case IrcMessage::Private:
    {
        IrcPrivateMessage *msg = static_cast<IrcPrivateMessage*>(message);
        QString channelName = msg->target().startsWith('#')
                ? msg->target() // This is a channel message
                : msg->sender().name(); // This is a private message

        if (msg->isAction())
        {
            // This is a CTCP action
            emit receiveCtcpAction(FIX_EMPTY_CHANNEL_NAME(channelName), msg->sender().name(), msg->message());
        }
        else if (msg->isRequest())
        {
            // This is a CTCP request
            emit receiveCtcpRequest(FIX_EMPTY_CHANNEL_NAME(msg->sender().name()), msg->message());
        }
        else
        {
            // This is a normal message
            emit receiveMessage(FIX_EMPTY_CHANNEL_NAME(channelName), msg->sender().name(), msg->message());
        }
        break;
    }
    case IrcMessage::Join:
    {
        // This is a join message
        IrcJoinMessage *msg = static_cast<IrcJoinMessage*>(message);
        emit receiveJoin(msg->channel(), msg->sender().name());
        break;
    }
    case IrcMessage::Part:
    {
        // This is a part message
        IrcPartMessage *msg = static_cast<IrcPartMessage*>(message);
        emit receivePart(msg->channel(), msg->sender().name(), msg->reason());
        break;
    }
    case IrcMessage::Nick:
    {
        // This is a nick change message
        IrcNickMessage *msg = static_cast<IrcNickMessage*>(message);
        emit receiveNickChange(msg->sender().name(), msg->nick());
        break;
    }
    case IrcMessage::Quit:
    {
        // This is a quit message
        IrcQuitMessage *msg = static_cast<IrcQuitMessage*>(message);
        emit receiveQuit(msg->sender().name(), msg->reason());
        break;
    }
    case IrcMessage::Topic:
    {
        // This is a topic message
        IrcTopicMessage *msg = static_cast<IrcTopicMessage*>(message);
        emit receiveTopic(msg->channel(), msg->topic());
        break;
    }
    case IrcMessage::Notice:
    {
        IrcNoticeMessage *msg = static_cast<IrcNoticeMessage*>(message);

        if (msg->isReply())
        {
            // This is a CTCP reply message
            emit receiveCtcpReply(FIX_EMPTY_CHANNEL_NAME(msg->sender().name()), msg->message());
        }
        else if (msg->target().startsWith('#'))
        {
            // This is a channel notice message
            emit receiveMessage(FIX_EMPTY_CHANNEL_NAME(msg->target()), msg->sender().name(), msg->message());
        }
        else
        {
            // This is a channel notice message
            emit receiveMessage(FIX_EMPTY_CHANNEL_NAME(msg->sender().name()), msg->sender().name(), msg->message());
        }
        break;
    }
    case IrcMessage::Kick:
    {
        IrcKickMessage *msg = static_cast<IrcKickMessage*>(message);
        emit receiveKick(msg->channel(), msg->sender().name(), msg->user(), msg->reason());
    }
    case IrcMessage::Mode:
    {
        IrcModeMessage *msg = static_cast<IrcModeMessage*>(message);
        emit receiveModeChange(msg->target(), msg->mode(), msg->argument());
        break;
    }
    case IrcMessage::Error:
        // TODO? Errors are also appearing as numeric messages?
        break;
    case IrcMessage::Ping:
    case IrcMessage::Pong:
        // These should be handled by communi itself
        break;
    case IrcMessage::Numeric:
        processNumericMessage((IrcNumericMessage*)message);
        break;
    case IrcMessage::Unknown:
    default:
    {
        qDebug() << "Unknown message received from" << _ircSession->host() << "command is" << message->command() << "parameters are" << message->parameters();
        break;
    }
    }
}

void CommuniIrcClient::processNumericMessage(IrcNumericMessage *message)
{
    if (message->code() == Irc::RPL_ENDOFNAMES)
    {
        QString channelName = message->parameters()[1];
        QStringList receivedUserNames = _receivedUserNames[channelName].toSet().toList();
        // workaround for bug: https://bugreports.qt.nokia.com/browse/QTBUG-12892
        // found in http://www.harshj.com/2009/10/24/sorting-entries-in-a-qstringlist-case-insensitively/
        // - Thank you!
        QMap<QString, QString> strMap;
        foreach (const QString &str, receivedUserNames)
        {
            strMap.insert(str.toLower(), str);
        }
        receivedUserNames = strMap.values();
        emit receiveUserNames(channelName, receivedUserNames);
        _receivedUserNames[channelName].clear();
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

        _receivedUserNames[message->parameters()[2]] += newNames;
    }
    else if (message->code() == Irc::RPL_MOTD)
    {
        emit receiveMotd(message->parameters().at(1));
    }
    else if (message->code() == Irc::RPL_TOPIC)
    {
        emit receiveTopic(message->parameters().at(1), message->parameters().at(2));
    }
    else if (message->code() == Irc::ERR_NICKNAMEINUSE)
    {
        QString oldNick = _ircSession->nickName();
        QString newNick = _ircSession->nickName() + "_";
        receiveError("The nickname '" + oldNick + "'' is already in use. Trying '" + newNick + "'.");
        changeNick(newNick);
    }
    else if (message->code() == Irc::ERR_NICKCOLLISION)
    {
        emit receiveError("Nick name collision!");
    }
    else if (message->code() == Irc::ERR_BANLISTFULL)
    {
        emit receiveError("Ban list is full.");
    }
    else if (message->code() == Irc::ERR_BANNEDFROMCHAN)
    {
        emit receiveError("You are banned from this channel.");
    }
    else if (message->code() == Irc::ERR_CANNOTSENDTOCHAN)
    {
        emit receiveError("You can't send messages to this channel.");
    }
    else if (message->code() == Irc::ERR_CHANNELISFULL)
    {
        emit receiveError("Channel is full.");
    }
    else if (message->code() == Irc::ERR_CHANOPRIVSNEEDED)
    {
        emit receiveError("Channel operator privileges are needed.");
    }
    else if (message->code() == Irc::ERR_INVITEONLYCHAN)
    {
        emit receiveError("You can only join this channel if you're invited.");
    }
    else if (message->code() == Irc::ERR_NOSUCHCHANNEL)
    {
        emit receiveError("There is no such channel.");
    }
    else if (message->code() == Irc::ERR_NOSUCHNICK)
    {
        emit receiveError("There is no such nickname.");
    }
    else if (message->code() == Irc::ERR_UNKNOWNCOMMAND)
    {
        emit receiveError("Unknown command.");
    }
    else if (message->code() >= 400)
    {
        emit receiveError("An error occoured! Error code is: " + QString::number(message->code()));
    }
    else
    {
        //qDebug() << message->code() << "received from" << url() << "command is" << message->command() << "parameters are" << message->parameters();
    }
}

const QString CommuniIrcClient::currentNick()
{
    return _ircSession->nickName();
}

void CommuniIrcClient::connectToServer()
{
    _ircSession->open();
}

void CommuniIrcClient::disconnectFromServer()
{
    _ircSession->close();
}

void CommuniIrcClient::quit(const QString &message)
{
    _ircSession->sendCommand(IrcCommand::createQuit(message));
}

void CommuniIrcClient::joinChannel(const QString &channelName, const QString &channelKey)
{
    _ircSession->sendCommand(IrcCommand::createJoin(channelName, channelKey));
    emit joinedChannel(FIX_EMPTY_CHANNEL_NAME(channelName));
}

void CommuniIrcClient::partChannel(const QString &channelName, const QString &message)
{
    _ircSession->sendCommand(IrcCommand::createPart(channelName, message));
    emit partedChannel(FIX_EMPTY_CHANNEL_NAME(channelName));
}

void CommuniIrcClient::queryUser(const QString &userName)
{
    Q_UNUSED(userName);
    emit queriedUser(userName);
}

void CommuniIrcClient::closeUser(const QString &userName)
{
    Q_UNUSED(userName);
    emit closedUser(userName);
}

void CommuniIrcClient::sendCtcpAction(const QString &channelName, const QString &action)
{
    _ircSession->sendCommand(IrcCommand::createCtcpAction(channelName, action));
    emit receiveCtcpAction(channelName, _ircSession->nickName(), action);
}

void CommuniIrcClient::sendCtcpRequest(const QString &userName, const QString &request)
{
    _ircSession->sendCommand(IrcCommand::createCtcpRequest(userName, request));
}

void CommuniIrcClient::sendCtcpReply(const QString &userName, const QString &message)
{
    _ircSession->sendCommand(IrcCommand::createCtcpReply(userName, message));
}

void CommuniIrcClient::sendMessage(const QString &channelName, const QString &message)
{
    _ircSession->sendCommand(IrcCommand::createMessage(channelName, message));
    emit receiveMessage(channelName, _ircSession->nickName(), message);
}

void CommuniIrcClient::requestTopic(const QString &channelName)
{
    _ircSession->sendCommand(IrcCommand::createTopic(channelName));
}

void CommuniIrcClient::changeNick(const QString &newNick)
{
    _ircSession->setNickName(newNick);
}

void CommuniIrcClient::kick(const QString &channelName, const QString &userName, const QString &message)
{
    _ircSession->sendCommand(IrcCommand::createKick(channelName, userName, message));
}

void CommuniIrcClient::sendRaw(const QString &message)
{
    _ircSession->sendRaw(message);
}

QAbstractSocket *CommuniIrcClient::socket()
{
    return _ircSession->socket();
}
