
TARGET = irc-chatter
TEMPLATE = app
VERSION = 0.3.2
QT = core gui network

lessThan(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MAJOR_VERSION, 4) | lessThan(QT_MINOR_VERSION, 7) {
        error(IRC Chatter requires Qt 4.7 or newer but Qt $$[QT_VERSION] was detected.)
    }
    QT += declarative
} else {
    QT += widgets quick
}

DEFINES += \
    COMMUNI_STATIC \
    HAVE_ICU \
    APP_VERSION=\\\"$$VERSION\\\"

HEADERS += \
    helpers/util.h \
    helpers/appeventlistener.h \
    model/channelmodel.h \
    model/servermodel.h \
    model/ircmodel.h \
    helpers/qobjectlistmodel.h \
    model/settings/appsettings.h \
    model/settings/serversettings.h \
    clients/abstractircclient.h \
    clients/communiircclient.h \
    helpers/commandparser.h \
    helpers/channelhelper.h \
    helpers/notifier.h \
    model/channelmodelcollection.h

SOURCES += \
    main.cpp \
    helpers/appeventlistener.cpp \
    model/channelmodel.cpp \
    model/ircmodel.cpp \
    model/servermodel.cpp \
    model/settings/appsettings.cpp \
    model/settings/serversettings.cpp \
    clients/abstractircclient.cpp \
    clients/communiircclient.cpp \
    helpers/commandparser.cpp \
    helpers/channelhelper.cpp \
    helpers/notifier.cpp \
    helpers/qobjectlistmodel.cpp \
    model/channelmodelcollection.cpp

RESOURCES += \
    ui-meego.qrc \
    ui-desktop.qrc

# Licensing and other information
OTHER_FILES += \
    LICENSE \
    LICENSE-DOCS \
    AUTHORS \
    qml/desktop/misc/ChannelSwitcherButton.qml \
    qml/desktop/misc/ServerSettingEntry.qml \
    qml/desktop/components/Switch.qml \
    qml/desktop/components/Dialog.qml \
    qml/desktop/components/Menu.qml \
    qml/desktop/components/MenuButton.qml

# User interface for MeeGo
OTHER_FILES += \
    qml/meego/AppWindow.qml \
    qml/meego/components/TitleLabel.qml \
    qml/meego/components/WorkingSelectionDialog.qml \
    qml/meego/components/CommonDialog.qml \
    qml/meego/components/ServerSettingsList.qml \
    qml/meego/pages/StartPage.qml \
    qml/meego/pages/ChatPage.qml \
    qml/meego/pages/SettingsPage.qml \
    qml/meego/pages/ManageServersPage.qml \
    qml/meego/sheets/JoinSheet.qml \
    qml/meego/sheets/ServerSettingsSheet.qml

# User interface for desktop
OTHER_FILES += \
    qml/desktop/AppWindow.qml \
    qml/desktop/components/Button.qml \
    qml/desktop/components/Bubble.qml \
    qml/desktop/components/Sidebar.qml \
    qml/desktop/components/TextField.qml \
    qml/desktop/components/VerticalScrollBar.qml \
    qml/desktop/components/Page.qml \
    qml/desktop/pages/StartPage.qml \
    qml/desktop/pages/ChatPage.qml

CONFIG += meegotouch

unix {
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
    QT += dbus
    INSTALLS = target
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/Communi
    DEPENDPATH += $$[QT_INSTALL_HEADERS]/Communi
    LIBS += -lCommuni
    DEFINES += USE_DESKTOP_UI

    target.path=/usr/bin
    # TODO: create generic desktop and icon files
}

# for MeeGo 1.2 Harmattan
contains(MEEGO_EDITION, harmattan) {
    DEFINES += MEEGO_EDITION_HARMATTAN HAVE_APPLAUNCHERD HAVE_MNOTIFICATION
    CONFIG += qdeclarative-boostable link_pkgconfig
    # Note: you will need to have Communi installed in your sysroot for this to build correctly
    PKGCONFIG += qdeclarative-boostable
    INCLUDEPATH += /usr/include/applauncherd /usr/include/Communi
    LIBS += -lCommuni
    DEFINES += USE_MEEGO_UI DEFAULT_CHANNELS=\\\"#harmattan, #irc-chatter\\\"

    # IRC Chatter app
    target.path=/opt/irc-chatter
    # Icon
    icon.files = installables/harmattan/irc-chatter-harmattan-icon.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    desktopfile.files = installables/harmattan/irc-chatter-harmattan.desktop
    desktopfile.path = /usr/share/applications
    # Communi libraries
    communilib.files = installables/harmattan/libCommuni.so.1
    communilib.path = /opt/irc-chatter
    communiuchardetplugin.files = installables/harmattan/libuchardetplugin.so
    communiuchardetplugin.path = /opt/irc-chatter/plugins/communi
    # Portrait and landscape splash screens
    splashes.files = installables/harmattan/irc-chatter-splash-harmattan-portrait.jpg installables/harmattan/irc-chatter-splash-harmattan-landscape.jpg
    splashes.path = /usr/share/irc-chatter
    # Notification icons
    notifyicons.files = installables/harmattan/irc-chatter-harmattan-icon.png installables/harmattan/irc-chatter-harmattan-lpm-icon.png installables/harmattan/irc-chatter-harmattan-statusbar-icon.png
    notifyicons.path = /usr/share/themes/blanco/meegotouch/icons
    # Notification event type config
    notifyconfig.files = installables/harmattan/irc-chatter.irc.conf
    notifyconfig.path = /usr/share/meegotouch/notifications/eventtypes

    INSTALLS = target splashes notifyicons icon notifyconfig desktopfile communilib communiuchardetplugin


    contains(DEFINES, HARMATTAN_DEV) {
        INSTALLS = target communilib communiuchardetplugin
        target.path = /home/developer
        communilib.path = /home/developer
        communiuchardetplugin.path = /home/developer/plugins/communi
        QMAKE_LFLAGS += -Wl,--rpath=/home/developer,-O3
        message("Using the Harmattan development configuration.")
    } else {
        QMAKE_LFLAGS += -Wl,--rpath=/opt/irc-chatter,-O3
    }
}

QMAKE_CLEAN += Makefile build-stamp configure-stamp irc-chatter
