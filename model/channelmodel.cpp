#include "channelmodel.h"

ChannelModel::ChannelModel(QString name, QObject *parent) :
    QObject(parent)
{
    _name = name;
}

void ChannelModel::close()
{
    // TODO
}

void ChannelModel::sendMessage(const QString &message)
{
    // TODO
}
