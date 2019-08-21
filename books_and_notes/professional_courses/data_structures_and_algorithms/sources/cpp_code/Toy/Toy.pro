TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    toy.cpp

include(deployment.pri)
qtcAddDeployment()

