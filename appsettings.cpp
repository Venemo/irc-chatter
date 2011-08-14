
#include <QtCore>
#include "appsettings.h"

#define APPSETTING_SERVERSETTINGS "ServerSettings"
#define APPSETTING_USERNICKNAME "UserNickname"
#define APPSETTING_USERIDENT "UserIdent"
#define APPSETTING_USERREALNAME "UserRealName"

AppSettings::AppSettings(QObject *parent) :
    QObject(parent),
    _serverSettings(new QObjectListModel<ServerSettings>(this))
{
    QByteArray array = _backend.value(APPSETTING_SERVERSETTINGS).toByteArray();
    QDataStream stream(array);
    int n;
    stream >> n;
    for (int i = 0; i < n; i++)
    {
        ServerSettings *server = new ServerSettings((QObject*)this);
        stream >> (*server);
        _serverSettings->addItem(server);
    }
}

QString AppSettings::userNickname() const
{
    return _backend.value(APPSETTING_USERNICKNAME).toString();
}

void AppSettings::setUserNickname(const QString &value)
{
    _backend.setValue(APPSETTING_USERNICKNAME, value);
}

QString AppSettings::userIdent() const
{
    return _backend.value(APPSETTING_USERIDENT).toString();
}

void AppSettings::setUserIdent(const QString &value)
{
    _backend.setValue(APPSETTING_USERIDENT, value);
}

QString AppSettings::userRealName() const
{
    return _backend.value(APPSETTING_USERREALNAME).toString();
}

void AppSettings::setUserRealName(const QString &value)
{
    _backend.setValue(APPSETTING_USERREALNAME, value);
}

QObjectListModel<ServerSettings> *AppSettings::serverSettings()
{
    return _serverSettings;
}

void AppSettings::saveServerSettings()
{
    QByteArray array;
    QBuffer buffer(&array);
    QDataStream stream(&buffer);
    buffer.open(QBuffer::WriteOnly);
    stream << _serverSettings->rowCount();
    foreach (ServerSettings *server, _serverSettings->getList())
        stream << (*server);
    buffer.close();
    _backend.setValue(APPSETTING_SERVERSETTINGS, array);
}

void AppSettings::appendServerSettings(ServerSettings *server)
{
    _serverSettings->addItem(server);
}

int AppSettings::serverSettingsCount() const
{
    return _serverSettings->rowCount();
}
