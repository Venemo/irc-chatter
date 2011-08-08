#include "ircmodel.h"

IrcModel::IrcModel(QObject *parent) :
    QObject(parent),
    _servers(new QObjectListModel<ServerModel>(this)),
    _currentChannelIndex(-1)
{
}

void IrcModel::fillWithDummyData()
{
    ServerModel *server = new ServerModel(this);

    ChannelModel *channel = new ChannelModel("#maemo", NULL, NULL);
    QStringList *userlist = new QStringList();
    userlist->append("Venemo");
    userlist->append("MohammadAG");
    userlist->append("alterego");
    channel->_users->setStringList(*userlist);
    server->_channels->addItem(channel);

    channel = new ChannelModel("#meego", NULL, NULL);
    userlist = new QStringList();
    userlist->append("Stskeeps");
    userlist->append("lcuk");
    userlist->append("wNULLNULLt_");
    channel->_users->setStringList(*userlist);
    server->_channels->addItem(channel);

    channel = new ChannelModel("#harmattan", NULL, NULL);
    userlist = new QStringList();
    userlist->append("djszapi");
    userlist->append("hiemanshu");
    userlist->append("DocScrutinizer");
    userlist->append("fiferboy");
    channel->_users->setStringList(*userlist);
    server->_channels->addItem(channel);

    server->_channels->addItem(new ChannelModel("#meego-bar", NULL, NULL));
    server->_channels->addItem(new ChannelModel("#qt", NULL, NULL));
    server->_channels->addItem(new ChannelModel("#qt-creator", NULL, NULL));
    server->_channels->addItem(new ChannelModel("#qt-qml", NULL, NULL));
    server->_channels->addItem(new ChannelModel("MohammadAG", NULL, NULL));
    server->_channels->addItem(new ChannelModel("wNULLNULLt_", NULL, NULL));

    _servers->addItem(server);
}

QObjectListModel<ChannelModel> *IrcModel::allChannels()
{
    // TODO: display all channels from all servers
    if (_servers->rowCount())
        return ((ServerModel*)_servers->getItem(NULL))->channels();

    return NULL;
}
