
# Communi uses uchardet, so we need to add it here too

UCHARDET_DIR = communi/src/3rdparty/uchardet-0.0.1

INCLUDEPATH += $$UCHARDET_DIR/src
DEPENDPATH += $$UCHARDET_DIR/src

HEADERS += $$UCHARDET_DIR/src/uchardet.h

SOURCES += \
    $$UCHARDET_DIR/src/CharDistribution.cpp \
    $$UCHARDET_DIR/src/JpCntx.cpp \
    $$UCHARDET_DIR/src/LangBulgarianModel.cpp \
    $$UCHARDET_DIR/src/LangCyrillicModel.cpp \
    $$UCHARDET_DIR/src/LangGreekModel.cpp \
    $$UCHARDET_DIR/src/LangHungarianModel.cpp \
    $$UCHARDET_DIR/src/LangHebrewModel.cpp \
    $$UCHARDET_DIR/src/LangThaiModel.cpp \
    $$UCHARDET_DIR/src/nsHebrewProber.cpp \
    $$UCHARDET_DIR/src/nsCharSetProber.cpp \
    $$UCHARDET_DIR/src/nsBig5Prober.cpp \
    $$UCHARDET_DIR/src/nsEUCJPProber.cpp \
    $$UCHARDET_DIR/src/nsEUCKRProber.cpp \
    $$UCHARDET_DIR/src/nsEUCTWProber.cpp \
    $$UCHARDET_DIR/src/nsEscCharsetProber.cpp \
    $$UCHARDET_DIR/src/nsEscSM.cpp \
    $$UCHARDET_DIR/src/nsGB2312Prober.cpp \
    $$UCHARDET_DIR/src/nsMBCSGroupProber.cpp \
    $$UCHARDET_DIR/src/nsMBCSSM.cpp \
    $$UCHARDET_DIR/src/nsSBCSGroupProber.cpp \
    $$UCHARDET_DIR/src/nsSBCharSetProber.cpp \
    $$UCHARDET_DIR/src/nsSJISProber.cpp \
    $$UCHARDET_DIR/src/nsUTF8Prober.cpp \
    $$UCHARDET_DIR/src/nsLatin1Prober.cpp \
    $$UCHARDET_DIR/src/nsUniversalDetector.cpp \
    $$UCHARDET_DIR/src/uchardet.cpp

# These are the actual communi headers and source files that we need

HEADERS += \
    communi/include/irc.h \
    communi/include/irccommand.h \
    communi/include/ircmessage.h \
    communi/include/ircparser.h \
    communi/include/ircsender.h \
    communi/include/ircsession.h \
    communi/include/ircutil.h \
    communi/include/ircglobal.h

SOURCES += \
    communi/src/irc.cpp \
    communi/src/irccommand.cpp \
    communi/src/ircmessage.cpp \
    communi/src/ircparser.cpp \
    communi/src/ircsender.cpp \
    communi/src/ircsession.cpp \
    communi/src/ircutil.cpp \
    communi/src/ircdecoder.cpp

INCLUDEPATH += communi/include
