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
// Copyright (C) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>
// Copyright (C) 2011, Hiemanshu Sharma <mail@theindiangeek.in>

#include <QtCore/QSettings>

#if QT_VERSION >= 0x050000
#include <QtGui/QGuiApplication>
#include <QtQuick/QtQuick>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#if defined(USE_DESKTOP_UI)
#include <QtWidgets/QApplication>
#endif
#else
#include <QtGui/QApplication>
#include <QtDeclarative/QtDeclarative>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#endif

#include "helpers/appeventlistener.h"
#include "model/ircmodel.h"
#include "model/settings/appsettings.h"

#if defined(HAVE_APPLAUNCHERD)
#include <MDeclarativeCache>
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QString appVersion(APP_VERSION);
    bool isPreRelease = false;

#if defined(PRERELEASE)
    appVersion += " (Unstable pre-release)";
    isPreRelease = true;
#endif

    QCoreApplication::setApplicationName("irc-chatter");
    QCoreApplication::setOrganizationName("irc-chatter");
    QCoreApplication::setApplicationVersion(APP_VERSION);

#if defined(HAVE_APPLAUNCHERD)
    qDebug() << "IRC Chatter is using applauncherd";
    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
    QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();
#elif QT_VERSION >= 0x050000 && defined(USE_DESKTOP_UI)
    QApplication *app = new QApplication(argc, argv);
    QQuickView *view = new QQuickView();
#elif QT_VERSION >= 0x050000
    QGuiApplication *app = new QGuiApplication(argc, argv);
    QQuickView *view = new QQuickView();
#else
    QApplication *app = new QApplication(argc, argv);
    QDeclarativeView *view = new QDeclarativeView();
#endif
    QCoreApplication::addLibraryPath("./plugins");

    AppSettings *appSettings = new AppSettings(app);
    IrcModel *model = new IrcModel(app, appSettings);
    AppEventListener *eventListener = new AppEventListener(model);
    app->installEventFilter(eventListener);
    qDebug() << "QApplication, QDeclarativeView, IrcModel, AppEventListener instances created";

    qmlRegisterType<ServerSettings>("net.venemo.ircchatter", 1, 0, "ServerSettings");
    qmlRegisterType<AppSettings>("net.venemo.ircchatter", 1, 0, "AppSettings");
    qmlRegisterUncreatableType<ChannelModel>("net.venemo.ircchatter", 1, 0, "ChannelModel", "This object is created in the model.");
    qmlRegisterUncreatableType<IrcModel>("net.venemo.ircchatter", 1, 0, "IrcModel", "This object is created in the model.");
    qDebug() << "QML types registered";

    QObject::connect(eventListener, SIGNAL(applicationActivated()), view, SLOT(raise()));
    QObject::connect(app, SIGNAL(aboutToQuit()), appSettings, SLOT(saveServerSettings()));
    QObject::connect(view->engine(), SIGNAL(quit()), app, SLOT(quit()));
#if QT_VERSION >= 0x050000
    view->setTitle("IRC Chatter");
#else
    view->setWindowTitle("IRC Chatter");
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);
#endif
    view->rootContext()->setContextProperty("ircModel", model);
    view->rootContext()->setContextProperty("appVersion", appVersion);
    view->rootContext()->setContextProperty("appSettings", appSettings);
    view->rootContext()->setContextProperty("isPreRelease", isPreRelease);
    qDebug() << "View set up";

#if defined(USE_MEEGO_UI)
    view->setSource(QUrl("qrc:/qml/meego/AppWindow.qml"));
    view->showFullScreen();
#elif defined(USE_DESKTOP_UI)
    if (view->screen()->size().width() > 1024)
        view->setMinimumSize(QSize(view->screen()->size().width() * 2 / 3, view->screen()->size().height() * 2 / 3));
    else
        view->setMinimumSize(QSize(800, 480));
    view->setWidth(view->minimumWidth());
    view->setHeight(view->minimumHeight());
    view->setPosition(view->screen()->size().width() / 2 - view->width() / 2, view->screen()->size().height() / 2 - view->height() / 2);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/qml/desktop/AppWindow.qml"));
    view->show();
#else
#error Please configure your build properly and select a UI.
#endif

    qDebug() << "View shown";

    int result = app->exec();
    delete view;
    delete app;
    return result;
}
