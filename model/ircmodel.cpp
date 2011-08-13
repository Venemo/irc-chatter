#include "ircmodel.h"
#include <ircclient-qt/IrcSession>

IrcModel::IrcModel(QObject *parent) :
    QObject(parent),
    _servers(new QObjectListModel<ServerModel>(this)),
    _currentChannelIndex(-1)
{
}

void IrcModel::connectToServer(const QString &url, const QString &nick)
{
    Irc::Session *session = new Irc::Session();
    session->setNick(nick);
    _servers->addItem(new ServerModel(this, url, session));
    connect(session, SIGNAL(connected()), this, SLOT(backendsConnectedToServer()));
}

QObjectListModel<ChannelModel> *IrcModel::allChannels()
{
    // TODO: display all channels from all servers
    if (_servers->rowCount())
        return ((ServerModel*)_servers->getItem(NULL))->channels();

    return NULL;
}

void IrcModel::backendsConnectedToServer()
{
    if (_currentChannelIndex == -1)
        setCurrentChannelIndex(0);
}
