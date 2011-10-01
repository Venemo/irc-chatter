
QT += declarative network
symbian:TARGET.UID3 = 0xE3B586A7

HEADERS += \
    model/channelmodel.h \
    qobjectlistmodel.h \
    util.h \
    model/ircmodel.h \
    model/servermodel.h \
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

CONFIG += libircclient-qt
DEFINES += IRC_NO_DEPRECATED IRC_SHARED
INCLUDEPATH += /usr/include/ircclient-qt /usr/include/applauncherd

# enable booster
CONFIG += qdeclarative-boostable
QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

unix {
    INSTALLS += target icon desktopfile
    target.path=/usr/bin
    icon.files = installables/irc-chatter-harmattan-icon.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    desktopfile.files = installables/irc-chatter-harmattan.desktop
    desktopfile.path = /usr/share/applications
}

# for harmattan
INSTALLS += splashportrait splashlandscape
splashportrait.files = installables/irc-chatter-splash-harmattan-portrait.jpg
splashportrait.path = /usr/share/irc-chatter
splashlandscape.files = installables/irc-chatter-splash-harmattan-landscape.jpg
splashlandscape.path = /usr/share/irc-chatter
CONFIG += link_pkgconfig
PKGCONFIG += qdeclarative-boostable
LIBS += -licui18n
