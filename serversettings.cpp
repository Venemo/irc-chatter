#include "serversettings.h"
#include <QtCore>

ServerSettings::ServerSettings(QObject *parent, const QString &url, const QString &password, const QStringList &autoJoinChannels) :
    QObject(parent),
    _serverUrl(url),
    _serverPassword(password),
    _autoJoinChannels(autoJoinChannels)
{
    if (!_autoJoinChannels.length() && url == "irc.freenode.net")
    {
        _autoJoinChannels.append("#irc-chatter");
        _autoJoinChannels.append("#harmattan");
        _autoJoinChannels.append("#maemo");
        _autoJoinChannels.append("#meego");
    }
}

QString ServerSettings::autoJoinChannelsInPlainString() const
{
    QString s;
    for (int i = 0; i < _autoJoinChannels.count(); i++)
    {
        s += _autoJoinChannels[i];
        if (i != _autoJoinChannels.count() - 1)
            s += ", ";
    }
    return s;
}

void ServerSettings::setAutoJoinChannelsInPlainString(const QString &value)
{
    _autoJoinChannels = value.split(",");
    for (int i = 0; i < _autoJoinChannels.count(); i++)
        _autoJoinChannels[i] = _autoJoinChannels[i].trimmed();
}

QDataStream &operator<<(QDataStream &stream, const ServerSettings &server)
{
    return stream << server.serverUrl() << server.serverPassword() << server.autoJoinChannels();
}

QDataStream &operator>>(QDataStream &stream, ServerSettings &server)
{
    return stream >> server._serverUrl >> server._serverPassword >> server._autoJoinChannels;
}
