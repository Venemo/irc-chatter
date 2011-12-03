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
//
// Authors of code borrowed from Konversation (URL regex and colorForNick method):
// Copyright (C) 2010 Eike Hein <hein@kde.org>
// Copyright (C) 2006 Michael Kreitzer <mrgrim@gr1m.org>

#include <QtCore>
#include <IrcSession>
#include <IrcSender>
#include <IrcUtil>
#include <IrcGlobal>
#include <IrcMessage>
#include <IrcCommand>

#include "channelmodel.h"
#include "servermodel.h"

QList<QString> *ChannelModel::_colors = 0;
QString ChannelModel::_autoCompletionSuffix(", ");
int ChannelModel::_maxLineNumber = 300;
int ChannelModel::_deletableLines = 100;

// This code is copypasted from Konversation - I hereby thank its authors
QRegExp ChannelModel::_urlRegexp(QString("\\b((?:(?:([a-z][\\w-]+:/{1,3})|www\\d{0,3}[.]|[a-z0-9.\\-]+[.][a-z]{2,4}/)(?:[^\\s()<>]+|\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\))+(?:\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\)|\\}\\]|[^\\s`!()\\[\\]{};:'\".,<>?%1%2%3%4%5%6])|[a-z0-9.\\-+_]+@[a-z0-9.\\-]+[.][a-z]{1,5}[^\\s/`!()\\[\\]{};:'\".,<>?%1%2%3%4%5%6]))").arg(QChar(0x00AB)).arg(QChar(0x00BB)).arg(QChar(0x201C)).arg(QChar(0x201D)).arg(QChar(0x2018)).arg(QChar(0x2019)));

ChannelModel::ChannelModel(ServerModel *parent, const QString &name, IrcSession *backend) :
    QObject(parent),
    _name(name),
    _users(new QStringListModel(this)),
    _displayedLines(0),
    _backend(backend),
    _sentMessagesCount(0)
{
    if (!_colors)
    {
        _colors = new QList<QString>();
        // Red
        _colors->append("#ff0000");
        // Green
        _colors->append("#00ff00");
        // Blue
        _colors->append("#0000ff");
        // Dark purple
        _colors->append("#400758");
        // Dark green
        _colors->append("#0c5807");
        // Dark orange
        _colors->append("#ba770e");
        // Yellowish green (my favourite colour)
        _colors->append("#73ba0e");
        // Yellow
        _colors->append("#cec700");
    }
}

ChannelModel::~ChannelModel()
{
}

void ChannelModel::channelNameChanged(const QString &newName)
{
    appendDeemphasisedInfo("Channel name is changed to " + newName);
    emit nameChanged();
}

void ChannelModel::receiveMotd(QString motd)
{
    appendDeemphasisedInfo("[MOTD] " + motd);
}

void ChannelModel::receiveJoined(const QString &userName)
{
    if (userName != _backend->nickName())
        appendDeemphasisedInfo("--> " + userName + " has joined this channel.");

    _soFarReceivedUserNames.append(userName);
    updateUserList();
}

void ChannelModel::receiveParted(const QString &userName, QString reason)
{
    appendDeemphasisedInfo("<-- " + userName + " has parted this channel." + (reason.length() ? (" (Reason: " + reason + ")") : ""));
    _soFarReceivedUserNames.removeAll(userName);
    updateUserList();
}

void ChannelModel::receiveQuit(const QString &userName, QString reason)
{
    appendDeemphasisedInfo("<-- " + userName + " has left this server." + (reason.length() ? (" (Reason: " + reason + ")") : ""));
    _soFarReceivedUserNames.removeAll(userName);
    updateUserList();
}

void ChannelModel::receiveNickChange(const QString &oldNick, const QString &newNick)
{
    appendDeemphasisedInfo("*** " + oldNick + " has changed nick to " + newNick + ".");
    _soFarReceivedUserNames.removeAll(oldNick);
    _soFarReceivedUserNames.append(newNick);
    updateUserList();
}

void ChannelModel::receiveInvite(const QString &origin, const QString &receiver, const QString &channel)
{
    appendEmphasisedInfo("*** " + origin + " has invited " + receiver + " to " + channel + ".");
}

void ChannelModel::receiveKicked(const QString &origin, const QString &nick, QString message)
{
    appendEmphasisedInfo("*** " + origin + " has kicked " + nick + " with message '" + message + "'.");
}

QString &ChannelModel::processMessage(QString &msg, bool *hasUserNick)
{
    msg.replace('&', "&amp;");
    msg.replace('<', "&lt;");
    msg.replace('>', "&gt;");
    msg.replace('\n', "<br />");
    msg.replace(_urlRegexp, "<a href=\"\\1\">\\1</a>");

    if (msg.contains(_backend->nickName()))
    {
        msg = "<span style='color:red'>" + msg + "</span>";
        if (hasUserNick)
            *hasUserNick = true;
    }

    return msg;
}

