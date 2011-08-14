#include "ircmodel.h"
#include <ircclient-qt/IrcSession>
#include <QtCore>
#include <QStringList>

IrcModel::IrcModel(QObject *parent) :
    QObject(parent),
    _servers(new QObjectListModel<ServerModel>(this)),
    _currentChannelIndex(-1)
{
}

void IrcModel::connectToServer(const QString &url, const QString &password, const QString &nick, const QString &ident, const QString &fullName, const QString &autoJoin)
{
    Irc::Session *session = new Irc::Session();
    session->setNick(nick);
    session->setPassword(password);
    session->setIdent(ident);
    if (fullName != "")
        session->setRealName(fullName);
    QStringList autoJoinList = autoJoin.split(",");
    for (int i = 0; i < autoJoinList.count(); i++)
        autoJoinList[i] = autoJoinList[i].trimmed();
    session->setAutoJoinChannels(autoJoinList);
    _servers->addItem(new ServerModel(this, url, session));
    connect(session, SIGNAL(connected()), this, SLOT(backendsConnectedToServer()));
}

QObjectListModel<ChannelModel> *IrcModel::allChannels()
{
    // TODO: display all channels from all servers
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
