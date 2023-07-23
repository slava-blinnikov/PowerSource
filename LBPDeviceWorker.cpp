#include <QThread>
#include <QDateTime>

#include <QSerialPortInfo>

#include "LBPDeviceWorker.h"

LBPDeviceWorker::LBPDeviceWorker(QObject *parent) : QObject(parent)
{
	m_port = nullptr;
	m_reconnectTimer = 0;
	m_requestTimer = 0;
	m_stateInited = false;
}

LBPDeviceWorker::~LBPDeviceWorker()
{
	clearConnection();
}

void LBPDeviceWorker::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_requestTimer) requestPowerState();
	else if (event->timerId() == m_reconnectTimer) connectToPort();
	else QObject::timerEvent(event);
}

void LBPDeviceWorker::clearConnection()
{
	m_stateInited = false;
	if (m_reconnectTimer != 0)
	{
		killTimer(m_reconnectTimer);
		m_reconnectTimer = 0;
	}
	if (m_requestTimer != 0)
	{
		killTimer(m_requestTimer);
		m_requestTimer = 0;
	}
	if (m_port != nullptr)
	{
		delete m_port;
		m_port = nullptr;
	}
}

bool LBPDeviceWorker::waitForAnswer(int count, QByteArray &answer)
{
	int n = 0;
	while (n < count)
	{
		if (m_port->waitForReadyRead(Resources::portOptions.ioTimeout))
		{
			answer.append(m_port->readAll());
			n = answer.size();
		}
		else
		{
			clearConnection();
			emit disconnected();
			return false;
		}
	}
	return true;
}

bool LBPDeviceWorker::handshake(QSerialPort *port)
{
	if (port->open(QIODevice::ReadWrite))
	{
		RequestType request;
		request.arr[0] = 'u';
		port->write(request.arr, 4);
		if (port->waitForBytesWritten(Resources::portOptions.ioTimeout))
		{
			if (port->waitForReadyRead(Resources::portOptions.ioTimeout))
			{
				QByteArray ans = port->readAll();
				if (ans.size() >= 3)
				{
					const char *d = ans.constData();
					if ((d[0] == 'L') && (d[1] == 'B') && (d[2] == 'P')) return true;
				}
			}
		}
	}
	return false;
}

void LBPDeviceWorker::connectToPort()
{
	clearConnection();
	QSerialPort *port = nullptr;
	bool ok = false;
	if (Resources::portOptions.portAutosearch)
	{
		QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
		foreach (QSerialPortInfo info, ports)
		{
			port = new QSerialPort();
			port->setBaudRate(Resources::portOptions.baudrate);
			port->setFlowControl(Resources::portOptions.flowcontrol);
			port->setParity(Resources::portOptions.parity);
			port->setDataBits(Resources::portOptions.databits);
			port->setStopBits(Resources::portOptions.stopbits);
			port->setPortName(info.portName());
			ok = handshake(port);
			if (ok) break;
			else
			{
				delete port;
				port = nullptr;
			}
		}
	}
	else
	{
		port = new QSerialPort();
		port->setBaudRate(Resources::portOptions.baudrate);
		port->setFlowControl(Resources::portOptions.flowcontrol);
		port->setParity(Resources::portOptions.parity);
		port->setDataBits(Resources::portOptions.databits);
		port->setStopBits(Resources::portOptions.stopbits);
		port->setPortName(Resources::portOptions.port);
		ok = handshake(port);
		if (!ok)
		{
			delete port;
			port = nullptr;
		}
	}
	if (ok && (port != nullptr))
	{
		m_port = port;
		m_requestTimer = startTimer(Resources::portOptions.reqestInterval);
		m_bigPacketSize = Resources::protocol.ver < PROTOCOL_VER_4_1 ? 43 : 45;
		emit connected(port->portName());
	}
	else
	{
		if (port != nullptr) delete port;
		m_reconnectTimer = startTimer(Resources::portOptions.reconnectDelay * 1000);
	}
}

