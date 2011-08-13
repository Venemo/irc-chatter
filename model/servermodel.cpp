
#include <QtCore>
#include <ircclient-qt/IrcSession>
#include <ircclient-qt/IrcBuffer>
#include <ircclient-qt/IrcGlobal>
#include <ircclient-qt/IrcUtil>

#include "servermodel.h"
#include "ircmodel.h"

ServerModel::ServerModel(IrcModel *parent, const QString &url, Irc::Session *backend) :
    QObject((QObject*)parent),
    _channels(new QObjectListModel<ChannelModel>(this)),
    _url(url),
    _backend(backend)
{
    if (_backend)
    {
        connect(_backend, SIGNAL(connected()), this, SLOT(backendConnectedToServer()));
        connect(_backend, SIGNAL(bufferAdded(Irc::Buffer*)), this, SLOT(backendAddedBuffer(Irc::Buffer*)));
        connect(_backend, SIGNAL(bufferRemoved(Irc::Buffer*)), this, SLOT(backendRemovedBuffer(Irc::Buffer*)));
        _backend->connectToServer(_url);
    }
}

ServerModel::~ServerModel()
{
    if (_backend)
    {
        _backend->quit("IRC Chatter (the first MeeGo IRC client) closed.");
        _backend->deleteLater();
    }
}

void ServerModel::backendConnectedToServer()
{
    connect(_backend->defaultBuffer(), SIGNAL(numericMessageReceived(QString,uint,QStringList)), this, SLOT(receiveNumericMessageFromBackend(QString,uint,QStringList)));
}

void ServerModel::backendAddedBuffer(Irc::Buffer *buffer)
{
    qDebug() << "backend added buffer " << buffer->receiver();
    _channels->addItem(new ChannelModel(this, buffer));
}

void ServerModel::backendRemovedBuffer(Irc::Buffer *buffer)
{
    qDebug() << "backend removed buffer " << buffer->receiver();
    removeModelForBuffer(buffer);
}

void ServerModel::removeModelForBuffer(Irc::Buffer *buffer)
{
    foreach (ChannelModel *channel, _channels->getList())
    {
        if (channel->name() == buffer->receiver())
        {
            if (channel == static_cast<IrcModel*>(parent())->currentChannel())
                static_cast<IrcModel*>(parent())->setCurrentChannelIndex(static_cast<IrcModel*>(parent())->currentChannelIndex() - 1);

            _channels->removeItem(channel);
            channel->deleteLater();
            break;
        }
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
    return true;
}

bool ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel " << channelName;
    _backend->part(channelName, "Leaving this channel. (with IRC Chatter, the first MeeGo IRC client)");
    return true;
}

bool ServerModel::queryUser(const QString &userName)
{
    qDebug() << "querying user " << userName;
    _backend->addBuffer(userName);
    return true;
}

bool ServerModel::closeUser(const QString &userName)
{
    qDebug() << "closing user " << userName;
    removeModelForBuffer(_backend->buffer(userName));
    return true;
}
