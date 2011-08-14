#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QSettings>

#include "model/ircmodel.h"
#include "appsettings.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication::setApplicationName("irc-chatter");
    QApplication::setOrganizationName("Venemo");

    QApplication app(argc, argv);
    IrcModel model(&app);
    AppSettings settings(&app);

    if (!settings.serverSettingsCount())
        settings.appendServerSettings(new ServerSettings(&settings));

    qmlRegisterType<ServerSettings>("net.venemo.ircchatter", 1, 0, "ServerSettings");
    qmlRegisterUncreatableType<AppSettings>("net.venemo.ircchatter", 1, 0, "AppSettings", "This class is created in C++, and only one instance is needed.");
    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<IrcModel>("net.venemo.ircchatter", 1, 0, "IrcModel", "This object is created in the model.");

    QDeclarativeView view;
    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    view.rootContext()->setContextProperty("ircModel", &model);
    view.rootContext()->setContextProperty("appSettings", &settings);
    view.setSource(QUrl("qrc:/qml/harmattan/main.qml"));
    view.showFullScreen();

    return app.exec();
}