void ChannelModel::appendLine(const QString &line)
{
    if (_channelText.length())
        _channelText += "<br />";

    if (_displayedLines > _maxLineNumber)
    {
        int position = 0;

        // Find the Nth line
        for (int i = 0; i < _deletableLines; i++)
            position = _channelText.indexOf("<br />", position);

        _channelText = _channelText.remove(0, position + 6);
        _displayedLines -= _deletableLines;
    }

    setChannelText(_channelText += line);
    _displayedLines++;
}

void ChannelModel::appendEmphasisedInfo(QString msg)
{
    appendLine("<span style='color: orange'>" + processMessage(msg) + "</span>");
}

void ChannelModel::appendDeemphasisedInfo(QString msg)
{
    appendLine("<span style='color: purple'>" + processMessage(msg) + "</span>");
}

void ChannelModel::appendError(QString msg)
{
    appendLine("<span style='color: red'>[ERROR] " + processMessage(msg) + "</span>");
}

void ChannelModel::receiveMessage(const QString &userName, QString message)
{
    bool hasUserNick = false;
    appendLine(QTime::currentTime().toString("HH:mm") + " <a href='user://" + userName +"' style='text-decoration: none; color: " + colorForNick(userName) + "'>" + userName + "</a>: " + processMessage(message, &hasUserNick));

    if (hasUserNick || !name().startsWith('#'))
        emit newMessageWithUserNickReceived();
    else
        emit newMessageReceived();
}

void ChannelModel::receiveCtcpAction(const QString &userName, QString message)
{
    appendLine(QTime::currentTime().toString("HH:mm") + " * <span style='color: " + colorForNick(userName) + "'>" + userName + "</span> " + processMessage(message));
}

void ChannelModel::receiveCtcpRequest(const QString &userName, QString message)
{
    qDebug() << "CTCP request received " << userName << message;
    _backend->sendCommand(IrcCommand::createCtcpReply(userName, "IRC Chatter, the first MeeGo IRC client"));
}

void ChannelModel::receiveCtcpReply(const QString &userName, QString message)
{
    qDebug() << "CTCP reply received " << userName << message;
    appendEmphasisedInfo("CTCP Reply from " + userName + ": " + message);
}

void ChannelModel::updateUserList()
{
    _soFarReceivedUserNames = _soFarReceivedUserNames.toSet().toList();
    // workaround for bug: https://bugreports.qt.nokia.com/browse/QTBUG-12892
    // found in http://www.harshj.com/2009/10/24/sorting-entries-in-a-qstringlist-case-insensitively/
    // - Thank you!
    QMap<QString, QString> strMap;
    foreach (const QString &str, _soFarReceivedUserNames)
    {
        strMap.insert(str.toLower(), str);
    }
    _soFarReceivedUserNames = strMap.values();
    _users->setStringList(_soFarReceivedUserNames);
}

void ChannelModel::receiveTopic(const QString &value)
{
    _topic = value;
    appendEmphasisedInfo("[TOPIC] " + _topic);
    emit topicChanged();
}

// This code is copypasted from Konversation (and modified by Timur Kristóf) - I hereby thank its authors
const QString ChannelModel::colorForNick(const QString &nick)
{
    if (nick == _backend->nickName())
        return "#000000";

    int nickvalue = 0;

    for (int index = 0; index < nick.length(); index++)
        nickvalue += nick[index].unicode();

    return (*_colors)[nickvalue % _colors->count()];
}

void ChannelModel::sendCurrentMessage()
{
    if (_currentMessage.length() > 0)
    {
        _sentMessages.append(_currentMessage);
        _sentMessagesCount = _sentMessages.count();

        if (_currentMessage.startsWith("/"))
        {
            parseCommand(_currentMessage);
        }
        else
        {
            receiveMessage(_backend->nickName(), _currentMessage);
            _backend->sendCommand(IrcCommand::createMessage(_name, _currentMessage));
        }

        setCurrentMessage(QString());
    }
}

void ChannelModel::setCurrentMessage(const QString &value)
{
    if (_completionFragment.length())
        _completionFragment.clear();

    _currentMessage = value;
    emit currentMessageChanged();
}

void ChannelModel::autoCompleteNick()
{
    QString newFragment;

    if (_completionFragment.length())
    {
        // The user already started cycling through the list of possible nicks, give the next one

        if (_possibleNickNames.count() <= 1)
            return;

        _currentCompletionIndex ++;

        if (_currentCompletionIndex >= _possibleNickNames.count())
            _currentCompletionIndex = 0;
    }
    else if (_currentMessage[_currentMessage.length() - 1] == ' ')
        return;
    else
    {
        // The user already has a message and is trying to start autocompleting a name

        if (_currentMessage.contains(' '))
        {
            _currentCompletionPosition = _currentMessage.lastIndexOf(' ') + 1;
            _completionFragment = _currentMessage.mid(_currentCompletionPosition);
        }
        else
        {
            _currentCompletionPosition = 0;
            _completionFragment = _currentMessage;
        }

        _possibleNickNames.clear();
        _currentCompletionIndex = 0;

        foreach (const QString &nick, _users->stringList())
            if (nick.startsWith(_completionFragment, Qt::CaseInsensitive))
                _possibleNickNames.append(&nick);

        if (!_possibleNickNames.count())
            return;
    }

    newFragment = *_possibleNickNames[_currentCompletionIndex];
    if (_currentCompletionPosition == 0)
        newFragment += _autoCompletionSuffix;

    _currentMessage.replace(_currentCompletionPosition, _completionFragment.length(), newFragment);
    emit currentMessageChanged();
    _completionFragment = newFragment;
}

