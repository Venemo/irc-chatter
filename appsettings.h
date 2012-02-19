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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>

#include "serversettings.h"
#include "qobjectlistmodel.h"
#include "util.h"

// Increase this when anything changes in the settings
#define APPSETTINGS_VERSION 4

// Setting property
#define SETTINGPROPERTY(type, name, settername, signalname, settingKey, defaultValue) \
    inline type name() const { return _backend.value(settingKey, defaultValue).value<type>(); } \
    inline void settername (const type &value) { _backend.setValue(settingKey, value); emit signalname (); }

class AppSettings : public QObject
{
    Q_OBJECT
    GENPROPERTY_R(bool, _areSettingsDeleted, areSettingsDeleted)
    Q_PROPERTY(bool areSettingsDeleted READ areSettingsDeleted NOTIFY areSettingsDeletedChanged)
    Q_PROPERTY(QObject* serverSettings READ serverSettings NOTIFY serverSettingsChanged)

    Q_PROPERTY(QString kickMessage READ kickMessage WRITE setKickMessage NOTIFY kickMessageChanged)
    Q_PROPERTY(QString partMessage READ partMessage WRITE setPartMessage NOTIFY partMessageChanged)
    Q_PROPERTY(QString quitMessage READ quitMessage WRITE setQuitMessage NOTIFY quitMessageChanged)
    Q_PROPERTY(QString sidebarColor READ sidebarColor WRITE setSidebarColor NOTIFY sidebarColorChanged)
    Q_PROPERTY(bool fontMonospace READ fontMonospace WRITE setFontMonospace NOTIFY fontMonospaceChanged)
    Q_PROPERTY(bool autoFocusTextField READ autoFocusTextField WRITE setAutoFocusTextField NOTIFY autoFocusTextFieldChanged)
    Q_PROPERTY(bool displayMiscEvents READ displayMiscEvents WRITE setDisplayMiscEvents NOTIFY displayMiscEventsChanged)
    Q_PROPERTY(bool displayTimestamps READ displayTimestamps WRITE setDisplayTimestamps NOTIFY displayTimestampsChanged)
    Q_PROPERTY(bool notifyOnNick READ notifyOnNick WRITE setNotifyOnNick NOTIFY notifyOnNickChanged)
    Q_PROPERTY(bool notifyOnPrivmsg READ notifyOnPrivmsg WRITE setNotifyOnPrivmsg NOTIFY notifyOnPrivmsgChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    QSettings _backend;
    QObjectListModel<ServerSettings> *_serverSettings;

public:
    explicit AppSettings(QObject *parent = 0);

    SETTINGPROPERTY(QString, partMessage, setPartMessage, partMessageChanged, "partMessage", "Leaving this channel. (with IRC Chatter)")
    SETTINGPROPERTY(QString, kickMessage, setKickMessage, kickMessageChanged, "kickMessage", "Kindergarten is elsewhere!")
    SETTINGPROPERTY(QString, quitMessage, setQuitMessage, quitMessageChanged, "quitMessage", "IRC Chatter closed.")
    SETTINGPROPERTY(QString, sidebarColor, setSidebarColor, sidebarColorChanged, "sidebarColor", "#f9a300")
    SETTINGPROPERTY(bool, fontMonospace, setFontMonospace, fontMonospaceChanged, "fontMonospace", false)
    SETTINGPROPERTY(bool, autoFocusTextField, setAutoFocusTextField, autoFocusTextFieldChanged, "autoFocusTextField", false)
    SETTINGPROPERTY(bool, displayMiscEvents, setDisplayMiscEvents, displayMiscEventsChanged, "displayMiscEvents", true)
    SETTINGPROPERTY(bool, displayTimestamps, setDisplayTimestamps, displayTimestampsChanged, "displayTimestamps", true)
    SETTINGPROPERTY(bool, notifyOnNick, setNotifyOnNick, notifyOnNickChanged, "notifyOnNick", true)
    SETTINGPROPERTY(bool, notifyOnPrivmsg, setNotifyOnPrivmsg, notifyOnPrivmsgChanged, "notifyOnPrivmsg", true)
    SETTINGPROPERTY(int, fontSize, setFontSize, fontSizeChanged, "fontSize", 24)

    QObjectListModel<ServerSettings> *serverSettings();
    Q_INVOKABLE void appendServerSettings(ServerSettings *server);
    Q_INVOKABLE int serverSettingsCount() const;
    Q_INVOKABLE ServerSettings *newServerSettings() { return new ServerSettings(this); }

public slots:
    Q_INVOKABLE void saveServerSettings();

signals:
    void areSettingsDeletedChanged();
    void serverSettingsChanged();

    void kickMessageChanged();
    void partMessageChanged();
    void quitMessageChanged();
    void fontSizeChanged();
    void fontMonospaceChanged();
    void sidebarColorChanged();
    void autoFocusTextFieldChanged();
    void displayMiscEventsChanged();
    void displayTimestampsChanged();
    void notifyOnNickChanged();
    void notifyOnPrivmsgChanged();
};

#endif // APPSETTINGS_H
