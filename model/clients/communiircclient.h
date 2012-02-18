
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

#ifndef COMMUNIIRCCLIENT_H
#define COMMUNIIRCCLIENT_H

#include <QHash>
#include <QString>
#include <QStringList>
#include "abstractircclient.h"

class IrcSession;
class IrcMessage;
class IrcNumericMessage;

class CommuniIrcClient : public AbstractIrcClient
{
    Q_OBJECT
    IrcSession *_ircSession;
    QHash<QString, QStringList> _receivedUserNames;

    void processNumericMessage(IrcNumericMessage *message);

public:
    explicit CommuniIrcClient(const QString &serverUrl, QObject *parent, ServerSettings *serverSettings, AppSettings *appSettings);

private slots:
    void messageReceived(IrcMessage *message);
    
public slots:
    virtual void connectToServer();
    virtual void disconnectFromServer();

    virtual void quit(const QString &message);
    virtual void joinChannel(const QString &channelName);
    virtual void partChannel(const QString &channelName, const QString &message);
    virtual void queryUser(const QString &userName);
    virtual void closeUser(const QString &userName);
    virtual void sendCtcpAction(const QString &channelName, const QString &action);
    virtual void sendCtcpRequest(const QString &userName, const QString &request);
    virtual void sendCtcpReply(const QString &userName, const QString &message);
    virtual void sendMessage(const QString &channelName, const QString &message);
    virtual void requestTopic(const QString &channelName);
    virtual void changeNick(const QString &newNick);
    virtual void kick(const QString &channelName, const QString &userName, const QString &message);
    virtual void sendRaw(const QString &message);
    
};

#endif // COMMUNIIRCCLIENT_H
