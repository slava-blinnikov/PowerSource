#ifndef OSCILLOSCOPEWIDGET_H
#define OSCILLOSCOPEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>

#include "Resources.h"
#include "Oscilloscope.h"

class OscilloscopeWidget : public QWidget
{
	Q_OBJECT
public:
	explicit OscilloscopeWidget(OscilloscopeRayList *rays, const OscilloscopeHandle &handle, QWidget *parent = nullptr);
	Oscilloscope *oscilloscope();

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	Oscilloscope *m_oscilloscope;
	QPushButton *m_btnClose;
	QCheckBox *m_chbVoltage;
	QCheckBox *m_chbVoltageTask;
	QCheckBox *m_chbCurrent;
	QCheckBox *m_chbCurrentTask;
	QCheckBox *m_chbTemperatureD;
	QCheckBox *m_chbTemperatureT;
	QSpinBox *m_spbGrid;
	QDoubleSpinBox *m_spbMin;
	QDoubleSpinBox *m_spbMax;
	QPushButton *m_btnApplyScale;

private slots:
	void closeOscilloscopeWidget();
	void toggleRayState(bool enabled);
	void applyScale();
	void onScaleEditChanged();

signals:
	void closeButtonClicked(QWidget *widget);
	void oscilloscopeChanged(QWidget *osl);
};

#endif // OSCILLOSCOPEWIDGET_H
