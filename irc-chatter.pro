
TARGET = irc-chatter
TEMPLATE = app
VERSION = 0.2.90
QT += core declarative network

DEFINES += \
    COMMUNI_STATIC \
    HAVE_ICU \
    APP_VERSION=\\\"$$VERSION\\\"

include(communi.pri)

HEADERS += \
    util.h \
    model/channelmodel.h \
    model/servermodel.h \
    model/ircmodel.h \
    qobjectlistmodel.h \
    appsettings.h \
    serversettings.h \
    model/clients/abstractircclient.h \
    model/clients/communiircclient.h \
    model/helpers/commandparser.h \
    model/helpers/channelhelper.h \
    model/helpers/notifier.h

SOURCES += \
    main.cpp \
    model/channelmodel.cpp \
    model/ircmodel.cpp \
    model/servermodel.cpp \
    appsettings.cpp \
    serversettings.cpp \
    model/clients/abstractircclient.cpp \
    model/clients/communiircclient.cpp \
    model/helpers/commandparser.cpp \
    model/helpers/channelhelper.cpp \
    model/helpers/notifier.cpp

RESOURCES += \
    ui-meego.qrc

OTHER_FILES += \
    LICENSE \
    LICENSE-DOCS \
    AUTHORS \
    qml/meego/AppWindow.qml \
    qml/meego/components/TitleLabel.qml \
    qml/meego/components/WorkingSelectionDialog.qml \
    qml/meego/components/CommonDialog.qml \
    qml/meego/components/ServerSettingsList.qml \
    qml/meego/pages/StartPage.qml \
    qml/meego/pages/ProgressPage.qml \
    qml/meego/pages/ChatPage.qml \
    qml/meego/pages/SettingsPage.qml \
    qml/meego/pages/ManageServersPage.qml \
    qml/meego/sheets/JoinSheet.qml \
    qml/meego/sheets/ServerSettingsSheet.qml

CONFIG += meegotouch

unix {
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic

    INSTALLS += target icon desktopfile
    target.path=/usr/bin
    icon.files = installables/irc-chatter-harmattan-icon.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    desktopfile.files = installables/irc-chatter-harmattan.desktop
    desktopfile.path = /usr/share/applications
}

contains(MEEGO_EDITION, harmattan) {
    # for Harmattan
    CONFIG += qdeclarative-boostable link_pkgconfig
    PKGCONFIG += qdeclarative-boostable
    INCLUDEPATH += /usr/include/applauncherd

    INSTALLS += splashportrait splashlandscape
    splashportrait.files = installables/irc-chatter-splash-harmattan-portrait.jpg
    splashportrait.path = /usr/share/irc-chatter
    splashlandscape.files = installables/irc-chatter-splash-harmattan-landscape.jpg
    splashlandscape.path = /usr/share/irc-chatter
}
