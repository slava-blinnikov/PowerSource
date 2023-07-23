#--------------------------------------------------------------------------------------------------
#
# Project created by QtCreator 2019-04-22T15:40:19
# Автор: Вячеслав Блинников
# e-mail: slava.blinnikov@gmail.com
#
# Используемые сторонние библиотеки:
# Qwt v.6.1.3 https://sourceforge.net/projects/qwt
#
# Специально для проекта регулируемого блока питания Владимира Денисенко
# https://radiokot.ru/forum/viewtopic.php?f=11&t=134419
#
#--------------------------------------------------------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET		= PowerSource
TEMPLATE	= app
MOC_DIR		= ./moc
RCC_DIR		= ./moc
OBJECTS_DIR	= ./objects
DESTDIR		= ./release
RC_FILE		= mainicon.rc
RESOURCES	+= resources.qrc
DEFINES		-= QT_DEPRECATED_WARNINGS
CONFIG		+= c++11
TRANSLATIONS += PowerSource_ru.ts

HEADERS += \
    CalibrationWidget.h \
    DigitalLineEdit.h \
    LBPDevice.h \
    LBPDeviceWorker.h \
    NumberToolButton.h \
    OnscreenKeyboard.h \
    OptionsDialog.h \
    Oscilloscope.h \
    OscilloscopeColorsDialog.h \
    OscilloscopeWidget.h \
    PlotDialog.h \
    PresetsDialog.h \
    PresetsTableDelegate.h \
    PresetsTableModel.h \
    PresetsTableView.h \
    TypesAndConstants.h \
    Resources.h \
    MainWindow.h

SOURCES += main.cpp \
    CalibrationWidget.cpp \
    DigitalLineEdit.cpp \
    LBPDevice.cpp \
    LBPDeviceWorker.cpp \
    NumberToolButton.cpp \
    OnscreenKeyboard.cpp \
    OptionsDialog.cpp \
    Oscilloscope.cpp \
    OscilloscopeColorsDialog.cpp \
    OscilloscopeWidget.cpp \
    PlotDialog.cpp \
    PresetsDialog.cpp \
    PresetsTableDelegate.cpp \
    PresetsTableModel.cpp \
    PresetsTableView.cpp \
    Resources.cpp \
    MainWindow.cpp

OTHER_FILES = \
$${DESTDIR}/PowerSource.conf \
$${DESTDIR}/styles.css

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(../qwt.pri)

DISTFILES += \
    PowerSource_ru.ts
