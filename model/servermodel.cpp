#include "servermodel.h"

ServerModel::ServerModel(QObject *parent) :
    QObject(parent),
    _channels(new QObjectListModel<ChannelModel>(this))
{
}

bool ServerModel::joinChannel(const QString &channelName)
{
    // TODO: join the channel in the backend
    return true;
}

bool ServerModel::partChannel(const QString &channelName)
{
    // TODO
    return true;
}

bool ServerModel::queryUser(const QString &userName)
{
    // TODO: query the user in the backend
    return true;
}

bool ServerModel::closeUser(const QString &userName)
{
    // TODO
    return true;
}
