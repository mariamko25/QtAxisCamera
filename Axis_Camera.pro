#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T08:30:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Axis_Camera
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        cameraaxis.cpp

HEADERS += \
        cameraaxis.h

FORMS += \
        cameraaxis.ui
# Additional configs we had to make in order to make de project work
# We also added the path to pkg-config in PATH "usr/locan/bin" in project "environnement de compilation"
# We added a new variable "PKG_CONFIG_PATH" and its value to "/usr/local/Cellar/opencv/3.3.0_3/lib/pkgconfig/opencv.pcR"
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv

# This is a resource file we added an image to it that we wanted to use
RESOURCES += \
    images.qrc

DISTFILES += \
    record1.png

