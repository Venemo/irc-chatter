
QT += declarative
symbian:TARGET.UID3 = 0xE3B586A7

HEADERS += \
    model/channelmodel.h \
    qobjectlistmodel.h \
    util.h \
    model/messagemodel.h \
    model/ircmodel.h

SOURCES += \
    main.cpp \
    model/channelmodel.cpp \
    model/messagemodel.cpp \
    model/ircmodel.cpp

RESOURCES += \
    harmattan-gui.qrc

OTHER_FILES += \
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

# enable booster
CONFIG += qdeclarative-boostable
QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic

INSTALLS += target
INSTALLS -= desktopfile icon
target.path=/home/developer










