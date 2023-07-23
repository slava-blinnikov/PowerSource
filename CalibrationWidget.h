#ifndef CALIBRATIONWIDGET_H
#define CALIBRATIONWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>

#include "Resources.h"

class CalibrationWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CalibrationWidget(QWidget *parent = nullptr);
	void setEnableControls(bool enabled);
	void updateProtocol();

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	enum CalibrationParameterFlag
	{
		NoCalibrationParameterFlag = 0,
		DACAutocorrectionFlag = 0x0001,
		RegimeUFlag = 0x0002,
		CorrectionUrefFlag = 0x0004,
		CalibrationDacUFlag = 0x0008,
		CalibrationAdcUFlag = 0x0010,
		RegimeIFlag = 0x0020,
		CorrectionIrefFlag = 0x0040,
		CalibrationDacIFlag = 0x0080,
		CalibrationAdcIFlag = 0x0100,
		FanOnTFlag = 0x0200,
		PowerOffTFlag = 0x0400
	};
	QCheckBox *m_chbAutocorrDac;
	QLabel *m_lblRegimeU;
	QSpinBox *m_spbRegimeU; //20 - 80
	QLabel *m_lblCorrUref;
	QDoubleSpinBox *m_spbCorrUref; //-0.2 - 0.2
	QLabel *m_lblCalibrAdcU;
	QDoubleSpinBox *m_spbCalibrAdcU; //39.0 - 85.0
	QLabel *m_lblCalibrDacU;
	QSpinBox *m_spbCalibrDacU; //3000 - 4095
	QLabel *m_lblRegimeI;
	QSpinBox *m_spbRegimeI; //3 - 30
	QLabel *m_lblCorrIref;
	QDoubleSpinBox *m_spbCorrIref; //-0.5 - 0.5
	QLabel *m_lblCalibrAdcI;
	QDoubleSpinBox *m_spbCalibrAdcI; //2.5 - 33.0
	QLabel *m_lblCalibrDacI;
	QSpinBox *m_spbCalibrDacI; //3000 - 4095
	QLabel *m_lblTemperatureFanOn;
	QSpinBox *m_spbTemperatureFanOn; //0 - 70
	QLabel *m_lblTemperaturePowerOff;
	QSpinBox *m_spbTemperaturePowerOff; //30 - 100
	quint16 m_edited;
	QPushButton *m_btnApply;
	QPushButton *m_btnCancel;
	quint16 m_saveRegimeU;
	double m_saveCorrUref;
	double m_saveCalibrAdcU;
	int m_saveCalibrDacU;
	quint16 m_saveRegimeI;
	double m_saveCorrIref;
	double m_saveCalibrAdcI;
	int m_saveCalibrDacI;
	int m_saveTemperatureFanOn;
	int m_saveTemperaturePowerOff;

private slots:
	void onEditParameter();
	void cancelEdit();
	void applyEdit();
	void onControlReturnPressed(QWidget *control);
	void onControlEscapePressed(QWidget *control);

signals:
	void controlRegimeUChanged(quint16 value);
	void controlRegimeIChanged(quint16 value);
	void controlTaskUCorrectionChanged(double value);
	void controlCalibrationAdcUChanged(double value);
	void controlCalibrationDacUChanged(int value);
	void controlTaskICorrectionChanged(double value);
	void controlCalibrationAdcIChanged(double value);
	void controlCalibrationDacIChanged(int value);
	void controlFanOnTemperatureChanged(int value);
	void controlPowerOffTemperatureChanged(int value);
	void controlAutocorrectionDacChanged(bool value);
	void parameterSelected(int paramNo);

public slots:
	void onRegimeUChanged(quint16 value);
	void onRegimeIChanged(quint16 value);
	void onTaskUCorrectionChanged(double value);
	void onCalibrationAdcUChanged(double value);
	void onCalibrationDacUChanged(int value);
	void onTaskICorrectionChanged(double value);
	void onCalibrationAdcIChanged(double value);
	void onCalibrationDacIChanged(int value);
	void onFanOnTemperatureChanged(int value);
	void onPowerOffTemperatureChanged(int value);
	void onAutocorrectionDacChanged(bool value);
};

#endif // CALIBRATIONWIDGET_H
