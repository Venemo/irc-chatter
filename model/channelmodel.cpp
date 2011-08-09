
#include <QtCore>
#include <ircclient-qt/IrcSession>
#include <ircclient-qt/IrcBuffer>
#include <ircclient-qt/IrcUtil>
#include <ircclient-qt/IrcGlobal>
#include "channelmodel.h"
#include "servermodel.h"

QList<QString> *ChannelModel::_colors = 0;

ChannelModel::ChannelModel(QString name, ServerModel *parent, Irc::Buffer *backend) :
    QObject(parent),
    _name(name),
    _users(new QStringListModel(this)),
    _messages(new QObjectListModel<MessageModel>(this)),
    _backend(backend)
{
    connect(_backend, SIGNAL(messageReceived(QString,QString)), this, SLOT(receiveMessageFromBackend(QString,QString)));
    connect(_backend, SIGNAL(unknownMessageReceived(QString,QStringList)), this, SLOT(receiveUnknownMessageFromBackend(QString,QStringList)));

    if (!_colors)
    {
        _colors = new QList<QString>();
        _colors->append("#ff0000");
        _colors->append("#00ff00");
        _colors->append("#0000ff");
    }
}

void ChannelModel::receiveMessageFromBackend(const QString &userName, const QString &message)
{
    _messages->addItem(new MessageModel(userName, message, this));
}

void ChannelModel::receiveUnknownMessageFromBackend(const QString &userName, const QStringList &message)
{
    qDebug() << "unknown message received at: " << _name << userName << message;
    _users->setStringList(_backend->names());
}

void ChannelModel::updateUserList()
{
    QStringList list = _backend->names();
    list.sort();
    _users->setStringList(list);
}

const QString ChannelModel::colorForNick(const QString &nick)
{
    if (nick == _backend->session()->nick())
        return "#000000";

    int nickvalue = 0;

    for (int index = 0; index < nick.length(); index++)
        nickvalue += nick[index].unicode();

    return (*_colors)[nickvalue % _colors->count()];
}

void ChannelModel::sendCurrentMessage()
{
    if (_currentMessage.length() > 0)
    {
        _backend->message(_currentMessage);
        qDebug() << _currentMessage << "was sent to" << name();
        setCurrentMessage(QString());
    }
}

void ChannelModel::setCurrentMessage(const QString &value)
{
    if (_completionFragment.length())
        _completionFragment.clear();

    _currentMessage = value;
    emit currentMessageChanged();
}

void ChannelModel::autoCompleteNick()
{
    if (_currentMessage.length() == 0)
    {
        // The user hasn't typed anything, let's give him the name of the last spoke user

        if (_messages->rowCount() > 0)
            setCurrentMessage(_messages->getList().last()->userName());

        return;
    }
    else if (_currentMessage[_currentMessage.length() - 1] == ' ')
        return;

    QString replacableFragment, newFragment;

    if (_completionFragment.length())
    {
        // The user already started cycling through the list of possible nicks, give the next one

        if (_possibleNickNames.count() <= 1)
            return;

        replacableFragment = _possibleNickNames[_currentCompletionIndex];
        _currentCompletionIndex ++;

        if (_currentCompletionIndex >= _possibleNickNames.count())
            _currentCompletionIndex = 0;
    }
    else
    {
        // The user already has a message and is trying to start autocompleting a name

        if (_currentMessage.contains(' '))
        {
            _currentCompletionPosition = _currentMessage.lastIndexOf(' ') + 1;
            replacableFragment = _currentMessage.mid(_currentCompletionPosition);
        }
        else
        {
            _currentCompletionPosition = 0;
            replacableFragment = _currentMessage;
        }

        _possibleNickNames.clear();
        _currentCompletionIndex = 0;

        foreach (QString nick, _users->stringList())
            if (nick.startsWith(replacableFragment, Qt::CaseInsensitive))
                _possibleNickNames.append(nick);

        if (!_possibleNickNames.count())
            return;

        _completionFragment = replacableFragment;
    }

    newFragment = _possibleNickNames[_currentCompletionIndex];
    //qDebug() << "replacable fragment is: " << replacableFragment << ", new fragment is: " << newFragment;

    _currentMessage.replace(_currentCompletionPosition, replacableFragment.length(), newFragment);
    emit currentMessageChanged();
}

void ChannelModel::fakeMessage()
{
    //qDebug() << "faking new message";
    _messages->addItem(new MessageModel("Zvdegor", QUuid::createUuid().toString(), this));
    //if (_messages.rowCount() < 15)
    QTimer::singleShot(1000, this, SLOT(fakeMessage()));
}
