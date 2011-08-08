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
    GENPROPERTY(int, _currentChannelIndex, currentChannelIndex, setCurrentChannelIndex, currentChannelIndexChanged)
    Q_PROPERTY(int currentChannelIndex READ currentChannelIndex WRITE setCurrentChannelIndex NOTIFY currentChannelIndexChanged)
    Q_PROPERTY(QObject* currentChannel READ currentChannel NOTIFY currentChannelIndexChanged)
    Q_PROPERTY(QObject* currentServer READ currentServer NOTIFY currentChannelIndexChanged)

public:
    explicit IrcModel(QObject *parent = 0);
    QObjectListModel<ChannelModel> *allChannels();
    ChannelModel *currentChannel() { return static_cast<ChannelModel*>(allChannels()->getItem(_currentChannelIndex)); }
    ServerModel *currentServer() { return static_cast<ServerModel*>(currentChannel()->parent()); }

    void fillWithDummyData();

signals:
    void allChannelsChanged();
    void serversChanged();
    void currentChannelIndexChanged();

};

#endif // IRCMODEL_H