void LBPDeviceWorker::requestPowerState()
{
	if (m_port != nullptr)
	{
		RequestType request;
		request.arr[0] = 'a';
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(m_bigPacketSize, answer))
		{
			const char *d = answer.constData();
			PowerStateHandle state;
			int idx = 0;
			state.numUref.arr[0] = d[idx++];
			state.numUref.arr[1] = d[idx++];
			state.numUref.value++;
			state.Uref.arr[0] = d[idx++];
			state.Uref.arr[1] = d[idx++];
			state.numIref.arr[0] = d[idx++];
			state.numIref.arr[1] = d[idx++];
			state.numIref.value++;
			state.Iref.arr[0] = d[idx++];
			state.Iref.arr[1] = d[idx++];
			state.regimeU.arr[0] = d[idx++];
			state.regimeU.arr[1] = d[idx++];
			state.corrUref.arr[0] = d[idx++];
			state.corrUref.arr[1] = d[idx++];
			state.calibrAdcU.arr[0] = d[idx++];
			state.calibrAdcU.arr[1] = d[idx++];
			state.calibrDacU.arr[0] = d[idx++];
			state.calibrDacU.arr[1] = d[idx++];
			state.regimeI.arr[0] = d[idx++];
			state.regimeI.arr[1] = d[idx++];
			state.corrIref.arr[0] = d[idx++];
			state.corrIref.arr[1] = d[idx++];
			state.calibrAdcI.arr[0] = d[idx++];
			state.calibrAdcI.arr[1] = d[idx++];
			state.calibrDacI.arr[0] = d[idx++];
			state.calibrDacI.arr[1] = d[idx++];
			state.coolerOn.arr[0] = d[idx++];
			state.coolerOn.arr[1] = d[idx++];
			state.powerOff.arr[0] = d[idx++];
			state.powerOff.arr[1] = d[idx++];
			state.trigger.arr[0] = d[idx++];
			state.trigger.arr[1] = d[idx++];
			if (Resources::protocol.ver >= PROTOCOL_VER_4_1)
			{
				state.corrDACs.arr[0] = d[idx++];
				state.corrDACs.arr[1] = d[idx++];
			}
			state.voltage.arr[0] = d[idx++];
			state.voltage.arr[1] = d[idx++];
			state.current.arr[0] = d[idx++];
			state.current.arr[1] = d[idx++];
			state.temperatureT = quint8(d[idx++]);
			state.temperatureD = quint8(d[idx++]);
			state.time.arr[0] = d[idx++];
			state.time.arr[1] = d[idx++];
			state.time.arr[2] = d[idx++];
			state.capacity.arr[0] = d[idx++];
			state.capacity.arr[1] = d[idx++];
			state.capacity.arr[2] = d[idx++];
			char f = d[idx++];
			state.running = (f & PowerRunning) != 0;
			state.overheat = (f & PowerOverheat) != 0;
			state.overload = (f & PowerOverload) != 0;
			state.noAdc = (f & PowerNoADC) != 0;
			state.noDacI = (f & PowerNoDACI) != 0;
			state.noDacU = (f & PowerNoDACU) != 0;
			if (m_stateInited)
			{
				OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), 0);
				if (m_state.running != state.running) emit runningState(state.running);
				if (m_state.Uref.value != state.Uref.value)
				{
                    double y = Resources::int16ToValue(state.Uref.value);
                    v.setY(y);
					Resources::lastValues[OscilloscopeVoltageTask] = v;
                    emit taskU(y);
				}
				if (m_state.Iref.value != state.Iref.value)
				{
                    double y = Resources::int16ToValue(state.Iref.value);
                    v.setY(y);
					Resources::lastValues[OscilloscopeCurrentTask] = v;
                    emit taskI(y);
				}
				if (m_state.trigger.value != state.trigger.value) emit triggerMode(state.trigger.value != 0);
				if (m_state.voltage.value != state.voltage.value)
				{
                    double y = Resources::int16ToValue(state.voltage.value);
                    v.setY(y);
					Resources::lastValues[OscilloscopeVoltage] = v;
                    emit voltage(y);
				}
				if (m_state.current.value != state.current.value)
				{
                    double y = Resources::int16ToValue(state.current.value);
                    v.setY(y);
					Resources::lastValues[OscilloscopeCurrent] = v;
                    emit current(y);
				}
				if (m_state.temperatureT != state.temperatureT)
				{
					v.setY(state.temperatureT);
					Resources::lastValues[OscilloscopeTemperatureTransistor] = v;
					emit temperatureT(state.temperatureT);
				}
				if (m_state.temperatureD != state.temperatureD)
				{
					v.setY(state.temperatureD);
					Resources::lastValues[OscilloscopeTemperatureDiode] = v;
					emit temperatureD(state.temperatureD);
				}
				if (m_state.regimeU.value != state.regimeU.value) emit regimeU(state.regimeU.value);
				if (m_state.regimeI.value != state.regimeI.value) emit regimeI(state.regimeI.value);
				bool old_alarm = m_state.overheat || m_state.overload;
				bool new_alarm = state.overheat || state.overload;
				if (old_alarm != new_alarm) emit alarm(new_alarm);
				if (m_state.overheat != state.overheat) emit overheat(state.overheat);
				if (m_state.overload != state.overload) emit overload(state.overload);
				if (m_state.noDacU != state.noDacU) emit noDacU(state.noDacU);
				if (m_state.noDacI != state.noDacI) emit noDacI(state.noDacI);
				if (m_state.noAdc != state.noAdc) emit noAdc(state.noAdc);
				if (m_state.corrUref.value != state.corrUref.value) emit taskUCorrection(Resources::intToValue(state.corrUref.value));
				if (m_state.calibrAdcU.value != state.calibrAdcU.value) emit calibrationAdcU(Resources::int16ToValue(state.calibrAdcU.value));
				if (m_state.calibrDacU.value != state.calibrDacU.value) emit calibrationDacU(state.calibrDacU.value);
				if (m_state.corrIref.value != state.corrIref.value) emit taskICorrection(Resources::intToValue(state.corrIref.value));
				if (m_state.calibrAdcI.value != state.calibrAdcI.value) emit calibrationAdcI(Resources::int16ToValue(state.calibrAdcI.value));
				if (m_state.calibrDacI.value != state.calibrDacI.value) emit calibrationDacI(state.calibrDacI.value);
				if (m_state.coolerOn.value != state.coolerOn.value) emit fanOnTemperature(state.coolerOn.value);
				if (m_state.powerOff.value != state.powerOff.value) emit powerOffTemperature(state.powerOff.value);
				if (m_state.corrDACs.value != state.corrDACs.value) emit autocorrectionDac(state.corrDACs.value != 0);
				if (m_state.time.value != state.time.value)
				{
					quint8 hour, min, sec;
					hour = quint8(state.time.value / 3600);
					min = (state.time.value % 3600) / 60;
					sec = state.time.value % 60;
					emit accumulatorTime(hour, min, sec);
				}
				if (m_state.capacity.value != state.capacity.value) accumulatorCapacity(m_state.capacity.value / 1000.0);
			}
			else
			{
				OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), 0);
                double y;

				emit runningState(state.running);

                y = Resources::int16ToValue(state.Uref.value);
                v.setY(y);
                Resources::lastValues[OscilloscopeVoltageTask] = v;
                emit taskU(y);

                y = Resources::int16ToValue(state.Iref.value);
                v.setY(y);
                Resources::lastValues[OscilloscopeCurrentTask] = v;
                emit taskI(y);

				emit triggerMode(state.trigger.value != 0);

                y = Resources::int16ToValue(state.voltage.value);
                v.setY(y);
                Resources::lastValues[OscilloscopeVoltage] = v;
                emit voltage(y);

                y = Resources::int16ToValue(state.current.value);
                v.setY(y);
                Resources::lastValues[OscilloscopeCurrent] = v;
                emit current(y);

                v.setY(state.temperatureT);
                Resources::lastValues[OscilloscopeTemperatureTransistor] = v;
				emit temperatureT(state.temperatureT);

                v.setY(state.temperatureD);
                Resources::lastValues[OscilloscopeTemperatureDiode] = v;
				emit temperatureD(state.temperatureD);

				emit regimeU(state.regimeU.value);
				emit regimeI(state.regimeI.value);
				emit alarm(state.overheat || state.overload);
				emit overheat(state.overheat);
				emit overload(state.overload);
				emit noDacU(state.noDacU);
				emit noDacI(state.noDacI);
				emit noAdc(state.noAdc);
				emit taskUCorrection(Resources::intToValue(state.corrUref.value));
				emit calibrationAdcU(Resources::int16ToValue(state.calibrAdcU.value));
				emit calibrationDacU(state.calibrDacU.value);
				emit taskICorrection(Resources::intToValue(state.corrIref.value));
				emit calibrationAdcI(Resources::int16ToValue(state.calibrAdcI.value));
				emit calibrationDacI(state.calibrDacI.value);
				emit fanOnTemperature(state.coolerOn.value);
				emit powerOffTemperature(state.powerOff.value);
				emit autocorrectionDac(state.corrDACs.value != 0);
				quint8 hour, min, sec;
				hour = quint8(state.time.value / 3600);
				min = (state.time.value % 3600) / 60;
				sec = state.time.value % 60;
				emit accumulatorTime(hour, min, sec);
				emit accumulatorCapacity(m_state.capacity.value / 1000.0);
				m_stateInited = true;
			}
			m_state = state;
		}
	}
}

