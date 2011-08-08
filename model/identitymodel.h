#ifndef IDENTITYMODEL_H
#define IDENTITYMODEL_H

#include <QObject>
#include "util.h"

class IdentityModel : public QObject
{
    Q_OBJECT

    GENPROPERTY(QString, _nickname, nickname, setNickname, nicknameChanged)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY nicknameChanged)

    friend class IrcModel;

public:
    explicit IdentityModel(QObject *parent = 0);

signals:
    void nicknameChanged();

};

#endif // IDENTITYMODEL_H
