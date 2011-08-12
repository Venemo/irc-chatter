#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QObject>
#include <QStringListModel>
#include "util.h"
#include "qobjectlistmodel.h"

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
    Q_PROPERTY(QObject* server READ parent NOTIFY serverChanged)
    GENPROPERTY(QString, _channelText, channelText, setChannelText, channelTextChanged)
    Q_PROPERTY(QString channelText READ channelText NOTIFY channelTextChanged)

    static QString _autoCompletionSuffix;
    QString _completionFragment;
    QList<QString> _possibleNickNames;
    int _currentCompletionIndex, _currentCompletionPosition;

    Irc::Buffer *_backend;

    static QList<QString> *_colors;
    static QRegExp _urlRegexp;
    friend class IrcModel;
    friend class ServerModel;

    void parseCommand(const QString &msg);
    QString &processMessage(QString &msg);
    void appendCommandInfo(const QString &msg);

protected:
    explicit ChannelModel(QString name, ServerModel *parent, Irc::Buffer *backend);

public:
    ~ChannelModel();
    int userCount() { return _users->rowCount(); }
    void setCurrentMessage(const QString &value);

    Q_INVOKABLE void autoCompleteNick();
    Q_INVOKABLE const QString colorForNick(const QString &nick);

signals:
    void nameChanged();
    void currentMessageChanged();
    void usersChanged();
    void serverChanged();
    void channelTextChanged();

private slots:
    void fakeMessage();
    void backendDeleted();
    void receiveMessageFromBackend(const QString &userName, QString message);
    void receiveNoticeFromBackend(const QString &userName, QString message);
    void receiveCtcpActionFromBackend(const QString &userName, QString message);
    void receiveCtcpRequestFromBackend(const QString &userName, QString message);
    void receiveCtcpReplyFromBackend(const QString &userName, QString message);
    void receiveUnknownMessageFromBackend(const QString &userName, const QStringList &message);

public slots:
    void sendCurrentMessage();
    void updateUserList();

};

#endif // CHANNELMODEL_H
