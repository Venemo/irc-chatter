
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
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QObject>
#include <QStringListModel>

#include "util.h"

class CommandParser;
class AbstractIrcClient;
class ServerModel;

class ChannelModel : public QObject
{
private:
    Q_OBJECT

    GENPROPERTY_R(QString, _name, name)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    GENPROPERTY_R(QString, _currentMessage, currentMessage)
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    GENPROPERTY_PTR_R(QStringListModel*, _users, users)
    Q_PROPERTY(QObject* users READ users NOTIFY usersChanged)
    Q_PROPERTY(int userCount READ userCount NOTIFY usersChanged)
    Q_PROPERTY(QObject* server READ parent NOTIFY serverChanged)
    GENPROPERTY_F(QString, _channelText, channelText, setChannelText, channelTextChanged)
    Q_PROPERTY(QString channelText READ channelText NOTIFY channelTextChanged)
    GENPROPERTY_R(QString, _topic, topic)
    Q_PROPERTY(QString topic READ topic NOTIFY topicChanged)
    GENPROPERTY_R(unsigned, _channelType, channelType)

    AbstractIrcClient *_ircClient;
    CommandParser *_commandParser;

    QString _completionFragment, _sentMessagesTemp;
    QStringList _soFarReceivedUserNames, _sentMessages;
    QList<const QString*> _possibleNickNames;
    int _currentCompletionIndex, _currentCompletionPosition, _displayedLines, _sentMessagesIndex;

    static QString _autoCompletionSuffix;
    static QRegExp _urlRegexp;
    static int _maxLineNumber, _deletableLines;

    friend class IrcModel;
    friend class ServerModel;

protected:
    explicit ChannelModel(ServerModel *parent, const QString &channelName, AbstractIrcClient *ircClient);

    void adjustForSentMessagesIndex();
    void parseCommand(const QString &msg);
    QString &processMessage(QString &msg, bool *hasUserNick = 0);

    void appendLine(const QString &line);
    void appendEmphasisedInfo(QString msg);
    void appendDeemphasisedInfo(QString msg);
    void appendError(QString msg);

    void receiveMessage(const QString &userName, QString message);
    void receiveCtcpAction(const QString &userName, QString message);
    void receiveCtcpRequest(const QString &userName, QString message);
    void receiveCtcpReply(const QString &userName, QString message);
    void receiveJoined(const QString &userName);
    void receiveParted(const QString &userName, QString reason);
    void receiveQuit(const QString &userName, QString reason);
    void receiveNickChange(const QString &oldNick, const QString &newNick);
    void receiveMotd(QString motd);
    void receiveInvite(const QString &origin, const QString &receiver);
    void receiveKicked(const QString &origin, const QString &nick, QString message);
    void receiveTopic(const QString &value);
    void receiveModeChange(const QString &mode, const QString &argument);
    void channelNameChanged(const QString &newName);

public:
    ~ChannelModel();
    int userCount() { return _users->rowCount(); }
    void setCurrentMessage(const QString &value);

    Q_INVOKABLE void autoCompleteNick();
    Q_INVOKABLE QString getUserNameFromIndex(int index) const;
    Q_INVOKABLE void getSentMessagesUp();
    Q_INVOKABLE void getSentMessagesDown();

    enum ChannelType
    {
        Server = 0,
        Channel = 1,
        Query = 2
    };

signals:
    void nameChanged();
    void currentMessageChanged();
    void usersChanged();
    void serverChanged();
    void channelTextChanged();
    void topicChanged();

    void newMessageReceived();
    void newMessageWithUserNickReceived();

public slots:
    void sendCurrentMessage();
    void updateUserList();

};

#endif // CHANNELMODEL_H
