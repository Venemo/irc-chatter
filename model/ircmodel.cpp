#include "ircmodel.h"
#include <ircclient-qt/IrcSession>

IrcModel::IrcModel(QObject *parent) :
    QObject(parent),
    _servers(new QObjectListModel<ServerModel>(this)),
    _currentChannelIndex(0)
{
}

void IrcModel::connectToServer(const QString &url, const QString &nick)
{
    Irc::Session *session = new Irc::Session();
    session->setNick(nick);
    _servers->addItem(new ServerModel(this, url, session));
}

QObjectListModel<ChannelModel> *IrcModel::allChannels()
{
    // TODO: display all channels from all servers
    if (_servers->rowCount())
        return ((ServerModel*)_servers->getItem(NULL))->channels();

    return NULL;
}

void IrcModel::fillWithDummyData()
{
    ServerModel *server = new ServerModel(this, "chat.freenode.net", 0);

    ChannelModel *channel = new ChannelModel("#maemo", server, NULL);
    QStringList *userlist = new QStringList();
    userlist->append("Venemo");
    userlist->append("MohammadAG");
    userlist->append("alterego");
    channel->_users->setStringList(*userlist);
    server->_channels->addItem(channel);

    channel = new ChannelModel("#meego", server, NULL);
    userlist = new QStringList();
    userlist->append("Stskeeps");
    userlist->append("lcuk");
    userlist->append("wNULLNULLt_");
    channel->_users->setStringList(*userlist);
    server->_channels->addItem(channel);

    channel = new ChannelModel("#harmattan", server, NULL);
    userlist = new QStringList();
    userlist->append("djszapi");
    userlist->append("hiemanshu");
    userlist->append("DocScrutinizer");
    userlist->append("fiferboy");
    channel->_users->setStringList(*userlist);
    server->_channels->addItem(channel);

    server->_channels->addItem(new ChannelModel("#meego-bar", server, NULL));
    server->_channels->addItem(new ChannelModel("#qt", server, NULL));
    server->_channels->addItem(new ChannelModel("#qt-creator", server, NULL));
    server->_channels->addItem(new ChannelModel("#qt-qml", server, NULL));
    server->_channels->addItem(new ChannelModel("MohammadAG", server, NULL));
    server->_channels->addItem(new ChannelModel("wNULLNULLt_", server, NULL));

    _servers->addItem(server);
}
