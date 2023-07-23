#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QTimer>
#include <QShortcut>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <qwt_wheel.h>
#include <qwt_thermo.h>
#include <qwt_color_map.h>
#include <qwt_scale_engine.h>

#include "LBPDevice.h"
#include "Resources.h"
#include "DigitalLineEdit.h"
#include "CalibrationWidget.h"
#include "PlotDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(LBPDevice *device, QWidget *parent = nullptr);
	void showWindow();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	LBPDevice *m_device;
	CalibrationWidget *m_wdgCalibration;
	QPixmap m_pxmConnected;
	QPixmap m_pxmDisconnected;
	QLabel *m_lblConnected;
	QLabel *m_lblNoDacU;
	QLabel *m_lblNoDacI;
	QLabel *m_lblNoAdc;
	QLabel *m_lblTitleU;
	QLabel *m_lblU;
	QLabel *m_lblTitleI;
	QLabel *m_lblI;
	QLabel *m_lblApplyU;
	QLabel *m_lblApplyI;
	DigitalLineEdit *m_leTaskU;
	DigitalLineEdit *m_leTaskI;
	QwtThermo *m_trmTaskU;
	QwtWheel *m_whlTaskU;
	QwtThermo *m_trmTaskI;
	QwtWheel *m_whlTaskI;
	QPushButton *m_btnStart;
	QLabel *m_lblAlarm;
	QToolButton *m_btnPresets;
	QMenu *m_mnuPresets;
	QVector<QAction*> m_presetActions;
	QPushButton *m_btnCalibration;
	QPushButton *m_btnOnOffTrigger;
	QCheckBox *m_chbIStable;
	QLabel *m_lblTempTransistorIndicator;
	QLabel *m_lblTempTransistorTitle;
	QLabel *m_lblTempTransistorValue;
	QLabel *m_lblTempTransistorUnit;
	QLabel *m_lblTempDiodeIndicator;
	QLabel *m_lblTempDiodeTitle;
	QLabel *m_lblTempDiodeValue;
	QLabel *m_lblTempDiodeUnit;
	QWidget *m_wdgAccumulator;
	QLabel *m_lblAccCapacity;
	QLabel *m_lblAccCapacityUnit;
	QLabel *m_lblAccTimer;
	QPushButton *m_btnAccReset;
	QPushButton *m_btnAccShow;
	QPushButton *m_btnOptions;
	QTimer m_tmrTaskU;
	QTimer m_tmrTaskI;
	double m_taskU;
	double m_taskI;
	QPushButton *m_btnPlot;
	PlotDialog *m_plotDialog;
	QShortcut *m_shcOptions; //F4
	QShortcut *m_shcEditTaskU; //F5
	QShortcut *m_shcEditTaskI; //F6
	QShortcut *m_shcPresets; //F7
	QShortcut *m_shcTriggerMode; //F8
	QShortcut *m_shcStart; //F9
	QShortcut *m_shcAccumulator; //F10
	QShortcut *m_shcChargeReset; //F11
	QShortcut *m_shcCalibration; //F12
	void setEnabledButtons(bool enabled);
	void createPresetsMenu();
	double scaleStep(quint16 value);

private slots:
	void onUTaskTimeout();
	void onITaskTimeout();
	void onUTaskWheelChanged(double v);
	void onITaskWheelChanged(double v);
	void onUTaskEditChanged(double v);
	void onITaskEditChanged(double v);
	void editTaskU();
	void editTaskI();
	void toggleRunning();
	void toggleTrigger();
	void showPresetsDialog();
	void onPresetAction();
	void addStateToPresets();
	void onTaskEditActivated();
	void onAccumulatorAction();
	void onCalibrationAction();
	void showOptionsDialog();
	void showPlotDialog();
	void onPlotDialogClosed();
	void onDeviceConnected(const QString &portName);
	void onDeviceDisconnected();
	void onRunningStateChanged(bool isRunning);
	void onVoltageChanged(double value);
	void onCurrentChanged(double value);
	void onTaskUChanged(double value);
	void onTaskIChanged(double value);
	void onTriggerModeChanged(bool trigger);
	void onRegimeUChanged(quint16 value);
	void onRegimeIChanged(quint16 value);
	void onAlarm(bool ok);
	void onOverheatChanged(bool ok);
	void onOverloadChanged(bool ok);
	void onTemperatureTChanged(quint8 t);
	void onTemperatureDChanged(quint8 t);
	void onNoDacUAlarmChanged(bool ok);
	void onNoDacIAlarmChanged(bool ok);
	void onNoAdcAlarmChanged(bool ok);
	void onAccumulatorTimeChanged(quint8 hour, quint8 min, quint8 sec);
	void onAccumulatorCapacityChanged(double value);

signals:
	void controlTaskUChanged(double v);
	void controlTaskIChanged(double v);
	void controlRunningChanged();
	void controlTriggerModeChanged(bool trigger);
	void presetSelected(const PresetRecord &preset);
	void parameterSelected(int paramNo);
	void resetAccumulator();
	void portSettingsChanged();
	void requestIntervalChanged();
};

#endif // MAINWINDOW_H
