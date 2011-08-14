
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

ChannelModel::ChannelModel(ServerModel *parent, Irc::Buffer *backend) :
    QObject(parent),
    _users(new QStringListModel(this)),
    _backend(backend)
{
    if (!_colors)
    {
        _colors = new QList<QString>();
        _colors->append("#ff0000");
        _colors->append("#00ff00");
        _colors->append("#0000ff");
    }

    if (!_backend)
        qDebug() << "A channel backend is null, this is an error. The app will segfault.";

    connect(_backend, SIGNAL(destroyed()), this, SLOT(backendDeleted()));
    connect(_backend, SIGNAL(messageReceived(QString,QString)), this, SLOT(receiveMessageFromBackend(QString,QString)));
    connect(_backend, SIGNAL(unknownMessageReceived(QString,QStringList)), this, SLOT(receiveUnknownMessageFromBackend(QString,QStringList)));
    connect(_backend, SIGNAL(noticeReceived(QString,QString)), this, SLOT(receiveNoticeFromBackend(QString,QString)));
    connect(_backend, SIGNAL(motdReceived(QString)), this, SLOT(receiveMotdFromBackend(QString)));
    connect(_backend, SIGNAL(ctcpActionReceived(QString,QString)), this, SLOT(receiveCtcpActionFromBackend(QString,QString)));
    connect(_backend, SIGNAL(ctcpRequestReceived(QString,QString)), this, SLOT(receiveCtcpRequestFromBackend(QString,QString)));
    connect(_backend, SIGNAL(ctcpReplyReceived(QString,QString)), this, SLOT(receiveCtcpReplyFromBackend(QString,QString)));
    connect(_backend, SIGNAL(joined(QString)), this, SLOT(receiveJoinedFromBackend(QString)));
    connect(_backend, SIGNAL(parted(QString,QString)), this, SLOT(receivePartedFromBackend(QString,QString)));
    connect(_backend, SIGNAL(quit(QString,QString)), this, SLOT(receiveQuitFromBackend(QString,QString)));
    connect(_backend, SIGNAL(nickChanged(QString,QString)), this, SLOT(receiveNickChangeFromBackend(QString,QString)));
    connect(_backend, SIGNAL(invited(QString,QString,QString)), this, SLOT(receiveInviteFromBackend(QString,QString,QString)));
    connect(_backend, SIGNAL(kicked(QString,QString,QString)), this, SLOT(receiveKickedFromBackend(QString,QString,QString)));
    connect(_backend, SIGNAL(receiverChanged(QString)), this, SLOT(channelNameChanged(QString)));
    connect(_backend, SIGNAL(receiverChanged(QString)), this, SIGNAL(nameChanged()));

}

ChannelModel::~ChannelModel()
{
    if (_backend)
        _backend->deleteLater();
}

QString ChannelModel::name() const
{
    return _backend->receiver();
}

void ChannelModel::channelNameChanged(const QString &newName)
{
    appendChannelInfo("Channel name is changed to " + newName);
}

void ChannelModel::receiveMotdFromBackend(QString motd)
{
    appendChannelInfo("[MOTD] " + processMessage(motd));
}

void ChannelModel::receiveJoinedFromBackend(const QString &userName)
{
    if (userName != _backend->session()->nick())
        appendChannelInfo("--> " + userName + " has joined this channel.");

    updateUserList();
}

void ChannelModel::receivePartedFromBackend(const QString &userName, QString reason)
{
    appendChannelInfo("<-- " + userName + " has parted this channel." + (reason.length() ? (" (Reason: " + processMessage(reason) + ")") : ""));
    updateUserList();
}

void ChannelModel::receiveQuitFromBackend(const QString &userName, QString reason)
{
    appendChannelInfo("<-- " + userName + " has left this server." + (reason.length() ? (" (Reason: " + processMessage(reason) + ")") : ""));
    updateUserList();
}

void ChannelModel::receiveNickChangeFromBackend(const QString &oldNick, const QString &newNick)
{
    appendChannelInfo("*** " + oldNick + " has changed nick to " + newNick + ".");
    updateUserList();
}

void ChannelModel::receiveInviteFromBackend(const QString &origin, const QString &receiver, const QString &channel)
{
    appendCommandInfo("*** " + origin + " has invited " + receiver + " to " + channel + ".");
}

void ChannelModel::receiveKickedFromBackend(const QString &origin, const QString &nick, QString message)
{
    appendCommandInfo("*** " + origin + " has kicked " + nick + " with message '" + processMessage(message) + "'.");
}

QString &ChannelModel::processMessage(QString &msg)
{
    msg.replace('&', "&amp;");
    msg.replace('<', "&lt;");
    msg.replace('>', "&gt;");
    msg.replace('\n', "<br />");
    msg.replace(_urlRegexp, "<a href=\"\\1\">\\1</a>");
    return msg;
}

