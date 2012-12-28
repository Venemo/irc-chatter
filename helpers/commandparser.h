
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

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QtCore/QObject>

class AbstractIrcClient;
class AppSettings;

class CommandParser : public QObject
{
    Q_OBJECT
    AbstractIrcClient *_ircClient;
    AppSettings *_appSettings;

public:
    explicit CommandParser(QObject *parent, AbstractIrcClient *ircClient, AppSettings *appSettings);
    void parseAndSendCommand(const QString &channelName, const QString &command);

signals:
    void commandParseError(const QString &error);
    
};

#endif // COMMANDPARSER_H
