#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QObject>
#include "util.h"

class MessageModel : public QObject
{
    Q_OBJECT
    GENPROPERTY(QString, _userName, userName, setUserName, userNameChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    GENPROPERTY(QString, _userNameColor, userNameColor, setUserNameColor, userNameColorChanged)
    Q_PROPERTY(QString userNameColor READ userNameColor WRITE setUserNameColor NOTIFY userNameColorChanged)
    GENPROPERTY(QString, _text, text, setText, textChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    GENPROPERTY(QString, _timestamp, timestamp, setTimestamp, timestampChanged)
    Q_PROPERTY(QString timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)

    friend class IrcModel;

public:
    explicit MessageModel(QObject *parent = 0);

signals:
    void userNameChanged();
    void userNameColorChanged();
    void textChanged();
    void timestampChanged();

public slots:

};

#endif // MESSAGEMODEL_H
