#ifndef QOBJECTLISTMODEL_H
#define QOBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QMetaProperty>

template<typename X>
class QObjectListModel : public QAbstractListModel
{
    QHash<int, QByteArray> _roles;
    QList<X*> _list;

public:
    explicit QObjectListModel(const QList<X*> &list = QList<X*>(), QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void addItem(X *item);
    void removeItem(const X *item);
    void removeItem(int index);
};

template<typename T>
QObjectListModel<T>::QObjectListModel(const QList<T*> &list, QObject *parent)
    : QAbstractListModel(parent),
      _list(list)
{
    QMetaObject meta = T::staticMetaObject;
    for (int i = 0; i < meta.propertyCount(); i++)
    {
        QMetaProperty prop = meta.property(i);
        _roles[Qt::UserRole + i + 1] = QByteArray(prop.name());
    }
    setRoleNames(_roles);
}

template<typename T>
int QObjectListModel<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.count();
}

template<typename T>
int QObjectListModel<T>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _roles.count();
}

template<typename T>
QVariant QObjectListModel<T>::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.count())
        return QVariant();

    const QObject *obj = _list[index.row()];
    return obj->property(_roles[role].data());
}

template<typename T>
bool QObjectListModel<T>::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= _list.count())
        return false;

    QObject *obj = _list[index.row()];
    return obj->setProperty(_roles[role].data(), value);
}

template<typename T>
void QObjectListModel<T>::addItem(T *item)
{
    int z = _list.count();
    beginInsertRows(QModelIndex(), z, z);
    _list.append(item);
    endInsertRows();
}

template<typename T>
void QObjectListModel<T>::removeItem(const T *item)
{
    int z = _list.indexOf(item);
    beginRemoveRows(QModelIndex(), z, z);
    _list.removeAt(z);
    endRemoveRows();
}

template<typename T>
void QObjectListModel<T>::removeItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    _list.removeAt(index);
    endRemoveRows();
}

#endif // QOBJECTLISTMODEL_H
