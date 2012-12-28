
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

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

#include "helpers/commandparser.h"
#include "clients/abstractircclient.h"
#include "model/settings/appsettings.h"

CommandParser::CommandParser(QObject *parent, AbstractIrcClient *ircClient, AppSettings *appSettings) :
    QObject(parent),
    _ircClient(ircClient),
    _appSettings(appSettings)
{
}

void CommandParser::parseAndSendCommand(const QString &channelName, const QString &msg)
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

            // TODO: add possibility to store channel keys in the autojoin
            _ircClient->joinChannel(commandParts[1], QString());
        }
        else
        {
            emit commandParseError("Invalid command. Correct usage: '/join &lt;channelname&gt;'");
        }
    }
    else if (channelName.startsWith('#') && (commandParts[0] == "/part" || commandParts[0] == "/p"))
    {
        if (n == 1)
            _ircClient->partChannel(channelName, _appSettings->partMessage());
        else if (n == 2)
            _ircClient->partChannel(commandParts[1], commandParts[2]);
        else
            emit commandParseError("Invalid command. Correct usage: '/part' or '/part &lt;channelname&gt'';");
    }
    else if (!channelName.startsWith('#') && commandParts[0] == "/close")
    {
        if (n == 1)
            _ircClient->closeUser(channelName);
        else if (n == 2)
            _ircClient->closeUser(commandParts[1]);
        else
            emit commandParseError("Invalid command. Correct usage: '/close' or '/close &lt;username&gt'';");
    }
    else if (commandParts[0] == "/quit")
    {
        if (n == 1)
        {
            _ircClient->quit(_appSettings->quitMessage());
            _ircClient->disconnectFromServer();
            QCoreApplication::instance()->processEvents();
            QCoreApplication::instance()->quit();
        }
        else
            emit commandParseError("Invalid command. Correct usage: '/quit'");
    }
    else if (commandParts[0] == "/me")
    {
        if (n > 1)
        {
            QString action = msg.mid(4);
            _ircClient->sendCtcpAction(channelName, action);
        }
        else
            emit commandParseError("Invalid command. Correct usage: '/me &lt;message&gt;'");
    }
    else if (commandParts[0] == "/msg" || commandParts[0] == "/m")
    {
        if (n > 2)
        {
            int length = commandParts[0].length() + commandParts[1].length();
            QString message = msg.mid(length + 2);
            _ircClient->queryUser(commandParts[1]);
            _ircClient->sendMessage(commandParts[1], message);
        }
        else
            emit commandParseError("Invalid command. Correct usage: '/msg &lt;username&gt; &lt;message&gt;'");
    }
    else if (commandParts[0] == "/nick" || commandParts[0] == "/n")
    {
        if (n == 2)
            _ircClient->changeNick(commandParts[1]);
        else
            emit commandParseError("Invalid command. Correct usage: '/nick &lt;new nick&gt;' ");
    }
    else if (commandParts[0] == "/topic")
    {
        if (n == 1)
            _ircClient->requestTopic(channelName);
        else
            emit commandParseError("Changing Topics is not supported yet!");
    }
    else if (commandParts[0] == "/query" || commandParts[0] == "/q")
    {
        if (n == 2)
            _ircClient->queryUser(commandParts[1]);
        else
            emit commandParseError("Invalid command. Correct usage: '/query &lt;username&gt;' ");
    }
    else if (commandParts[0] == "/kick" || commandParts[0] == "/k")
    {
        if (n == 2)
            _ircClient->kick(channelName, commandParts[1], _appSettings->kickMessage());
        else if (n == 3)
            _ircClient->kick(commandParts[2], commandParts[1], _appSettings->kickMessage());
        else if (n != 1)
        {
            QString reason = msg.mid(commandParts[0].length() + commandParts[1].length() + commandParts[2].length() + 3);
            _ircClient->kick(commandParts[2], commandParts[1], reason);
        }
        else
            emit commandParseError("Invalid command. Correct usage: '/kick &lt;nick&gt; [channel] [reason]'");
    }
    else if (commandParts[0] == "/ctcp")
    {
        if (n == 2)
            _ircClient->sendCtcpRequest(commandParts[1], QString("VERSION"));
        else if (n == 3)
            _ircClient->sendCtcpRequest(commandParts[1], commandParts[2]);
        else
            emit commandParseError("Invalid command. Correct usage: '/ctcp &lt;user name&gt; [request]'");
    }
    else if (commandParts[0] == "/raw")
    {
        QString rawCommand = msg.mid(5);
        emit commandParseError("Sending raw command '" + rawCommand + "' to the server.");
        _ircClient->sendRaw(rawCommand);
    }
    else if (commandParts[0] == "/whois")
    {
        if (n == 2)
            _ircClient->sendWhois(commandParts[1]);
        else
            emit commandParseError("Invalid command. Correct usage: '/whois &lt;user name&gt;'");
    }
    // TODO
//    else if (commandParts[0] == "/quote")
//    {
//        QString rawCommand = msg.mid(7);
//        emit commandParseError("Sending raw command '" + rawCommand + "' to the server.");
//        _backend->sendRaw(rawCommand);
//    }
    else
    {
        emit commandParseError("Sending raw command '" + msg + "' to the server.");
        _ircClient->sendRaw(msg);
    }
}
