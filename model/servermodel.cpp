#include "servermodel.h"
#include <QtCore>
#include <ircclient-qt/IrcSession>
#include <ircclient-qt/IrcBuffer>
#include <ircclient-qt/IrcGlobal>
#include <ircclient-qt/IrcUtil>

ServerModel::ServerModel(IrcModel *parent, const QString &url, Irc::Session *backend) :
    QObject((QObject*)parent),
    _channels(new QObjectListModel<ChannelModel>(this)),
    _url(url),
    _backend(backend)
{
    connect(_backend, SIGNAL(connected()), this, SLOT(backendConnectedToServer()));
    _backend->connectToServer(_url);
}

ServerModel::~ServerModel()
{
    _backend->quit("IRC Chatter (the first MeeGo IRC client) closed.");
}

void ServerModel::backendConnectedToServer()
{
    connect(_backend->defaultBuffer(), SIGNAL(numericMessageReceived(QString,uint,QStringList)), this, SLOT(receiveNumericMessageFromBackend(QString,uint,QStringList)));

    foreach (Irc::Buffer *buffer, _backend->buffers())
    {
        QString name = buffer->names().count() ? buffer->names().at(0) : "Unnamed";
        _channels->addItem(new ChannelModel(name, this, buffer));
    }
}

void ServerModel::receiveNumericMessageFromBackend(const QString &name, uint x, const QStringList &message)
{
    Q_UNUSED(name);
    Q_UNUSED(message);

    if (x == Irc::Rfc::RPL_ENDOFNAMES)
    {
        foreach (ChannelModel *channel, _channels->getList())
            channel->updateUserList();
    }
}

bool ServerModel::joinChannel(const QString &channelName)
{
    qDebug() << "joining channel " << channelName;

    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == channelName)
        {
            qDebug() << "Already connected to this channel";
        }
    }

    _backend->join(channelName);
    _backend->names(channelName);
    Irc::Buffer *b = _backend->addBuffer(channelName);
    _channels->addItem(new ChannelModel(channelName, this, b));

    return true;
}

bool ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel " << channelName;

    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == channelName)
        {
            _channels->removeItem(channel);
            break;
        }
    }

    _backend->part(channelName, "Leaving this channel. (with IRC Chatter, the first MeeGo IRC client)");

    return true;
}

bool ServerModel::queryUser(const QString &userName)
{
    qDebug() << "querying user " << userName;
    // TODO: query the user in the backend
    return true;
}

bool ServerModel::closeUser(const QString &userName)
{
    qDebug() << "closing user " << userName;
    // TODO
    return true;
}
