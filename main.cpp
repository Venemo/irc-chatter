#include <QtGui/QApplication>
#include <QtDeclarative>

#include "qobjectlistmodel.h"
#include "model/channelmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QObjectListModel<ChannelModel> list;
    list.addItem(new ChannelModel("#meego"));
    list.addItem(new ChannelModel("#harmattan"));
    list.addItem(new ChannelModel("#maemo"));
    list.addItem(new ChannelModel("w00t_"));

    QDeclarativeView view;
    view.rootContext()->setContextProperty("channelList", &list);
    view.setSource(QUrl("qrc:/qml/harmattan/main.qml"));
    view.showFullScreen();
    return app.exec();
}
