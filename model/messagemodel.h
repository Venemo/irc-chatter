#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QtCore>
#include "util.h"
#include "usermodel.h"

class MessageModel : public QObject
{
    Q_OBJECT
    GENPROPERTY_PTR(UserModel*, _user, user, setUser, userChanged)
    Q_PROPERTY(UserModel* user READ user WRITE setUser NOTIFY userChanged)
    GENPROPERTY(QString, _text, text, setText, textChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    GENPROPERTY(QString, _timestamp, timestamp, setTimestamp, timestampChanged)
    Q_PROPERTY(QString timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)

public:
    explicit MessageModel(QObject *parent = 0);

signals:
    void userChanged();
    void textChanged();
    void timestampChanged();

public slots:

};

#endif // MESSAGEMODEL_H
