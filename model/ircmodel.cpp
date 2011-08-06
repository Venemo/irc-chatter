#include "ircmodel.h"

IrcModel::IrcModel(QObject *parent) :
    QObject(parent)
{
    _colors.append("#ff0000");
    _colors.append("#00ff00");
    _colors.append("#0000ff");
}

const QString &IrcModel::colorForNick(const QString &nick)
{
    int nickvalue = 0;

    for (int index = 0; index < nick.length(); index++)
        nickvalue += nick[index].unicode();

    return _colors[nickvalue % _colors.count()];
}

bool IrcModel::joinChannel(const QString &channelName)
{
    // TODO: join the channel in the backend
    return true;
}

bool IrcModel::queryUser(const QString &userName)
{
    // TODO: query the user in the backend
    return true;
}

void IrcModel::fillWithDummyData()
{
    ChannelModel *channel = new ChannelModel("#maemo", this);
    QStringList *userlist = new QStringList();
    userlist->append("Venemo");
    userlist->append("MohammadAG");
    userlist->append("alterego");
    channel->_users.setStringList(*userlist);
    _channelList.addItem(channel);

    channel = new ChannelModel("#meego", this);
    userlist = new QStringList();
    userlist->append("Stskeeps");
    userlist->append("lcuk");
    userlist->append("w00t_");
    channel->_users.setStringList(*userlist);
    _channelList.addItem(channel);

    channel = new ChannelModel("#harmattan", this);
    userlist = new QStringList();
    userlist->append("djszapi");
    userlist->append("hiemanshu");
    userlist->append("DocScrutinizer");
    channel->_users.setStringList(*userlist);
    _channelList.addItem(channel);

}