void LBPDeviceWorker::requestSwitchRunning()
{
	if (m_port != nullptr)
	{
		RequestType request;
		request.arr[0] = 'p';
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			if (answer.at(0) == 'p') emit runningState(!m_state.running);
		}
	}
}

void LBPDeviceWorker::requestSetUTask(double value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = Resources::valueToInt16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 1; //Uref
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit taskU(Resources::int16ToValue(v.value));
			}
		}
	}
}

void LBPDeviceWorker::requestSetITask(double value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = Resources::valueToInt16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 3; //Iref
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit taskI(Resources::int16ToValue(v.value));
			}
		}
	}
}

void LBPDeviceWorker::requestSetTriggerMode(bool trigger)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = trigger ? 1 : 0;
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 14; //Trigger
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w') emit triggerMode(d[3] == 1);
		}
	}
}

void LBPDeviceWorker::requestApplyPreset(const PresetRecord &preset)
{
	if (m_port != nullptr)
	{
		bool ok;
		DeviceValue16 v;
		QByteArray answer;
		RequestType request;
		ok  = false;
		v.value = Resources::valueToInt16(preset.voltage);
		request.arr[0] = 'w';
		request.arr[1] = 1;
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		if (waitForAnswer(4, answer))
		{
			ok = true;
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit taskU(Resources::int16ToValue(v.value));
			}
		}
		if (ok)
		{
			ok = false;
			request.value = 0;
			answer.clear();
			v.value = Resources::valueToInt16(preset.current);
			request.arr[0] = 'w';
			request.arr[1] = 3;
			request.arr[2] = v.arr[0];
			request.arr[3] = v.arr[1];
			m_port->write(request.arr, 4);
			if (waitForAnswer(4, answer))
			{
				ok = true;
				const char *d = answer.constData();
				if (d[0] == 'w')
				{
					v.arr[0] = d[2];
					v.arr[1] = d[3];
					emit taskI(Resources::int16ToValue(v.value));
				}
			}
		}
		if (ok)
		{
			ok = false;
			request.value = 0;
			answer.clear();
			v.value = preset.trigger ? 1 : 0;
			request.arr[0] = 'w';
			request.arr[1] = 14;
			request.arr[2] = v.arr[0];
			request.arr[3] = v.arr[1];
			m_port->write(request.arr, 4);
			if (waitForAnswer(4, answer))
			{
				const char *d = answer.constData();
				if (d[0] == 'w') emit triggerMode(d[3] == 1);
			}
		}
	}
}

