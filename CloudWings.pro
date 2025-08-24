QT += core gui sql widgets printsupport

CONFIG += c++17

TARGET = CloudWings

# Application information
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "CloudWings Airlines"
QMAKE_TARGET_PRODUCT = "CloudWings Airline Management System"
QMAKE_TARGET_DESCRIPTION = "A comprehensive airline management solution"
QMAKE_TARGET_COPYRIGHT = "Copyright 2024 CloudWings Airlines"

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/loginwindow.cpp \
    src/adminwindow.cpp \
    src/passengerwindow.cpp \
    src/database.cpp \
    src/utils/helpers.cpp \
    src/utils/validators.cpp

# Header files
HEADERS += \
    include/mainwindow.h \
    include/loginwindow.h \
    include/adminwindow.h \
    include/passengerwindow.h \
    include/database.h \
    include/utils/helpers.h \
    include/utils/validators.h

# Include paths
INCLUDEPATH += \
    include \
    include/utils

# Forms
FORMS += \
    ui/adminwindow.ui \
    ui/passengerwindow.ui

# Resource files
RESOURCES += \
    resources/resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Platform-specific settings
win32 {
    VERSION = 1.0.0.0
}

macx {
    VERSION = 1.0.0
}

# Build configuration
CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
} else {
    DESTDIR = release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
}

# Compiler warnings
*g++* {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
}

*msvc* {
    QMAKE_CXXFLAGS += /W4
}

# Include stylesheet
QMAKE_POST_LINK += $$quote(cmd /c copy /y resources\\styles.qss $$shell_path($$shell_quote($$OUT_PWD)\\debug\\styles.qss))

# Windows specific
# RC_ICONS = resources/icon.ico

# macOS specific
# ICON = resources/icon.icns 