######################################################################
# Automatically generated by qmake (2.01a) ?? ???. 27 22:20:20 2013
######################################################################

QT       += core gui

TEMPLATE =  app
TARGET   =  Life
CONFIG   += debug_and_release

OBJECTS_DIR = obj/
MOC_DIR = moc/

# уменьшаем размер исполняемого файла
CONFIG(release, debug|release) {
    linux-g++ {
        QMAKE_CXXFLAGS_RELEASE += -Os -Wl,--strip-all # --gc-sections
    }
}

# Uncomment for profiler
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

# Input
SOURCES += main.cpp \
    src/mainwindow.cpp \
    src/fieldwidget.cpp \
    src/age.cpp \
    src/spinbox.cpp

HEADERS += src/mainwindow.h \
    src/fieldwidget.h \
    src/age.h \
    src/spinbox.h
