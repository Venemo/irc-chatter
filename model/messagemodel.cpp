#include "messagemodel.h"
#include <QTime>

MessageModel::MessageModel(const QString &userName, const QString &text, ChannelModel *parent) :
    QObject((QObject*)parent),
    _userName(userName),
    _text(text),
    _timestamp(QTime::currentTime().toString("HH:mm"))
{
}
