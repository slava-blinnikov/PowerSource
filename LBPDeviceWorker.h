#ifndef LBPDEVICEWORKER_H
#define LBPDEVICEWORKER_H

#include <QObject>
#include <QTimerEvent>
#include <QQueue>

#include "Resources.h"

class LBPDeviceWorker : public QObject
{
	Q_OBJECT
public:
	explicit LBPDeviceWorker(QObject *parent = nullptr);
	~LBPDeviceWorker();

protected:
	void timerEvent(QTimerEvent *event);

private:
	QSerialPort *m_port;
	int m_reconnectTimer;
	int m_requestTimer;
	bool m_stateInited;
	PowerStateHandle m_state;
	int m_bigPacketSize;
	void clearConnection();
	bool waitForAnswer(int count, QByteArray &answer);
	bool handshake(QSerialPort *port);

private slots:
	void requestPowerState();

public slots:
	void connectToPort();
	void requestSwitchRunning();
	void requestSetUTask(double value);
	void requestSetITask(double value);
	void requestSetTriggerMode(bool trigger);
	void requestApplyPreset(const PresetRecord &preset);
	void requestSetRegimeU(quint16 value);
	void requestSetRegimeI(quint16 value);
	void requestSetTaskUCorrection(double value);
	void requestSetCalibrationAdcU(double value);
	void requestSetCalibrationDacU(int value);
	void requestSetTaskICorrection(double value);
	void requestSetCalibrationAdcI(double value);
	void requestSetCalibrationDacI(int value);
	void requestSetFanOnTemperature(int value);
	void requestSetPowerOffTemperature(int value);
	void requestSetAutocorrectionDac(bool value);
	void requestMoveToParameter(int paramNo);
	void requestResetAccumulator();
	void onPortSettionsChanged();
	void onRequestIntervalChanged();

signals:
	void connected(const QString &portName);
	void disconnected();
	void runningState(bool isRunning);
	void voltage(double value);
	void current(double value);
	void taskU(double value);
	void taskI(double value);
	void triggerMode(bool trigger);
	void regimeU(quint16 value);
	void regimeI(quint16 value);
	void alarm(bool ok);
	void overheat(bool ok);
	void overload(bool ok);
	void temperatureT(quint8 t);
	void temperatureD(quint8 t);
	void noDacU(bool ok);
	void noDacI(bool ok);
	void noAdc(bool ok);
	void taskUCorrection(double value);
	void calibrationAdcU(double value);
	void calibrationDacU(int value);
	void taskICorrection(double value);
	void calibrationAdcI(double value);
	void calibrationDacI(int value);
	void fanOnTemperature(int value);
	void powerOffTemperature(int value);
	void autocorrectionDac(bool value);
	void accumulatorTime(quint8 hour, quint8 min, quint8 sec);
	void accumulatorCapacity(double value);
};

#endif // LBPDEVICEWORKER_H
