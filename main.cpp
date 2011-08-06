#include <QtGui/QApplication>
#include <QtDeclarative>

#include "model/ircmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    IrcModel model(&app);
    model.fillWithDummyData();

    qmlRegisterType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel");
    qmlRegisterType<MessageModel>("net.venemo.ircchatter", 1, 0, "MessageModel");
    qmlRegisterType<IrcModel>("net.venemo.ircchatter", 1, 0, "IrcModel");

    QDeclarativeView view;
    view.rootContext()->setContextProperty("ircModel", &model);
    view.setSource(QUrl("qrc:/qml/harmattan/main.qml"));
    view.showFullScreen();

    return app.exec();
}