void LBPDeviceWorker::requestSetRegimeU(quint16 value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = value;
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 4; //Regim_U
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit regimeU(v.value);
			}
		}
	}
}

void LBPDeviceWorker::requestSetRegimeI(quint16 value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = value;
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 8; //Regim_I
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit regimeI(v.value);
			}
		}
	}
}

void LBPDeviceWorker::requestSetTaskUCorrection(double value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = Resources::valueToInt16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 5; //corr_Uref
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit taskUCorrection(Resources::intToValue(v.value));
			}
		}
	}
}

void LBPDeviceWorker::requestSetCalibrationAdcU(double value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = Resources::valueToInt16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 6; //calibr_U
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit calibrationAdcU(Resources::int16ToValue(v.value));
			}
		}
	}
}

void LBPDeviceWorker::requestSetCalibrationDacU(int value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = quint16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 7; //calibr_Upwm
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit calibrationDacU(v.value);
			}
		}
	}
}

void LBPDeviceWorker::requestSetTaskICorrection(double value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = Resources::valueToInt16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 9; //corr_Iref
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit taskICorrection(Resources::intToValue(v.value));
			}
		}
	}
}

void LBPDeviceWorker::requestSetCalibrationAdcI(double value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = Resources::valueToInt16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 10; //calibr_I
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit calibrationAdcI(Resources::int16ToValue(v.value));
			}
		}
	}
}

