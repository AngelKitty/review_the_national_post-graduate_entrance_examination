TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    TSP.cpp

include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    TSP.txt