void ChannelModel::appendCommandInfo(const QString &msg)
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += "<span style='color: orange'>" + msg + "</span>");
}

void ChannelModel::appendChannelInfo(const QString &msg)
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += "<span style='color: purple'>" + msg + "</span>");
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
    receiveMessageFromBackend(userName, message);
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
    _backend->session()->ctcpReply(userName, "IRC Chatter, the first MeeGo IRC client");
}

void ChannelModel::receiveCtcpReplyFromBackend(const QString &userName, QString message)
{
    qDebug() << "CTCP reply received " << userName << message;
    appendCommandInfo("CTCP Reply from " + userName + ": " + message);
}

void ChannelModel::receiveUnknownMessageFromBackend(const QString &userName, const QStringList &message)
{
    qDebug() << "unknown message received at: " << name() << userName << message;
    _users->setStringList(_backend->names());
}

void ChannelModel::updateUserList()
{
    QStringList list = _backend->names();
    // workaround for bug: https://bugreports.qt.nokia.com/browse/QTBUG-12892
    // found in http://www.harshj.com/2009/10/24/sorting-entries-in-a-qstringlist-case-insensitively/
    // - Thank you!
    QMap<QString, QString> strMap;
    foreach (QString str, list)
    {
        strMap.insert( str.toLower(), str );
    }
    list = strMap.values();
    _users->setStringList(list);
}

void ChannelModel::setTopic(const QString &value)
{
    _topic = value;
    appendCommandInfo("[TOPIC] " + _topic);
    emit topicChanged();
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

        // TODO

        return;
    }


    QString replacableFragment, newFragment;

    if (_completionFragment.length())
    {
        // The user already started cycling through the list of possible nicks, give the next one

        if (_possibleNickNames.count() <= 1)
            return;

        replacableFragment = _possibleNickNames[_currentCompletionIndex];
        if (_currentCompletionPosition == 0)
            replacableFragment += _autoCompletionSuffix;
        _currentCompletionIndex ++;

        if (_currentCompletionIndex >= _possibleNickNames.count())
            _currentCompletionIndex = 0;
    }
    else if (_currentMessage[_currentMessage.length() - 1] == ' ')
        return;
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

    _currentMessage.replace(_currentCompletionPosition, replacableFragment.length(), newFragment);
    emit currentMessageChanged();
}

void ChannelModel::fakeMessage()
{
    if (_channelText.length())
        _channelText += "<br />";

    setChannelText(_channelText += QTime::currentTime().toString("HH:mm") + " <span style='color: " + colorForNick("Zvdegor") + "'>" + "Zvdegor" + "</span>: " + QUuid::createUuid().toString());
}

void ChannelModel::backendDeleted()
{
    _backend = 0;
    deleteLater();
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
    else if (name().startsWith('#') && (commandParts[0] == "/part" || commandParts[0] == "/p"))
    {
        if (n == 1)
            static_cast<ServerModel*>(parent())->partChannel(name());
        else if (n == 2)
            static_cast<ServerModel*>(parent())->partChannel(commandParts[1]);
        else
            appendCommandInfo("Invalid command. Correct usage: '/part' or '/part &lt;channelname&gt'';");
    }
    else if (!name().startsWith('#') && commandParts[0] == "/close")
    {
        if (n == 1)
            static_cast<ServerModel*>(parent())->closeUser(name());
        else if (n == 2)
            static_cast<ServerModel*>(parent())->closeUser(commandParts[1]);
        else
            appendCommandInfo("Invalid command. Correct usage: '/close' or '/close &lt;username&gt'';");
    }
    else if (commandParts[0] == "/quit" || commandParts[0] == "/q")
    {
        if (n == 1)
            QCoreApplication::instance()->quit();
        else
            appendCommandInfo("Invalid command. Correct usage: '/quit'");
    }
    else if (commandParts[0] == "/me")
    {
        if (n > 1)
            _backend->ctcpAction(msg.mid(4));
        else
            appendCommandInfo("Invalid command. Correct usage: '/me &lt;message&gt;'");
    }
    else if (commandParts[0] == "/msg")
    {
        if (n > 2)
            appendCommandInfo("/msg is not supported yet, but planned to");
        else
            appendCommandInfo("Invalid command. Correct usage: '/msg &lt;username&gt; &lt;message&gt;'");
    }
    else if (commandParts[0] == "/nick")
    {
        if (n == 2)
            static_cast<ServerModel*>(parent())->changeNick(commandParts[1]);
        else
            appendCommandInfo("Invalid command. Correct usage: '/nick &lt;new nick&gt;' ");
    }
    else if (commandParts[0] == "/topic")
    {
        if (n == 1)
            appendCommandInfo("[TOPIC] " + _topic);
        else
            appendCommandInfo("Changing Topics is not supported yet!");
    }
    else
        appendCommandInfo("Unknown command, maybe it will be supported later?");
}
