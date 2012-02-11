// This file is part of IRC Chatter, the first IRC Client for MeeGo.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Copyright (C) 2011, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QSettings>
#include <MDeclarativeCache>

#include "model/ircmodel.h"
#include "appsettings.h"

class AppFocusFilter : public QObject
{
public:
    AppFocusFilter(IrcModel *parent) : QObject((QObject*)parent) { }

    bool eventFilter(QObject *obj, QEvent *event)
    {
        Q_UNUSED(obj);

        if (event->type() == QEvent::FocusIn)
        {
            static_cast<IrcModel*>(parent())->setIsAppInFocus(true);
        }
        else if (event->type() == QEvent::FocusOut)
        {
            static_cast<IrcModel*>(parent())->setIsAppInFocus(false);
        }
        return false;
    }
};

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QString appVersion(APP_VERSION);
    bool isPreRelease = false;

#if defined(PRERELEASE)
    appVersion += " (Unstable pre-release)";
    isPreRelease = true;
#endif

    QApplication::setApplicationName("irc-chatter");
    QApplication::setOrganizationName("irc-chatter");
    QApplication::setApplicationVersion(APP_VERSION);

    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
    IrcModel *model = new IrcModel(app);
    AppSettings *settings = new AppSettings(app);
    app->installEventFilter(new AppFocusFilter(model));

    if (!settings->serverSettingsCount())
        settings->appendServerSettings(new ServerSettings(settings));

    qmlRegisterType<ServerSettings>("net.venemo.ircchatter", 1, 0, "ServerSettings");
    qmlRegisterUncreatableType<AppSettings>("net.venemo.ircchatter", 1, 0, "AppSettings", "This class is created in C++, and only one instance is needed.");
    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<IrcModel>("net.venemo.ircchatter", 1, 0, "IrcModel", "This object is created in the model.");

    QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();
    QObject::connect(view->engine(), SIGNAL(quit()), app, SLOT(quit()));
    view->setWindowTitle("IRC Chatter");
    view->rootContext()->setContextProperty("ircModel", model);
    view->rootContext()->setContextProperty("appSettings", settings);
    view->rootContext()->setContextProperty("appVersion", appVersion);
    view->rootContext()->setContextProperty("isPreRelease", isPreRelease);
    view->setSource(QUrl("qrc:/qml/harmattan/main.qml"));
    view->showFullScreen();

    int result = app->exec();
    delete view;
    delete app;
    return result;
}
