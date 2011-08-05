#ifndef CHANNELMESSAGESMODEL_H
#define CHANNELMESSAGESMODEL_H

#include <QAbstractListModel>

class ChannelMessagesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ChannelMessageRoles
    {
        TextRole = Qt::UserRole + 1,
        UserNameRole,
        TimestampRole,
        ColorRole
    };

    explicit ChannelMessagesModel(QObject *parent = 0);

signals:

public slots:

};

#endif // CHANNELMESSAGESMODEL_H
