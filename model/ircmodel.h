#ifndef IRCMODEL_H
#define IRCMODEL_H

#include <QObject>
#include "channelmodel.h"
#include "messagemodel.h"
#include "qobjectlistmodel.h"

class IrcModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* channelList READ channelList NOTIFY channelListChanged)
    QObjectListModel<ChannelModel> _channelList;
    QList<QString> _colors;

public:
    explicit IrcModel(QObject *parent = 0);
    QObject *channelList() { return &_channelList; }
    Q_INVOKABLE const QString &colorForNick(const QString &nick);
    Q_INVOKABLE bool joinChannel(const QString &channelName);
    Q_INVOKABLE bool partChannel(const QString &channelName);
    Q_INVOKABLE bool queryUser(const QString &userName);
    Q_INVOKABLE bool closeUser(const QString &userName);

    void fillWithDummyData();

signals:
    void channelListChanged();

};

#endif // IRCMODEL_H
