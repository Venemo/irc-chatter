#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QObject>
#include <QStringListModel>
#include "util.h"
#include "qobjectlistmodel.h"
#include "messagemodel.h"

class ChannelModel : public QObject
{
    Q_OBJECT

    GENPROPERTY(QString, _name, name, setName, nameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    GENPROPERTY_R(QString, _currentMessage, currentMessage)
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    Q_PROPERTY(QObject* users READ users NOTIFY usersChanged)
    Q_PROPERTY(int userCount READ userCount NOTIFY usersChanged)

    QObjectListModel<MessageModel> _messages;
    QStringListModel _users;

    QString _completionFragment;
    QList<QString> _possibleNickNames;
    int _currentCompletionIndex, _currentCompletionPosition;

    friend class IrcModel;

public:
    explicit ChannelModel(QString name = QString(), QObject *parent = 0);
    Q_INVOKABLE QObject *messages() { return &_messages; }
    Q_INVOKABLE QStringListModel *users() { return &_users; }
    Q_INVOKABLE int userCount() { return _users.rowCount(); }
    Q_INVOKABLE void autoCompleteNick();
    void setCurrentMessage(const QString &value);

signals:
    void nameChanged();
    void currentMessageChanged();
    void usersChanged();

private slots:
    void fakeMessage();

public slots:
    void sendCurrentMessage();

};

#endif // CHANNELMODEL_H
