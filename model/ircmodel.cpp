#include "ircmodel.h"
#include <ircclient-qt/IrcSession>

#include <QtCore>
#include <QStringList>
#include "appsettings.h"
#include <QSslSocket>

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
