TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Tunel.cpp

include(deployment.pri)
qtcAddDeployment()

