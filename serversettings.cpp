#include "serversettings.h"

ServerSettings::ServerSettings(QObject *parent) :
    QObject(parent)
{
}

QDataStream &operator<<(QDataStream &stream, const ServerSettings &server)
{
    return (stream << server.serverUrl() << server.serverPassword() << server.autoJoinChannels());
}

QDataStream &operator>>(QDataStream &stream, ServerSettings &server)
{
    return (stream >> server._serverUrl >> server._serverPassword >> server._autoJoinChannels);
}
