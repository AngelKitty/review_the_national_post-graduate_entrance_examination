TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Zuma2.cpp

include(deployment.pri)
qtcAddDeployment()

