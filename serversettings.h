#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

#include <QObject>
#include <QStringList>
#include <QDataStream>
#include "util.h"

class ServerSettings : public QObject
{
    Q_OBJECT
    GENPROPERTY(QString, _serverUrl, serverUrl, setServerUrl, serverUrlChanged)
    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    GENPROPERTY(QString, _serverPassword, serverPassword, setServerPassword, serverPasswordChanged)
    Q_PROPERTY(QString serverPassword READ serverPassword WRITE setServerPassword NOTIFY serverPasswordChanged)
    GENPROPERTY(QStringList, _autoJoinChannels, autoJoinChannels, setAutoJoinChannels, autoJoinChannelsChanged)
    Q_PROPERTY(QStringList autoJoinChannels READ autoJoinChannels WRITE setAutoJoinChannels NOTIFY autoJoinChannelsChanged)

    friend QDataStream &operator>>(QDataStream &stream, ServerSettings &server);

public:
    explicit ServerSettings(QObject *parent = 0);

signals:
    void serverUrlChanged();
    void serverPasswordChanged();
    void autoJoinChannelsChanged();

};

QDataStream &operator<<(QDataStream &stream, const ServerSettings &server);
QDataStream &operator>>(QDataStream &stream, ServerSettings &server);

#endif // SERVERSETTINGS_H
