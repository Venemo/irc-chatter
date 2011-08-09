#include "servermodel.h"
#include <QtCore>

ServerModel::ServerModel(QObject *parent) :
    QObject(parent),
    _channels(new QObjectListModel<ChannelModel>(this))
{
}

bool ServerModel::joinChannel(const QString &channelName)
{
    qDebug() << "joining channel " << channelName;
    // TODO: join the channel in the backend
    return true;
}

bool ServerModel::partChannel(const QString &channelName)
{
    qDebug() << "parting channel " << channelName;
    // TODO
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
