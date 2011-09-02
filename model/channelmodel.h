
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
#include "qobjectlistmodel.h"

namespace Irc { class Buffer; }
class ServerModel;

class ChannelModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    GENPROPERTY_R(QString, _currentMessage, currentMessage)
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    GENPROPERTY_R(QStringListModel*, _users, users)
    Q_PROPERTY(QObject* users READ users NOTIFY usersChanged)
    Q_PROPERTY(int userCount READ userCount NOTIFY usersChanged)
    Q_PROPERTY(QObject* server READ parent NOTIFY serverChanged)
    GENPROPERTY(QString, _channelText, channelText, setChannelText, channelTextChanged)
    Q_PROPERTY(QString channelText READ channelText NOTIFY channelTextChanged)
    GENPROPERTY_R(QString, _topic, topic)
    Q_PROPERTY(QString topic READ topic NOTIFY topicChanged)

    static QString _autoCompletionSuffix;
    QString _completionFragment;
    QList<const QString*> _possibleNickNames;
    int _currentCompletionIndex, _currentCompletionPosition, _displayedLines;

    Irc::Buffer *_backend;

    static QList<QString> *_colors;
    static QRegExp _urlRegexp;
    static int _maxLineNumber, _deletableLines;

    friend class IrcModel;
    friend class ServerModel;

protected:
    explicit ChannelModel(ServerModel *parent, Irc::Buffer *backend);
    void setTopic(const QString &value);

    void parseCommand(const QString &msg);
    QString &processMessage(QString &msg, bool *hasUserNick = 0);

    void appendLine(const QString &line);
    void appendEmphasisedInfo(QString msg);
    void appendDeemphasisedInfo(QString msg);
    void appendError(QString msg);

public:
    ~ChannelModel();
    int userCount() { return _users->rowCount(); }
    void setCurrentMessage(const QString &value);

    Q_INVOKABLE void autoCompleteNick();
    Q_INVOKABLE const QString colorForNick(const QString &nick);
    Q_INVOKABLE void queryUser(const quint16 &index);
    QString name() const;

signals:
    void nameChanged();
    void currentMessageChanged();
    void usersChanged();
    void serverChanged();
    void channelTextChanged();
    void topicChanged();

    void newMessageReceived();
    void newMessageWithUserNickReceived();

private slots:
    void fakeMessage();
    void backendDeleted();
    void receiveMessageFromBackend(const QString &userName, QString message);
    void receiveNoticeFromBackend(const QString &userName, QString message);
    void receiveCtcpActionFromBackend(const QString &userName, QString message);
    void receiveCtcpRequestFromBackend(const QString &userName, QString message);
    void receiveCtcpReplyFromBackend(const QString &userName, QString message);
    void receiveUnknownMessageFromBackend(const QString &userName, const QStringList &message);
    void receiveJoinedFromBackend(const QString &userName);
    void receivePartedFromBackend(const QString &userName, QString reason);
    void receiveQuitFromBackend(const QString &userName, QString reason);
    void receiveNickChangeFromBackend(const QString &oldNick, const QString &newNick);
    void receiveMotdFromBackend(QString motd);
    void receiveInviteFromBackend(const QString &origin, const QString &receiver, const QString &channel);
    void receiveKickedFromBackend(const QString &origin, const QString &nick, QString message);
    void channelNameChanged(const QString &newName);

public slots:
    void sendCurrentMessage();
    void updateUserList();

};

#endif // CHANNELMODEL_H
