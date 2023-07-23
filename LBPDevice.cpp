#include "LBPDevice.h"

LBPDevice::LBPDevice(QObject *parent) : QObject(parent)
{
	m_thread = nullptr;
	m_worker = nullptr;
}

bool LBPDevice::isActive()
{
	return (m_thread != nullptr) && (m_worker != nullptr);
}

void LBPDevice::onThreadFinished()
{
	m_thread = nullptr;
	m_worker = nullptr;
}

void LBPDevice::start()
{
	if (!isActive())
	{
		m_thread = new QThread();
		m_worker = new LBPDeviceWorker();
		m_worker->moveToThread(m_thread);
		connect(m_thread, SIGNAL(started()), m_worker, SLOT(connectToPort()));
		connect(m_worker, SIGNAL(disconnected()), m_worker, SLOT(connectToPort()), Qt::QueuedConnection);
		connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
		connect(m_thread, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
		connect(m_thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));

		connect(m_worker, SIGNAL(connected(QString)), this, SIGNAL(connected(QString)));
		connect(m_worker, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
		connect(m_worker, SIGNAL(runningState(bool)), this, SIGNAL(runningState(bool)));
		connect(m_worker, SIGNAL(voltage(double)), this, SIGNAL(voltage(double)));
		connect(m_worker, SIGNAL(current(double)), this, SIGNAL(current(double)));
		connect(m_worker, SIGNAL(taskU(double)), this, SIGNAL(taskU(double)));
		connect(m_worker, SIGNAL(taskI(double)), this, SIGNAL(taskI(double)));
		connect(m_worker, SIGNAL(triggerMode(bool)), this, SIGNAL(triggerMode(bool)));
		connect(m_worker, SIGNAL(regimeU(quint16)), this, SIGNAL(regimeU(quint16)));
		connect(m_worker, SIGNAL(regimeI(quint16)), this, SIGNAL(regimeI(quint16)));
		connect(m_worker, SIGNAL(alarm(bool)), this, SIGNAL(alarm(bool)));
		connect(m_worker, SIGNAL(overheat(bool)), this, SIGNAL(overheat(bool)));
		connect(m_worker, SIGNAL(overload(bool)), this, SIGNAL(overload(bool)));
		connect(m_worker, SIGNAL(temperatureT(quint8)), this, SIGNAL(temperatureT(quint8)));
		connect(m_worker, SIGNAL(temperatureD(quint8)), this, SIGNAL(temperatureD(quint8)));
		connect(m_worker, SIGNAL(noDacU(bool)), this, SIGNAL(noDacU(bool)));
		connect(m_worker, SIGNAL(noDacI(bool)), this, SIGNAL(noDacI(bool)));
		connect(m_worker, SIGNAL(noAdc(bool)), this, SIGNAL(noAdc(bool)));
		connect(m_worker, SIGNAL(taskUCorrection(double)), this, SIGNAL(taskUCorrection(double)));
		connect(m_worker, SIGNAL(calibrationAdcU(double)), this, SIGNAL(calibrationAdcU(double)));
		connect(m_worker, SIGNAL(calibrationDacU(int)), this, SIGNAL(calibrationDacU(int)));
		connect(m_worker, SIGNAL(taskICorrection(double)), this, SIGNAL(taskICorrection(double)));
		connect(m_worker, SIGNAL(calibrationAdcI(double)), this, SIGNAL(calibrationAdcI(double)));
		connect(m_worker, SIGNAL(calibrationDacI(int)), this, SIGNAL(calibrationDacI(int)));
		connect(m_worker, SIGNAL(fanOnTemperature(int)), this, SIGNAL(fanOnTemperature(int)));
		connect(m_worker, SIGNAL(powerOffTemperature(int)), this, SIGNAL(powerOffTemperature(int)));
		connect(m_worker, SIGNAL(autocorrectionDac(bool)), this, SIGNAL(autocorrectionDac(bool)));
		connect(m_worker, SIGNAL(accumulatorTime(quint8,quint8,quint8)), this, SIGNAL(accumulatorTime(quint8,quint8,quint8)));
		connect(m_worker, SIGNAL(accumulatorCapacity(double)), this, SIGNAL(accumulatorCapacity(double)));

		void stabilization(bool value);

		m_thread->start();
	}
}

void LBPDevice::stop()
{
	if (isActive()) QMetaObject::invokeMethod(m_thread, "quit", Qt::QueuedConnection);
}

void LBPDevice::requestSwitchRunning()
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSwitchRunning", Qt::QueuedConnection);
}

void LBPDevice::requestSetUTask(double value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetUTask", Qt::QueuedConnection, Q_ARG(double, value));
}

void LBPDevice::requestSetITask(double value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetITask", Qt::QueuedConnection, Q_ARG(double, value));
}

void LBPDevice::requestSetTriggerMode(bool trigger)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetTriggerMode", Qt::QueuedConnection, Q_ARG(bool, trigger));
}

void LBPDevice::requestApplyPreset(const PresetRecord &preset)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestApplyPreset", Qt::QueuedConnection, Q_ARG(PresetRecord, preset));
}

void LBPDevice::requestSetRegimeU(quint16 value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetRegimeU", Qt::QueuedConnection, Q_ARG(quint16, value));
}

void LBPDevice::requestSetRegimeI(quint16 value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetRegimeI", Qt::QueuedConnection, Q_ARG(quint16, value));
}

void LBPDevice::requestSetTaskUCorrection(double value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetTaskUCorrection", Qt::QueuedConnection, Q_ARG(double, value));
}

void LBPDevice::requestSetCalibrationAdcU(double value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetCalibrationAdcU", Qt::QueuedConnection, Q_ARG(double, value));
}

void LBPDevice::requestSetCalibrationDacU(int value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetCalibrationDacU", Qt::QueuedConnection, Q_ARG(int, value));
}

void LBPDevice::requestSetTaskICorrection(double value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetTaskICorrection", Qt::QueuedConnection, Q_ARG(double, value));
}

void LBPDevice::requestSetCalibrationAdcI(double value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetCalibrationAdcI", Qt::QueuedConnection, Q_ARG(double, value));
}

void LBPDevice::requestSetCalibrationDacI(int value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetCalibrationDacI", Qt::QueuedConnection, Q_ARG(int, value));
}

void LBPDevice::requestSetFanOnTemperature(int value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetFanOnTemperature", Qt::QueuedConnection, Q_ARG(int, value));
}

void LBPDevice::requestSetPowerOffTemperature(int value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetPowerOffTemperature", Qt::QueuedConnection, Q_ARG(int, value));
}

void LBPDevice::requestSetAutocorrectionDac(bool value)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestSetAutocorrectionDac", Qt::QueuedConnection, Q_ARG(bool, value));
}

void LBPDevice::requestMoveToParameter(int paramNo)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestMoveToParameter", Qt::QueuedConnection, Q_ARG(int, paramNo));
}

void LBPDevice::requestResetAccumulator()
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "requestResetAccumulator", Qt::QueuedConnection);
}

void LBPDevice::onPortSettionsChanged()
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "onPortSettionsChanged", Qt::QueuedConnection);
}

void LBPDevice::onRequestIntervalChanged()
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "onRequestIntervalChanged", Qt::QueuedConnection);
}

void LBPDevice::startOscilloscope(quint8 rays)
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "startOscilloscope", Qt::QueuedConnection, Q_ARG(quint8, rays));
}

void LBPDevice::stopOscilloscope()
{
	if (isActive()) QMetaObject::invokeMethod(m_worker, "stopOscilloscope", Qt::QueuedConnection);
}
