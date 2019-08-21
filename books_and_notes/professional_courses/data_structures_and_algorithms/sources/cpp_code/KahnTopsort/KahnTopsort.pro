TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    kahntopsortTSP.cpp

include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    TSP.txt

