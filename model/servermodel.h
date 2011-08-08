#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QObject>
#include "util.h"
#include "qobjectlistmodel.h"
#include "channelmodel.h"

class ServerModel : public QObject
{
    Q_OBJECT

    GENPROPERTY_R(QObjectListModel<ChannelModel>*, _channels, channels)
    Q_PROPERTY(QObject* channels READ channels NOTIFY channelsChanged)
    GENPROPERTY(QString, _url, url, setUrl, urlChanged)
    GENPROPERTY(QString, _password, password, setPassword, passwordChanged)

    friend class IrcModel;

protected:
    explicit ServerModel(QObject *parent = 0);

public:
    Q_INVOKABLE bool joinChannel(const QString &channelName);
    Q_INVOKABLE bool partChannel(const QString &channelName);
    Q_INVOKABLE bool queryUser(const QString &userName);
    Q_INVOKABLE bool closeUser(const QString &userName);

signals:
    void channelsChanged();
    void urlChanged();
    void passwordChanged();

public slots:

};

#endif // SERVERMODEL_H
