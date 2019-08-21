TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    train.cpp

include(deployment.pri)
qtcAddDeployment()

