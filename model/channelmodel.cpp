
#include <QtCore>
#include <ircclient-qt/IrcSession>
#include <ircclient-qt/IrcBuffer>
#include <ircclient-qt/IrcUtil>
#include <ircclient-qt/IrcGlobal>
#include "channelmodel.h"
#include "servermodel.h"

QList<QString> *ChannelModel::_colors = 0;
QRegExp ChannelModel::_urlRegexp(QString("\\b((?:(?:([a-z][\\w-]+:/{1,3})|www\\d{0,3}[.]|[a-z0-9.\\-]+[.][a-z]{2,4}/)(?:[^\\s()<>]+|\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\))+(?:\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\)|\\}\\]|[^\\s`!()\\[\\]{};:'\".,<>?%1%2%3%4%5%6])|[a-z0-9.\\-+_]+@[a-z0-9.\\-]+[.][a-z]{1,5}[^\\s/`!()\\[\\]{};:'\".,<>?%1%2%3%4%5%6]))").arg(QChar(0x00AB)).arg(QChar(0x00BB)).arg(QChar(0x201C)).arg(QChar(0x201D)).arg(QChar(0x2018)).arg(QChar(0x2019)));
QString ChannelModel::_autoCompletionSuffix(", ");

ChannelModel::ChannelModel(QString name, ServerModel *parent, Irc::Buffer *backend) :
    QObject(parent),
    _name(name),
    _users(new QStringListModel(this)),
    _backend(backend)
{
    if (_backend)
    {
        connect(_backend, SIGNAL(messageReceived(QString,QString)), this, SLOT(receiveMessageFromBackend(QString,QString)));
        connect(_backend, SIGNAL(unknownMessageReceived(QString,QStringList)), this, SLOT(receiveUnknownMessageFromBackend(QString,QStringList)));
        connect(_backend, SIGNAL(noticeReceived(QString,QString)), this, SLOT(receiveNoticeFromBackend(QString,QString)));
        connect(_backend, SIGNAL(ctcpActionReceived(QString,QString)), this, SLOT(receiveCtcpActionFromBackend(QString,QString)));
        connect(_backend, SIGNAL(ctcpRequestReceived(QString,QString)), this, SLOT(receiveCtcpRequestFromBackend(QString,QString)));
        connect(_backend, SIGNAL(ctcpReplyReceived(QString,QString)), this, SLOT(receiveCtcpReplyFromBackend(QString,QString)));
    }

    if (!_colors)
    {
        _colors = new QList<QString>();
        _colors->append("#ff0000");
        _colors->append("#00ff00");
        _colors->append("#0000ff");
    }
}

QString &ChannelModel::processMessage(QString &msg)
{
    msg.replace('&', "&amp;");
    msg.replace('<', "&lt;");
    msg.replace('<', "&gt;");
    msg.replace(_urlRegexp, "<a href=\"\\1\">\\1</a>");
    return msg;
}

void ChannelModel::appendCommandInfo(const QString &msg)
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += "<span style='color: yellow'>" + msg + "</span>");
}

void ChannelModel::receiveMessageFromBackend(const QString &userName, QString message)
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += QTime::currentTime().toString("HH:mm") + " <span style='color: " + colorForNick(userName) + "'>" + userName + "</span>: " + processMessage(message));
}

void ChannelModel::receiveNoticeFromBackend(const QString &userName, QString message)
{
    // Notice is basically a "private message", and that is supposed to be displayed the same way as a normal message

    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += QTime::currentTime().toString("HH:mm") + " <span style='color: " + colorForNick(userName) + "'>" + userName + "</span>: " + processMessage(message));
}

void ChannelModel::receiveCtcpActionFromBackend(const QString &userName, QString message)
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += QTime::currentTime().toString("HH:mm") + " * <span style='color: " + colorForNick(userName) + "'>" + userName + "</span> " + processMessage(message));
}

void ChannelModel::receiveCtcpRequestFromBackend(const QString &userName, QString message)
{
    qDebug() << "CTCP request received " << userName << message;
}

void ChannelModel::receiveCtcpReplyFromBackend(const QString &userName, QString message)
{
    qDebug() << "CTCP reply received " << userName << message;
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
        if (_currentMessage.startsWith("/"))
            parseCommand(_currentMessage);
        else
            _backend->message(_currentMessage);

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

        //...

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

        replacableFragment = _possibleNickNames[_currentCompletionIndex] + _autoCompletionSuffix;
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
    if (_currentCompletionPosition == 0)
        newFragment += _autoCompletionSuffix;

    //qDebug() << "replacable fragment is: " << replacableFragment << ", new fragment is: " << newFragment;

    _currentMessage.replace(_currentCompletionPosition, replacableFragment.length(), newFragment);
    emit currentMessageChanged();
}

void ChannelModel::fakeMessage()
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += QTime::currentTime().toString("HH:mm") + " <span style='color: " + colorForNick("Zvdegor") + "'>" + "Zvdegor" + "</span>: " + QUuid::createUuid().toString());
}

void ChannelModel::parseCommand(const QString &msg)
{
    QStringList commandParts = msg.split(' ', QString::SkipEmptyParts);
    int n = commandParts.count();

    if (commandParts[0] == "/join" || commandParts[0] == "/j")
    {
        if (n == 2)
            static_cast<ServerModel*>(parent())->joinChannel(commandParts[1]);
        else
            appendCommandInfo("Invalid command. Correct usage: '/join &lt;channelname&gt;'");
    }
    else if (commandParts[0] == "/part" || commandParts[0] == "/p")
    {
        if (n == 1)
            static_cast<ServerModel*>(parent())->partChannel(name());
        else if (n == 2)
            static_cast<ServerModel*>(parent())->partChannel(commandParts[1]);
        else
            appendCommandInfo("Invalid command. Correct usage: '/part' or '/join &lt;channelname&gt'';");
    }
    else if (commandParts[0] == "/quit" || commandParts[0] == "/q")
    {
        if (n == 1)
            QCoreApplication::instance()->quit();
        else
            appendCommandInfo("Invalid command. Correct usage: '/quit'");

    }
    else
        appendCommandInfo("Unknown command, maybe it will be supported later?");
}
