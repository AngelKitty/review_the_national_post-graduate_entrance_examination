TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    RangeSearch.cpp

include(deployment.pri)
qtcAddDeployment()

