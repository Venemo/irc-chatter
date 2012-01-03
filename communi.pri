
HEADERS += \
    communi/include/irc.h \
    communi/include/irccommand.h \
    communi/include/ircencoder.h \
    communi/include/ircmessage.h \
    communi/include/ircparser.h \
    communi/include/ircsender.h \
    communi/include/ircsession.h \
    communi/include/ircutil.h \
    communi/include/ircglobal.h

SOURCES += \
    communi/src/irc.cpp \
    communi/src/irccommand.cpp \
    communi/src/ircencoder.cpp \
    communi/src/ircmessage.cpp \
    communi/src/ircparser.cpp \
    communi/src/ircsender.cpp \
    communi/src/ircsession.cpp \
    communi/src/ircutil.cpp

INCLUDEPATH += communi/include
include(communi/icu.pri)
