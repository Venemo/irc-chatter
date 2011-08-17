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
#include <QStringList>
#include <QSslSocket>
#include <ircclient-qt/IrcSession>

#include "ircmodel.h"
#include "appsettings.h"

IrcModel::IrcModel(QObject *parent) :
    QObject(parent),
    _servers(new QObjectListModel<ServerModel>(this)),
    _currentChannelIndex(-1)
{
}

void IrcModel::connectToServer(ServerSettings *server, AppSettings *settings)
{
    Irc::Session *session = new Irc::Session();

    session->setNick(settings->userNickname());

    if (settings->userIdent().length())
        session->setIdent(settings->userIdent());
    else
        session->setIdent("ircchatter");

    if (settings->userRealName().length())
        session->setRealName(settings->userRealName());

    session->setPort(server->serverPort());

    if (server->serverSSL())
    {
        QSslSocket* socket = new QSslSocket(session);
        socket->ignoreSslErrors();
        socket->setPeerVerifyMode(QSslSocket::VerifyNone);
        session->setSocket(socket);
    }

    session->setAutoJoinChannels(server->autoJoinChannels());
    session->setPassword(server->serverPassword());

    _servers->addItem(new ServerModel(this, server->serverUrl(), session));
    connect(session, SIGNAL(connected()), this, SLOT(backendsConnectedToServer()));
}

QObjectListModel<ChannelModel> *IrcModel::allChannels()
{
    // TODO: display all channels from all servers (currently only one is able to be displayed)
    if (_servers->rowCount())
        return ((ServerModel*)_servers->getItem(0))->channels();

    return NULL;
}

void IrcModel::backendsConnectedToServer()
{
    if (_currentChannelIndex == -1)
    {
        qDebug() << "this is the first connection, and it succeeded";
        setCurrentChannelIndex(0);
        emit readyToDisplay();
        emit allChannelsChanged();
    }
}
