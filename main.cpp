#include <QtGui/QApplication>
#include <QtDeclarative>

#include "model/ircmodel.h"
#include "appsettings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    IrcModel model(&app);

    qmlRegisterUncreatableType<ServerSettings>("net.venemo.ircchatter", 1, 0, "ServerSettings", "This class is created in C++");
    qmlRegisterUncreatableType<AppSettings>("net.venemo.ircchatter", 1, 0, "AppSettings", "This class is created in C++, and only one instance is needed.");
    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<IrcModel>("net.venemo.ircchatter", 1, 0, "IrcModel", "This object is created in the model.");

    QDeclarativeView view;
    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    view.rootContext()->setContextProperty("ircModel", &model);
    view.setSource(QUrl("qrc:/qml/harmattan/main.qml"));
    view.showFullScreen();

    return app.exec();
}
