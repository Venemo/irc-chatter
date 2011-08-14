#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include "serversettings.h"
#include "qobjectlistmodel.h"

#define APPSETTINGS_VERSION 1

class QSettings;

class AppSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* serverSettings READ serverSettings NOTIFY serverSettingsChanged)
    Q_PROPERTY(QString userNickname READ userNickname WRITE setUserNickname NOTIFY userNicknameChanged)
    Q_PROPERTY(QString userIdent READ userIdent WRITE setUserIdent NOTIFY userIdentChanged)
    Q_PROPERTY(QString userRealName READ userRealName WRITE setUserRealName NOTIFY userRealNameChanged)

    QSettings _backend;
    QObjectListModel<ServerSettings> *_serverSettings;

public:
    explicit AppSettings(QObject *parent = 0);

    QString userNickname() const;
    void setUserNickname(const QString &value);
    QString userIdent() const;
    void setUserIdent(const QString &value);
    QString userRealName() const;
    void setUserRealName(const QString &value);

    QObjectListModel<ServerSettings> *serverSettings();
    Q_INVOKABLE void saveServerSettings();
    Q_INVOKABLE void appendServerSettings(ServerSettings *server);
    Q_INVOKABLE int serverSettingsCount() const;

signals:
    void serverSettingsChanged();
    void userNicknameChanged();
    void userIdentChanged();
    void userRealNameChanged();

};

#endif // APPSETTINGS_H
