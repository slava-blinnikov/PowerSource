#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTimerEvent>
#include <QStatusBar>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include <QLabel>

#include <QtMath>
#include <QTimer>
#include <QThread>

#include "OscilloscopeWidget.h"
#include "OscilloscopeColorsDialog.h"

//#define PWRSRC_TEST_SIGNAL

#ifdef PWRSRC_TEST_SIGNAL
class TestSignalUnitWorker : public QObject
{
	Q_OBJECT
public:
	TestSignalUnitWorker(QObject *parent = nullptr);
	void setDepth(qint64 depth);

private:
	QTimer *m_timer;
	double m_period;

public slots:
	void start();
	void stop();

private slots:
	void timeout();

signals:
	void voltage(double value);
	void current(double value);
	void taskU(double value);
	void taskI(double value);
	void temperatureT(quint8 t);
	void temperatureD(quint8 t);
};

class TestSignalUnit : public QObject
{
	Q_OBJECT
public:
	TestSignalUnit(QObject *parent = nullptr);
	void setDepth(qint64 depth);

private:
	qint64 m_depth;
	TestSignalUnitWorker *m_worker;
	QThread *m_thread;

public slots:
	void start();
	void stop();

private slots:
	void onThreadFinished();

signals:
	void voltage(double value);
	void current(double value);
	void taskU(double value);
	void taskI(double value);
	void temperatureT(quint8 t);
	void temperatureD(quint8 t);
};
#endif

class PlotDialog : public QDialog
{
	Q_OBJECT
public:
	explicit PlotDialog(QWidget *parent = nullptr);
	~PlotDialog() override;

protected:
	void closeEvent(QCloseEvent* event) override;
	void timerEvent(QTimerEvent *event) override;
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
#ifdef PWRSRC_TEST_SIGNAL
	TestSignalUnit test;
#endif
	QVBoxLayout *m_vblPlots;
	QVector<Oscilloscope *> m_plots;
	OscilloscopeRayList m_rays;
	quint8 m_rayFlags;
	qint64 m_depth;
	int m_timer_id;
	bool m_changed;
	QPushButton *m_btnAddOscilloscope;
	QSpinBox *m_spbGrid;
	QSpinBox *m_spbMinutes;
	QSpinBox *m_spbSeconds;
	QPushButton *m_btnApplyDepth;
	QPushButton *m_btnColors;
	OscilloscopeColorsDialog *m_dlgColours;
	void truncToDepth(OscilloscopeRayData &data);
	void truncRays();
	void updatePlots();
	void invalidateRayFlags();
	void invalidateRayData(OscilloscopeRay flag);
	void updateColors();

private slots:
	void closeOscilloscopeWidget(QWidget *widget);
	void onOscilloscopeChanged(QWidget *oslw);
	void onRayFlagsChanged();
	void addOscilloscope();
	void applyDepth();
	void onDepthEditChanged();
	void showColorsDialog();
	void onColorsDialogClosed();
	void onColorsChanged();

signals:
	void dialogClosed();

public slots:
	void onVoltageChanged(double value);
	void onCurrentChanged(double value);
	void onTaskUChanged(double value);
	void onTaskIChanged(double value);
	void onTemperatureTChanged(quint8 t);
	void onTemperatureDChanged(quint8 t);
};

#endif // PLOTDIALOG_H
