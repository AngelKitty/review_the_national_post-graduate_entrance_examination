TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    LightHouse.cpp

include(deployment.pri)
qtcAddDeployment()

