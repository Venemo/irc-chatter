// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

#include <QtCore>
#include <QDebug>

#include "appsettings.h"

#define APPSETTING_SETTINGSVERSION "SettingsVersion"
#define APPSETTING_SERVERSETTINGS "ServerSettings"

AppSettings::AppSettings(QObject *parent) :
    QObject(parent),
    _areSettingsDeleted(false),
    _serverSettings(new QObjectListModel<ServerSettings>(this))
{
    // Checking for settings version

    bool containedSettingsVersion = _backend.contains(APPSETTING_SETTINGSVERSION);
    int savedSettingsVersion = _backend.value(APPSETTING_SETTINGSVERSION, 0).toInt();

    if (savedSettingsVersion < APPSETTINGS_VERSION)
    {
        // Clearing all settings if the saved version is different than current one
        _backend.clear();
        _backend.setValue(APPSETTING_SETTINGSVERSION, APPSETTINGS_VERSION);

        if (containedSettingsVersion)
        {
            _areSettingsDeleted = true;
            emit areSettingsDeletedChanged();
        }
    }
    else
    {
        // Deserializing server settings

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

void AppSettings::appendServerSettings(ServerSettings *serverSettings)
{
    if (!_serverSettings->getList().contains(serverSettings))
    {
        _serverSettings->addItem(serverSettings);
    }
}

void AppSettings::deleteServerSettings(ServerSettings *serverSettings)
{
    if (_serverSettings->getList().contains(serverSettings))
    {
        _serverSettings->removeItem(serverSettings);
    }
    serverSettings->deleteLater();
}

int AppSettings::serverSettingsCount() const
{
    return _serverSettings->rowCount();
}
