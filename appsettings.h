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
#define APPSETTINGS_VERSION 3

class QSettings;

class AppSettings : public QObject
{
    Q_OBJECT
    GENPROPERTY_R(bool, _areSettingsDeleted, areSettingsDeleted)
    Q_PROPERTY(bool areSettingsDeleted READ areSettingsDeleted NOTIFY areSettingsDeletedChanged)
    Q_PROPERTY(QObject* serverSettings READ serverSettings NOTIFY serverSettingsChanged)
    Q_PROPERTY(QString userNickname READ userNickname WRITE setUserNickname NOTIFY userNicknameChanged)
    Q_PROPERTY(QString userIdent READ userIdent WRITE setUserIdent NOTIFY userIdentChanged)
    Q_PROPERTY(QString userRealName READ userRealName WRITE setUserRealName NOTIFY userRealNameChanged)
    Q_PROPERTY(QString kickMessage READ kickMessage WRITE setKickMessage NOTIFY kickMessageChanged)
    Q_PROPERTY(QString partMessage READ partMessage WRITE setPartMessage NOTIFY partMessageChanged)
    Q_PROPERTY(quint16 fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    QSettings _backend;
    QObjectListModel<ServerSettings> *_serverSettings;

public:
    explicit AppSettings(QObject *parent = 0);

    QString userNickname() const;
    void setUserNickname(const QString &value);
    QString userIdent() const;
    void setUserIdent(const QString &value);
    QString userRealName() const;
    void setUserRealName(const QString &value);
    QString partMessage() const;
    void setPartMessage(const QString &value);
    QString kickMessage() const;
    void setKickMessage(const QString &value);
    QString quitMessage() const;
    void setQuitMessage(const QString &value);
    quint16 fontSize() const;
    void setFontSize(const quint16 &value);

    QObjectListModel<ServerSettings> *serverSettings();
    Q_INVOKABLE void saveServerSettings();
    Q_INVOKABLE void appendServerSettings(ServerSettings *server);
    Q_INVOKABLE int serverSettingsCount() const;

signals:
    void areSettingsDeletedChanged();
    void serverSettingsChanged();
    void userNicknameChanged();
    void userIdentChanged();
    void userRealNameChanged();
    void kickMessageChanged();
    void partMessageChanged();
    void fontSizeChanged();

};

#endif // APPSETTINGS_H
