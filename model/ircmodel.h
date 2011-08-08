#ifndef IRCMODEL_H
#define IRCMODEL_H

#include <QObject>
#include "channelmodel.h"
#include "messagemodel.h"
#include "servermodel.h"
#include "qobjectlistmodel.h"

class IrcModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* allChannels READ allChannels NOTIFY allChannelsChanged)
    GENPROPERTY_R(QObjectListModel<ServerModel>*, _servers, servers)
    Q_PROPERTY(QObject* servers READ servers NOTIFY serversChanged)

public:
    explicit IrcModel(QObject *parent = 0);
    QObjectListModel<ChannelModel> *allChannels();

    void fillWithDummyData();

signals:
    void allChannelsChanged();
    void serversChanged();

};

#endif // IRCMODEL_H
