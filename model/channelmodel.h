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
    GENPROPERTY_PTR(QObjectListModel<UserModel>*, _users, users, setUsers, usersChanged)
    Q_PROPERTY(QObjectListModel<UserModel>* users READ users WRITE setUsers NOTIFY usersChanged)
    GENPROPERTY_PTR(QObjectListModel<MessageModel>*, _messages, messages, setMessages, messagesChanged)
    Q_PROPERTY(QObjectListModel<MessageModel>* messages READ messages WRITE setMessages NOTIFY messagesChanged)

public:
    explicit ChannelModel(QString name = QString(), QObject *parent = 0);

signals:
    void nameChanged();
    void usersChanged();
    void messagesChanged();
    void messageReceived(const QString &message);

public slots:
    void close();
    void sendMessage(const QString &message);

};

#endif // CHANNELMODEL_H
