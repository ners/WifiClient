HEADERS = $$files(src/*.h, true)
SOURCES = $$files(src/*.cpp, true)
RESOURCES = build/application.qrc
INCLUDEPATH = src
TRANSLATIONS = $$files(languages/*.ts)
CONFIG += c++17 gui
QT += qml
