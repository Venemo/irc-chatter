#include <QtGui/QApplication>
#include <QtDeclarative>

#include "model/ircmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    IrcModel model(&app);
    model.connectToServer("chat.freenode.net", "Your_nick_here");

    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<IrcModel>("net.venemo.ircchatter", 1, 0, "IrcModel", "This object is created in the model.");

    QDeclarativeView view;
    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    view.rootContext()->setContextProperty("ircModel", &model);
    view.setSource(QUrl("qrc:/qml/harmattan/main.qml"));
    view.showFullScreen();

    return app.exec();
}