void LBPDeviceWorker::requestSetCalibrationDacI(int value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = quint16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 11; //calibr_Ipwm
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit calibrationDacI(v.value);
			}
		}
	}
}

void LBPDeviceWorker::requestSetFanOnTemperature(int value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = quint16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 12; //coolerOn
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit fanOnTemperature(v.value);
			}
		}
	}
}

void LBPDeviceWorker::requestSetPowerOffTemperature(int value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = quint16(value);
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 13; //powerOff
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit powerOffTemperature(v.value);
			}
		}
	}
}

void LBPDeviceWorker::requestSetAutocorrectionDac(bool value)
{
	if (m_port != nullptr)
	{
		DeviceValue16 v;
		v.value = value ? 1 : 0;
		RequestType request;
		request.arr[0] = 'w';
		request.arr[1] = 15; //CorrDACs
		request.arr[2] = v.arr[0];
		request.arr[3] = v.arr[1];
		m_port->write(request.arr, 4);
		QByteArray answer;
		if (waitForAnswer(4, answer))
		{
			const char *d = answer.constData();
			if (d[0] == 'w')
			{
				v.arr[0] = d[2];
				v.arr[1] = d[3];
				emit autocorrectionDac(v.value != 0);
			}
		}
	}
}

void LBPDeviceWorker::requestMoveToParameter(int paramNo)
{
	if (m_port != nullptr)
	{
		RequestType request;
		request.arr[0] = 'N';
		request.arr[1] = char(quint8(paramNo));
		request.arr[2] = 0;
		request.arr[3] = 0;
		m_port->write(request.arr, 4);
		QByteArray answer;
		waitForAnswer(4, answer);
	}
}

void LBPDeviceWorker::requestResetAccumulator()
{
	if (m_port != nullptr)
	{
		RequestType request;
		request.arr[0] = 's';
		request.arr[1] = 0;
		request.arr[2] = 0;
		request.arr[3] = 0;
		m_port->write(request.arr, 4);
		QByteArray answer;
		waitForAnswer(4, answer);
	}
}

void LBPDeviceWorker::onPortSettionsChanged()
{
	if (m_port != nullptr) connectToPort();
}

void LBPDeviceWorker::onRequestIntervalChanged()
{
	if (m_requestTimer != 0)
	{
		killTimer(m_requestTimer);
		m_requestTimer = startTimer(Resources::portOptions.reqestInterval);
	}
}

/*
void LBPDeviceWorker::startOscilloscope(quint8 rays)
{
	m_rays.clear();
	m_rayFlags = rays;
	if (m_rayFlags & OscilloscopeVoltage) m_rays[OscilloscopeVoltage] = OscilloscopeRayData();
	if (m_rayFlags & OscilloscopeVoltageTask) m_rays[OscilloscopeVoltageTask] = OscilloscopeRayData();
	if (m_rayFlags & OscilloscopeCurrent) m_rays[OscilloscopeCurrent] = OscilloscopeRayData();
	if (m_rayFlags & OscilloscopeCurrentTask) m_rays[OscilloscopeCurrentTask] = OscilloscopeRayData();
	if (m_rayFlags & OscilloscopeTemperatureTransistor) m_rays[OscilloscopeTemperatureTransistor] = OscilloscopeRayData();
	if (m_rayFlags & OscilloscopeTemperatureDiode) m_rays[OscilloscopeTemperatureDiode] = OscilloscopeRayData();
}

void LBPDeviceWorker::stopOscilloscope()
{
	m_rayFlags = 0;
	m_rays.clear();
}
*/
