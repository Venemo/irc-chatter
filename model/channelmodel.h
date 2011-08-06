#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QtCore>
#include "util.h"
#include "qobjectlistmodel.h"
#include "usermodel.h"
#include "messagemodel.h"

class ChannelModel : public QObject
{
    Q_OBJECT
    GENPROPERTY(QString, _name, name, setName, nameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    GENPROPERTY(QString, _currentMessage, currentMessage, setCurrentMessage, currentMessageChanged)
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    QObjectListModel<MessageModel> _messages;

public:
    explicit ChannelModel(QString name = QString(), QObject *parent = 0);
    Q_INVOKABLE QObject *messages() { return &_messages; }

signals:
    void nameChanged();
    void usersChanged();
    void messagesChanged();
    void currentMessageChanged();
    void messageReceived(const MessageModel &message);

private slots:
    void fakeMessage();

public slots:
    void close();
    void sendCurrentMessage();

};

#endif // CHANNELMODEL_H
