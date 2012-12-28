
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

#ifndef ABSTRACTIRCCLIENT_H
#define ABSTRACTIRCCLIENT_H

#include <QObject>
#include <QAbstractSocket>

#include "helpers/util.h"

class ServerSettings;

// This class abstracts away the actual IRC client implementations from
// the model layer of the application. It contains code that is common
// to all IRC client implementations and handles communication between the model
// layer and the actual IRC client implementation.

class AbstractIrcClient : public QObject
{
    Q_OBJECT

public:
    explicit AbstractIrcClient(QObject *parent, ServerSettings *serverSettings);
    
signals:
    // Implementations of this class SHOULD emit these signals when appropriate.
    void connectedToServer();
    void disconnectedFromServer();
    void socketErrorHappened(QAbstractSocket::SocketError error);

    // Messages corresponding to a single channel.
    void receiveUserNames(const QString &channelName, const QStringList &userNames);
    void receiveMessage(const QString &channelName, const QString &userName, const QString &message);
    void receiveCtcpRequest(const QString &userName, const QString &message);
    void receiveCtcpReply(const QString &userName, const QString &message);
    void receiveCtcpAction(const QString &channelName, const QString &userName, const QString &message);
    void receivePart(const QString &channelName, const QString &userName, const QString &message);
    void receiveJoin(const QString &channelName, const QString &userName);
    void receiveTopic(const QString &channelName, const QString &topic);
    void receiveKick(const QString &channelName, const QString &userName, const QString &kickedUserName, const QString &message);
    void receiveModeChange(const QString &channelName, const QString &mode, const QString &arguments);

    // Messages corresponding to the server itself.
    void receiveQuit(const QString &userName, const QString &message);
    void receiveNickChange(const QString &oldNick, const QString &newNick);
    void receiveMotd(const QString &motd);
    void receiveError(const QString &error);
    void joinedChannel(const QString &channelName);
    void queriedUser(const QString &channelName);
    void partedChannel(const QString &channelName);
    void closedUser(const QString &channelName);
    
public:
    // Implementations of this class SHOULD implement all the methods below.

    virtual const QString currentNick() = 0;
    virtual void connectToServer() = 0;
    virtual void disconnectFromServer() = 0;

    virtual void quit(const QString &message) = 0;
    virtual void joinChannel(const QString &channelName, const QString &channelKey) = 0;
    virtual void partChannel(const QString &channelName, const QString &reason) = 0;
    virtual void queryUser(const QString &userName) = 0;
    virtual void closeUser(const QString &userName) = 0;
    virtual void sendCtcpAction(const QString &channelName, const QString &action) = 0;
    virtual void sendCtcpRequest(const QString &userName, const QString &request) = 0;
    virtual void sendCtcpReply(const QString &userName, const QString &message) = 0;
    virtual void sendMessage(const QString &channelName, const QString &message) = 0;
    virtual void requestTopic(const QString &channelName) = 0;
    virtual void changeNick(const QString &newNick) = 0;
    virtual void kick(const QString &channelName, const QString &userName, const QString &message) = 0;
    virtual void sendRaw(const QString &message) = 0;
    virtual void sendWhois(const QString userName) = 0;

    virtual QAbstractSocket *socket() = 0;

};

#endif // ABSTRACTIRCCLIENT_H
