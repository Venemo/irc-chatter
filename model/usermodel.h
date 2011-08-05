#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QColor>
#include "util.h"
#include "qobjectlistmodel.h"

class UserModel : public QObject
{
    Q_OBJECT
    GENPROPERTY(QString, _name, name, setName, nameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    GENPROPERTY(QColor, _color, color, setColor, colorChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit UserModel(QObject *parent = 0);

signals:
    void nameChanged();
    void colorChanged();

public slots:

};

#endif // USERMODEL_H
