#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QObject>
#include <QStringListModel>
#include "util.h"
#include "qobjectlistmodel.h"
#include "messagemodel.h"

namespace Irc { class Buffer; }
class ServerModel;

class ChannelModel : public QObject
{
    Q_OBJECT

    GENPROPERTY(QString, _name, name, setName, nameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    GENPROPERTY_R(QString, _currentMessage, currentMessage)
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)
    GENPROPERTY_R(QStringListModel*, _users, users)
    Q_PROPERTY(QObject* users READ users NOTIFY usersChanged)
    Q_PROPERTY(int userCount READ userCount NOTIFY usersChanged)
    GENPROPERTY_R(QObjectListModel<MessageModel>*, _messages, messages)
    Q_PROPERTY(QObject* messages READ messages NOTIFY messagesChanged)
    Q_PROPERTY(QObject* server READ parent NOTIFY serverChanged)

    static QString _autoCompletionSuffix;
    QString _completionFragment;
    QList<QString> _possibleNickNames;
    int _currentCompletionIndex, _currentCompletionPosition;

    Irc::Buffer *_backend;

    static QList<QString> *_colors;
    friend class IrcModel;
    friend class ServerModel;

protected:
    explicit ChannelModel(QString name, ServerModel *parent, Irc::Buffer *backend);

public:
    int userCount() { return _users->rowCount(); }
    void setCurrentMessage(const QString &value);

    Q_INVOKABLE void autoCompleteNick();
    Q_INVOKABLE const QString colorForNick(const QString &nick);

signals:
    void nameChanged();
    void currentMessageChanged();
    void usersChanged();
    void messagesChanged();
    void serverChanged();

private slots:
    void fakeMessage();
    void receiveMessageFromBackend(const QString &userName, const QString &message);
    void receiveUnknownMessageFromBackend(const QString &userName, const QStringList &message);
    void receiveNoticeFromBackend(const QString &userName, const QString &message);
    void receiveCtcpActionFromBackend(const QString &userName, const QString &message);
    void receiveCtcpRequestFromBackend(const QString &userName, const QString &message);
    void receiveCtcpReplyFromBackend(const QString &userName, const QString &message);

public slots:
    void sendCurrentMessage();
    void updateUserList();

};

#endif // CHANNELMODEL_H
