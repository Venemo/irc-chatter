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
    GENPROPERTY(quint16, _serverPort, serverPort, setServerPort, serverPortChanged)
    Q_PROPERTY(quint16 serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    GENPROPERTY(bool, _serverSSL, serverSSL, setServerSSL, serverSSLChanged)
    Q_PROPERTY(bool serverSSL READ serverSSL WRITE setServerSSL NOTIFY serverSSLChanged)
    GENPROPERTY(QString, _serverPassword, serverPassword, setServerPassword, serverPasswordChanged)
    Q_PROPERTY(QString serverPassword READ serverPassword WRITE setServerPassword NOTIFY serverPasswordChanged)
    GENPROPERTY(QStringList, _autoJoinChannels, autoJoinChannels, setAutoJoinChannels, autoJoinChannelsChanged)
    Q_PROPERTY(QStringList autoJoinChannels READ autoJoinChannels WRITE setAutoJoinChannels NOTIFY autoJoinChannelsChanged)
    Q_PROPERTY(QString autoJoinChannelsInPlainString READ autoJoinChannelsInPlainString WRITE setAutoJoinChannelsInPlainString NOTIFY autoJoinChannelsChanged)


    friend QDataStream &operator>>(QDataStream &stream, ServerSettings &server);

public:
    explicit ServerSettings(QObject *parent = 0, const QString &url = "irc.freenode.net", const quint16 &port = 6667, const bool ssl = false, const QString &password = QString(), const QStringList &autoJoinChannels = QStringList());
    QString autoJoinChannelsInPlainString() const;
    void setAutoJoinChannelsInPlainString(const QString &value);

signals:
    void serverUrlChanged();
    void serverPasswordChanged();
    void autoJoinChannelsChanged();
    void serverPortChanged();
    void serverSSLChanged();

};

QDataStream &operator<<(QDataStream &stream, const ServerSettings &server);
QDataStream &operator>>(QDataStream &stream, ServerSettings &server);

#endif // SERVERSETTINGS_H
