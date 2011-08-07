#include "channelmodel.h"
#include <QtCore>

ChannelModel::ChannelModel(QString name, QObject *parent) :
    QObject(parent)
{
    _name = name;
    //fakeMessage();

}

void ChannelModel::sendCurrentMessage()
{
    // TODO: send message
    if (_currentMessage.length() > 0)
    {
        qDebug() << currentMessage() << "was sent to" << name();
        setCurrentMessage(QString());
    }
}

void ChannelModel::fakeMessage()
{
    //qDebug() << "faking new message";
    MessageModel *m = new MessageModel();
    m->setUserName("Zvdegor");
    m->setUserNameColor("#0000ff");
    m->setText(QUuid::createUuid().toString());
    m->setTimestamp(QTime::currentTime().toString("HH:mm"));
    _messages.addItem(m);
    //if (_messages.rowCount() < 15)
    QTimer::singleShot(1000, this, SLOT(fakeMessage()));
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

        if (_messages.rowCount() > 0)
            setCurrentMessage(_messages.lastItem()->userName());

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

        foreach (QString nick, _users.stringList())
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