void ChannelModel::fakeMessage()
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += QTime::currentTime().toString("HH:mm") + " <span style='color: " + colorForNick("Zvdegor") + "'>" + "Zvdegor" + "</span>: " + QUuid::createUuid().toString());
}

void ChannelModel::parseCommand(const QString &msg)
{
    QStringList commandParts = msg.split(' ', QString::SkipEmptyParts);
    int n = commandParts.count();

    if (commandParts[0] == "/join" || commandParts[0] == "/j")
    {
        if (n == 2)
        {
            // Allow the user to spare the '#' character which is handy for VKB
            if (!commandParts[1].startsWith('#'))
                commandParts[1].insert(0, '#');

            static_cast<ServerModel*>(parent())->joinChannel(commandParts[1]);
        }
        else
        {
            appendEmphasisedInfo("Invalid command. Correct usage: '/join &lt;channelname&gt;'");
        }
    }
    else if (name().startsWith('#') && (commandParts[0] == "/part" || commandParts[0] == "/p"))
    {
        if (n == 1)
            static_cast<ServerModel*>(parent())->partChannel(name());
        else if (n == 2)
            static_cast<ServerModel*>(parent())->partChannel(commandParts[1]);
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/part' or '/part &lt;channelname&gt'';");
    }
    else if (!name().startsWith('#') && commandParts[0] == "/close")
    {
        if (n == 1)
            static_cast<ServerModel*>(parent())->closeUser(name());
        else if (n == 2)
            static_cast<ServerModel*>(parent())->closeUser(commandParts[1]);
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/close' or '/close &lt;username&gt'';");
    }
    else if (commandParts[0] == "/quit")
    {
        if (n == 1)
            QCoreApplication::instance()->quit();
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/quit'");
    }
    else if (commandParts[0] == "/me")
    {
        if (n > 1)
            _backend->sendCommand(IrcCommand::createCtcpAction(_name, msg.mid(4)));
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/me &lt;message&gt;'");
    }
    else if (commandParts[0] == "/msg" || commandParts[0] == "/m")
    {
        if (n > 2)
        {
            quint16 length = commandParts[0].length() + commandParts[1].length();
            static_cast<ServerModel*>(parent())->msgUser(commandParts[1], msg.mid(length+1));
        }
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/msg &lt;username&gt; &lt;message&gt;'");
    }
    else if (commandParts[0] == "/nick" || commandParts[0] == "/n")
    {
        if (n == 2)
            static_cast<ServerModel*>(parent())->changeNick(commandParts[1]);
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/nick &lt;new nick&gt;' ");
    }
    else if (commandParts[0] == "/topic")
    {
        if (n == 1)
            appendEmphasisedInfo("[TOPIC] " + _topic);
        else
            appendEmphasisedInfo("Changing Topics is not supported yet!");
    }
    else if (commandParts[0] == "/query" || commandParts[0] == "/q")
    {
        if (n == 2)
            static_cast<ServerModel*>(parent())->queryUser(commandParts[1]);
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/query &lt;username&gt;' ");
    }
    else if (commandParts[0] == "/kick" || commandParts[0] == "/k")
    {
        if (n == 2)
            static_cast<ServerModel*>(parent())->kickUser(commandParts[1], name());
        else if (n == 3)
            static_cast<ServerModel*>(parent())->kickUser(commandParts[1], commandParts[2]);
        else if (n != 1)
        {
            QString reason = msg.mid(commandParts[0].length() + commandParts[1].length() + commandParts[2].length() + 3);
            static_cast<ServerModel*>(parent())->kickUser(commandParts[1], commandParts[2], reason);
        }
        else
            appendEmphasisedInfo("Invalid command. Correct usage: '/kick &lt;nick&gt; [channel] [reason]'");
    }
    else
        appendEmphasisedInfo("Unknown command, maybe it will be supported later?");
}

void ChannelModel::queryUser(const quint16 &index)
{
    static_cast<ServerModel*>(parent())->queryUser(_users->stringList().at(index));
}

QString ChannelModel::getSentMessagesUp()
{
    if (_sentMessagesCount == -1) {
        return QString("");
    }
    else if (_sentMessagesCount != 0) {
        _sentMessagesCount -= 1;
        return QVariant(_sentMessages.at(_sentMessagesCount)).toString();
    }
    else {
        return QVariant(_sentMessages.at(0)).toString();
    }
}
QString ChannelModel::getSentMessagesDown()
{
    if (_sentMessagesCount == -1 || _sentMessagesCount == _sentMessages.count())
        return QString("");
    else if (_sentMessages.count() == _sentMessagesCount+1) {
        _sentMessagesCount += 1;
        return QString("");
    }
    else {
        _sentMessagesCount += 1;
        return QVariant(_sentMessages.at(_sentMessagesCount)).toString();
    }
}
