
TARGET = irc-chatter
TEMPLATE = app
VERSION = 0.2.0
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
    serversettings.h

SOURCES += \
    main.cpp \
    model/channelmodel.cpp \
    model/ircmodel.cpp \
    model/servermodel.cpp \
    appsettings.cpp \
    serversettings.cpp

RESOURCES += \
    harmattan-gui.qrc

OTHER_FILES += \
    LICENSE \
    LICENSE-DOCS \
    AUTHORS \
    qml/harmattan/main.qml \
    qml/harmattan/ChatPage.qml \
    qml/harmattan/ConnectionSheet.qml \
    qml/harmattan/SettingsPage.qml \
    qml/harmattan/FirstrunPage.qml \
    qml/harmattan/JoinSheet.qml \
    qml/harmattan/ProgressPage.qml \
    qml/harmattan/TitleLabel.qml \
    qml/harmattan/WorkingSelectionDialog.qml \
    qml/harmattan/CommonDialog.qml

unix {
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic

    INSTALLS += target icon desktopfile
    target.path=/usr/bin
    icon.files = installables/irc-chatter-harmattan-icon.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    desktopfile.files = installables/irc-chatter-harmattan.desktop
    desktopfile.path = /usr/share/applications

    contains(DEFINES, PRERELEASE) {
        icon.files = installables/irc-chatter-harmattan-prerelease-icon.png
        desktopfile.files = installables/irc-chatter-harmattan-prerelease.desktop
    }
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

symbian {
    TARGET.UID3 = 0xE3B586A7
}
