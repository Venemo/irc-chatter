#include "channelmodel.h"
#include <QtCore>

ChannelModel::ChannelModel(QString name, QObject *parent) :
    QObject(parent)
{
    _name = name;
    //fakeMessage();

}

void ChannelModel::close()
{
    // TODO
}

void ChannelModel::sendCurrentMessage()
{
    // TODO: send message
    qDebug() << currentMessage() << "was sent to" << name();
    setCurrentMessage(QString());
}

void ChannelModel::fakeMessage()
{
    //qDebug() << "faking new message";
    MessageModel *m = new MessageModel();
    m->setUserName("Zvdegor");
    m->setUserNameColor("#0000ff");
    m->setText(QUuid::createUuid().toString());
    m->setTimestamp(QTime::currentTime().toString("HH:mm"));
    _messages.addItem(m);
    //if (_messages.rowCount() < 15)
        QTimer::singleShot(1000, this, SLOT(fakeMessage()));
}
