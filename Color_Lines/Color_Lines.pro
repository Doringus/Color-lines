QT += quick sql

CONFIG += c++17


SOURCES += \
        gameloader.cpp \
        gamesaver.cpp \
        gametable.cpp \
        gametablemodel.cpp \
        main.cpp \
        randomgenerator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    gameloader.h \
    gamesaver.h \
    gametable.h \
    gametablemodel.h \
    pathfinder.h \
    randomgenerator.h
