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
// Authors of code borrowed from Konversation (URL regex):
// Copyright (C) 2010 Eike Hein <hein@kde.org>

#include <QtCore>

#include "channelmodel.h"
#include "servermodel.h"
#include "ircmodel.h"
#include "clients/abstractircclient.h"
#include "helpers/commandparser.h"
#include "helpers/channelhelper.h"
#include "helpers/notifier.h"

QString ChannelModel::_autoCompletionSuffix(", ");
int ChannelModel::_maxLineNumber = 300;
int ChannelModel::_deletableLines = 100;

// This code is copypasted from Konversation - I hereby thank its authors
QRegExp ChannelModel::_urlRegexp(QString("\\b((?:(?:([a-z][\\w-]+:/{1,3})|www\\d{0,3}[.]|[a-z0-9.\\-]+[.][a-z]{2,4}/)(?:[^\\s()<>]+|\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\))+(?:\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\)|\\}\\]|[^\\s`!()\\[\\]{};:'\".,<>?%1%2%3%4%5%6])|[a-z0-9.\\-+_]+@[a-z0-9.\\-]+[.][a-z]{1,5}[^\\s/`!()\\[\\]{};:'\".,<>?%1%2%3%4%5%6]))").arg(QChar(0x00AB)).arg(QChar(0x00BB)).arg(QChar(0x201C)).arg(QChar(0x201D)).arg(QChar(0x2018)).arg(QChar(0x2019)));

ChannelModel::ChannelModel(ServerModel *parent, const QString &channelName, AbstractIrcClient *ircClient) :
    QObject(parent),
    _name(channelName),
    _users(new QStringListModel(this)),
    _ircClient(ircClient),
    _commandParser(new CommandParser(this, _ircClient, this->appSettings())),
    _displayedLines(0),
    _sentMessagesIndex(-1)
{
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
    if (appSettings()->displayMiscEvents())
    {
        if (userName != _ircClient->currentNick())
            appendDeemphasisedInfo("--> " + userName + " has joined this channel.");
    }

    _userNames.append(userName);
    updateUserList();
}

void ChannelModel::receiveParted(const QString &userName, QString reason)
{
    if (appSettings()->displayMiscEvents())
    {
        appendDeemphasisedInfo("<-- " + userName + " has parted this channel." + (reason.length() ? (" (Reason: " + reason + ")") : ""));
    }

    _userNames.removeAll(userName);
    updateUserList();
}

void ChannelModel::receiveQuit(const QString &userName, QString reason)
{
    if (appSettings()->displayMiscEvents())
    {
        appendDeemphasisedInfo("<-- " + userName + " has left this server." + (reason.length() ? (" (Reason: " + reason + ")") : ""));
    }

    _userNames.removeAll(userName);
    updateUserList();
}

void ChannelModel::receiveNickChange(const QString &oldNick, const QString &newNick)
{
    if (appSettings()->displayMiscEvents())
    {
        appendDeemphasisedInfo("*** " + oldNick + " has changed nick to " + newNick + ".");
    }

    _userNames.removeAll(oldNick);
    _userNames.append(newNick);
    updateUserList();
}

void ChannelModel::receiveInvite(const QString &origin, const QString &receiver)
{
    appendEmphasisedInfo("*** " + origin + " has invited " + receiver + " to " + _name + ".");
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

    if (msg.contains(_ircClient->currentNick()))
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
    QString line;

    if (appSettings()->displayTimestamps())
        line += QTime::currentTime().toString("HH:mm") + " ";

    line += "<a href='user://" + userName +"' style='text-decoration: none; color: " + ChannelHelper::colorForNick(userName, _ircClient->currentNick()) + "'>" + userName + "</a>: " + processMessage(message, &hasUserNick);

    appendLine(line);

    if (!static_cast<IrcModel*>(parent()->parent())->isAppInFocus()
            && ((hasUserNick && appSettings()->notifyOnNick())
            || (!_name.startsWith('#') && appSettings()->notifyOnPrivmsg())))
    {

        Notifier::notify(message);
    }

    if (hasUserNick || !name().startsWith('#'))
        emit newMessageWithUserNickReceived();
    else
        emit newMessageReceived();
}

void ChannelModel::receiveCtcpAction(const QString &userName, QString message)
{
    bool hasUserNick = false;
    QString line;

    if (appSettings()->displayTimestamps())
        line += QTime::currentTime().toString("HH:mm") + " ";

    line += "* <span style='color: " + ChannelHelper::colorForNick(userName, _ircClient->currentNick()) + "'>" + userName + "</span> " + processMessage(message, &hasUserNick);

    appendLine(line);

    if (!static_cast<IrcModel*>(parent()->parent())->isAppInFocus()
            && ((hasUserNick && appSettings()->notifyOnNick())
            || (!_name.startsWith('#') && appSettings()->notifyOnPrivmsg())))
    {

        Notifier::notify(message);
    }

    if (hasUserNick || !name().startsWith('#'))
        emit newMessageWithUserNickReceived();
    else
        emit newMessageReceived();
}

void ChannelModel::receiveCtcpRequest(const QString &userName, QString message)
{
    qDebug() << "CTCP request received " << userName << message;
    // TODO
    // _backend->sendCommand(IrcCommand::createCtcpReply(userName, "IRC Chatter, the first MeeGo IRC client"));
}

void ChannelModel::receiveCtcpReply(const QString &userName, QString message)
{
    qDebug() << "CTCP reply received " << userName << message;
    appendEmphasisedInfo("CTCP Reply from " + userName + ": " + message);
}

void ChannelModel::receiveTopic(const QString &value)
{
    _topic = value;
    appendEmphasisedInfo("[TOPIC] " + _topic);
    emit topicChanged();
}

void ChannelModel::receiveModeChange(const QString &mode, const QString &argument)
{
    appendEmphasisedInfo(QString("Channel mode is ") + mode + QString(", argument is ") + argument);
}

void ChannelModel::receiveUserList(const QStringList &userList)
{
    _userNames = userList;
    updateUserList();
}

void ChannelModel::updateUserList()
{
    _users->setStringList(_userNames);
}

void ChannelModel::sendCurrentMessage()
{
    if (_currentMessage.length() > 0)
    {
        if (_currentMessage.startsWith("/"))
        {
            _commandParser->parseAndSendCommand(_name, _currentMessage);
        }
        else
        {
            _ircClient->sendMessage(_name, _currentMessage);
        }

        _sentMessages.insert(0, _currentMessage);
        _sentMessagesIndex = -1;
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

QString ChannelModel::getUserNameFromIndex(int index) const
{
    qDebug() << "heyheyhey";
    return _users->stringList().at(index);
}

void ChannelModel::getSentMessagesUp()
{
    if (_sentMessagesIndex == -1)
        _sentMessagesTemp = _currentMessage;

    if (_sentMessagesIndex < _sentMessages.count() - 1)
    {
        _sentMessagesIndex++;
        adjustForSentMessagesIndex();
    }
}

void ChannelModel::getSentMessagesDown()
{
    if (_sentMessagesIndex > -1)
    {
        _sentMessagesIndex--;
        adjustForSentMessagesIndex();
    }
}

void ChannelModel::adjustForSentMessagesIndex()
{
    if (_sentMessagesIndex == -1)
    {
        setCurrentMessage(_sentMessagesTemp);
    }
    else
    {
        setCurrentMessage(_sentMessages[_sentMessagesIndex]);
    }
}

AppSettings *ChannelModel::appSettings()
{
    return static_cast<IrcModel*>(parent()->parent())->appSettings();
}
