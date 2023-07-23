#include <QFontDatabase>
#include <cmath>

#include "Resources.h"

QString Resources::configFile;
QString Resources::cssFile;
QString Resources::presetsFile = QString("presets.conf");
QStringList Resources::fonts;
QRect Resources::windowRect;
int Resources::applyInterval = 1000;
Qt::WindowState Resources::windowState = Qt::WindowNoState;
SerialPortOptions Resources::portOptions;
int Resources::num_digits = 2;
int Resources::num_precision = 2;
PresetsList Resources::presets;
bool Resources::focusSync = false;
bool Resources::accumulator = false;
bool Resources::virtualKeyboard = false;
ProtocolVersion Resources::protocol;
OscilloscopeConfig Resources::oscilloscope;
OscilloscopeValueList Resources::lastValues;
OscilloscopeRayNames Resources::rayNames;

void Resources::readWindowState()
{
	QFile confFile(WINDOW_STATE_FILE);
	if (confFile.open(QIODevice::ReadOnly))
	{
		QJsonParseError parseError;
		QJsonDocument doc(QJsonDocument::fromJson(confFile.readAll(), &parseError));
		if (parseError.error == QJsonParseError::NoError)
		{
			QJsonObject conf = doc.object();
			windowRect = QRect(conf["left"].toInt(), conf["top"].toInt(), conf["width"].toInt(), conf["height"].toInt());
			QString s = conf["state"].toString();
			if (s == "minimized") windowState = Qt::WindowMinimized;
			else if (s == "maximized") windowState = Qt::WindowMaximized;
			else if (s == "full_screen") windowState = Qt::WindowFullScreen;
			else windowState = Qt::WindowNoState;
			accumulator = conf["accumulator"].toBool();
			QJsonObject osl = conf["oscilloscope"].toObject();
			oscilloscope.windowRect = QRect(osl["left"].toInt(), osl["top"].toInt(), osl["width"].toInt(), osl["height"].toInt());
		}
	}
}

void Resources::writeWindowState()
{
	QFile confFile(WINDOW_STATE_FILE);
	if (confFile.open(QIODevice::WriteOnly))
	{
		QJsonObject conf;
		conf["left"] = windowRect.left();
		conf["top"] = windowRect.top();
		conf["width"] = windowRect.width();
		conf["height"] = windowRect.height();
		QString s("normal");
		if (windowState == Qt::WindowMinimized) s = "minimized";
		else if (windowState == Qt::WindowMaximized) s ="maximized";
		else if (windowState == Qt::WindowFullScreen) s = "full_screen";
		conf["state"] = s;
		conf["accumulator"] = accumulator;
		QJsonObject osl;
		osl["left"] = oscilloscope.windowRect.left();
		osl["top"] = oscilloscope.windowRect.top();
		osl["width"] = oscilloscope.windowRect.width();
		osl["height"] = oscilloscope.windowRect.height();
		conf["oscilloscope"] = osl;
		QJsonDocument doc(conf);
		confFile.write(doc.toJson());
	}
}

void Resources::readConfig()
{
	rayNames[OscilloscopeVoltage] = QObject::tr("Voltage");
	rayNames[OscilloscopeVoltageTask] = QObject::tr("Voltage task");
	rayNames[OscilloscopeCurrent] = QObject::tr("Current");
	rayNames[OscilloscopeCurrentTask] = QObject::tr("Current task");
	rayNames[OscilloscopeTemperatureTransistor] = QObject::tr("T (transistor)");
	rayNames[OscilloscopeTemperatureDiode] = QObject::tr("T (diode)");

	QFile confFile(configFile);
	if (confFile.open(QIODevice::ReadOnly))
	{
		QJsonParseError parseError;
		QJsonDocument doc(QJsonDocument::fromJson(confFile.readAll(), &parseError));
		if (parseError.error == QJsonParseError::NoError)
		{
			QJsonObject config = doc.object();
			QJsonValue v;
			if (config.contains("general"))
			{
				QJsonObject general = config["general"].toObject();
				v = general["cssfile"];
				if (!v.isNull()) cssFile = v.toString();
				v = general["presetsfile"];
				if (!v.isNull()) presetsFile = v.toString();
				v = general["fonts"];
				if (!v.isNull())
				{
					QString fn;
					fonts.clear();
					QJsonArray farr = v.toArray();
					for (int i = 0; i < farr.size(); ++i)
					{
						fn = farr[i].toString();
						fonts.append(fn);
						QFontDatabase::addApplicationFont(fn);
					}
				}
				v = general["apply_interval"];
				if (!v.isNull())
				{
					int n = v.toInt();
					if (n > 0) applyInterval = n;
				}
				v = general["num_digits"];
				if (!v.isNull())
				{
					int n = v.toInt();
					if (n > 0) num_digits = n;
				}
				v = general["num_precision"];
				if (!v.isNull())
				{
					int n = v.toInt();
					if (n > 0) num_precision = n;
				}
				focusSync = general["focus_sync"].toBool();
				virtualKeyboard = general["virtual_keyboard"].toBool();
			}
			if (config.contains("serial_port"))
			{
				QJsonObject serport = config["serial_port"].toObject();
				portOptions.portAutosearch = serport["autosearch"].toBool();
				portOptions.reconnectDelay = serport["reconnect_delay"].toInt(15);
				portOptions.port = serport["port"].toString();
				portOptions.baudrate = qint32(serport["baudrate"].toInt(QSerialPort::Baud9600));
				portOptions.databits = QSerialPort::DataBits(serport["databits"].toInt(QSerialPort::Data8));
				QString s = serport["parity"].toString();
				if (s == "NONE") portOptions.parity = QSerialPort::NoParity;
				else if (s == "EVEN") portOptions.parity = QSerialPort::EvenParity;
				else if (s == "ODD") portOptions.parity = QSerialPort::OddParity;
				else if (s == "SPACE") portOptions.parity = QSerialPort::SpaceParity;
				else if (s == "MARK") portOptions.parity = QSerialPort::MarkParity;
				double dv = serport["stopbits"].toDouble(QSerialPort::OneStop);
				if (qAbs(dv - 1.0) < 0.0001) portOptions.stopbits = QSerialPort::OneStop;
				else if (qAbs(dv - 1.5) < 0.0001) portOptions.stopbits = QSerialPort::OneAndHalfStop;
				else if (qAbs(dv - 2) < 0.0001) portOptions.stopbits = QSerialPort::TwoStop;
				s = serport["flowcontrol"].toString();
				if (s == "OFF") portOptions.flowcontrol = QSerialPort::NoFlowControl;
				if (s == "HARDWARE") portOptions.flowcontrol = QSerialPort::HardwareControl;
				if (s == "SOFTWARE") portOptions.flowcontrol = QSerialPort::SoftwareControl;
				portOptions.ioTimeout = serport["timeout"].toInt(1000);
			}
			if (config.contains("device"))
			{
				QJsonObject dev = config["device"].toObject();
				portOptions.reqestInterval = dev["reqest_interval"].toInt(500);
				protocol.ver = PROTOCOL_VER_4_1;
				QString s = dev["protocol"].toString();
				if (!s.isEmpty())
				{
					QStringList vl = s.split('.');
					int n = vl.size();
					bool ok;
					if (n == 1)
					{
						quint8 mjr = quint8(vl.at(0).toUInt(&ok));
						if (ok)
						{
							protocol.arr[0] = 0;
							protocol.arr[1] = mjr;
						}
					}
					else if (n == 2)
					{
						quint8 mjr = quint8(vl.at(0).toUInt(&ok));
						if (ok)
						{
							quint8 mnr = quint8(vl.at(1).toUInt(&ok));
							if (ok)
							{
								protocol.arr[0] = mnr;
								protocol.arr[1] = mjr;
							}
						}
					}
				}
			}
			if (config.contains("oscilloscope"))
			{
				QJsonObject osl = config["oscilloscope"].toObject();
				oscilloscope.rays = 0;
				oscilloscope.depth = osl["depth"].toInt(180000);
				oscilloscope.x_grid = osl["grid"].toInt(10);
				oscilloscope.update_interval = osl["update_interval"].toInt(300);
				oscilloscope.background = QColor(osl["background"].toString());
				QJsonObject jclr = osl["colours"].toObject();
				oscilloscope.colors[OscilloscopeVoltage]  = QColor(jclr["voltage"].toString());
				oscilloscope.colors[OscilloscopeVoltageTask]  = QColor(jclr["voltage_task"].toString());
				oscilloscope.colors[OscilloscopeCurrent]  = QColor(jclr["current"].toString());
				oscilloscope.colors[OscilloscopeCurrentTask]  = QColor(jclr["current_task"].toString());
				oscilloscope.colors[OscilloscopeTemperatureTransistor]  = QColor(jclr["temperature_diode"].toString());
				oscilloscope.colors[OscilloscopeTemperatureDiode]  = QColor(jclr["temperature_transistor"].toString());
				QJsonArray jarrpnl = osl["panels"].toArray();
				for (int i = 0; i < jarrpnl.size(); i++)
				{
					QJsonObject jpnl = jarrpnl[i].toObject();
					OscilloscopeHandle panel;
					panel.y_min = jpnl["min"].toDouble(0);
					panel.y_max = jpnl["max"].toDouble(100);
					panel.y_grid = jpnl["grid"].toInt(10);
					panel.rays = 0;
					QJsonArray jrays = jpnl["rays"].toArray();
					QString s;
					for (int j = 0; j < jrays.size(); j++)
					{
						s = jrays[j].toString().trimmed().toLower();
						if (s == "voltage") panel.rays |= OscilloscopeVoltage;
						if (s == "voltage_task") panel.rays |= OscilloscopeVoltageTask;
						if (s == "current") panel.rays |= OscilloscopeCurrent;
						if (s == "current_task") panel.rays |= OscilloscopeCurrentTask;
						if (s == "temperature_diode") panel.rays |= OscilloscopeTemperatureDiode;
						if (s == "temperature_transistor") panel.rays |= OscilloscopeTemperatureTransistor;
					}
					oscilloscope.rays |= panel.rays;
					oscilloscope.handles.append(panel);
				}
			}
		}
	}
}

void Resources::writeConfig()
{
	QFile confFile(configFile);
	if (confFile.open(QIODevice::WriteOnly))
	{
		QJsonObject conf;
		QJsonObject general;
		general["cssfile"] = cssFile;
		general["presetsfile"] = presetsFile;
		QJsonArray fnt;
		for (int i = 0; i < fonts.size(); ++i) fnt.append(fonts.at(i));
		general["fonts"] = fnt;
		general["apply_interval"] = applyInterval;
		general["num_digits"] = num_digits;
		general["num_precision"] = num_precision;
		general["focus_sync"] = focusSync;
		general["virtual_keyboard"] = virtualKeyboard;
		conf["general"] = general;
		QJsonObject serport;
		serport["autosearch"] = portOptions.portAutosearch;
		serport["reconnect_delay"] = portOptions.reconnectDelay;
		serport["port"] = portOptions.port;
		serport["baudrate"] = portOptions.baudrate;
		serport["databits"] = portOptions.databits;
		QString s = "NONE";
		if (portOptions.parity == QSerialPort::NoParity) s = "NONE";
		else if (portOptions.parity == QSerialPort::EvenParity) s = "EVEN";
		else if (portOptions.parity == QSerialPort::OddParity) s = "ODD";
		else if (portOptions.parity == QSerialPort::SpaceParity) s = "SPACE";
		else if (portOptions.parity == QSerialPort::MarkParity) s = "MARK";
		serport["parity"] = s;
		double dv = 1;
		if (portOptions.stopbits == QSerialPort::OneStop) dv = 1;
		else if (portOptions.stopbits == QSerialPort::OneAndHalfStop) dv = 1.5;
		else if (portOptions.stopbits == QSerialPort::TwoStop) dv = 2;
		serport["stopbits"] = dv;
		s = "OFF";
		if (portOptions.flowcontrol == QSerialPort::NoFlowControl) s = "OFF";
		if (portOptions.flowcontrol == QSerialPort::HardwareControl) s = "HARDWARE";
		if (portOptions.flowcontrol == QSerialPort::SoftwareControl) s = "SOFTWARE";
		serport["flowcontrol"] = s;
		serport["timeout"] = portOptions.ioTimeout;
		conf["serial_port"] = serport;
		QJsonObject dev;
		dev["reqest_interval"] = portOptions.reqestInterval;
		dev["protocol"] = QString("%1.%2").arg(protocol.arr[1]).arg(protocol.arr[0]);
		conf["device"] = dev;
		QJsonObject osl;
		osl["depth"] = oscilloscope.depth;
		osl["grid"] = oscilloscope.x_grid;
		osl["background"] = oscilloscope.background.name();
		osl["update_interval"] = oscilloscope.update_interval;
		QJsonObject osl_colours;
		osl_colours["voltage"] = oscilloscope.colors[OscilloscopeVoltage].name();
		osl_colours["voltage_task"] = oscilloscope.colors[OscilloscopeVoltageTask].name();
		osl_colours["current"] = oscilloscope.colors[OscilloscopeCurrent].name();
		osl_colours["current_task"] = oscilloscope.colors[OscilloscopeCurrentTask].name();
		osl_colours["temperature_diode"] = oscilloscope.colors[OscilloscopeTemperatureDiode].name();
		osl_colours["temperature_transistor"] = oscilloscope.colors[OscilloscopeTemperatureTransistor].name();
		osl["colours"] = osl_colours;
		QJsonArray osl_panels;
		int n = oscilloscope.handles.size();
		for (int i = 0; i < n; i++)
		{
			const OscilloscopeHandle &panel = oscilloscope.handles.at(i);
			QJsonObject osl_panel;
			osl_panel["min"] = panel.y_min;
			osl_panel["max"] = panel.y_max;
			osl_panel["grid"] = panel.y_grid;
			QJsonArray osl_rays;
			if (panel.rays & OscilloscopeVoltage) osl_rays.append("voltage");
			if (panel.rays & OscilloscopeVoltageTask) osl_rays.append("voltage_task");
			if (panel.rays & OscilloscopeCurrent) osl_rays.append("current");
			if (panel.rays & OscilloscopeCurrentTask) osl_rays.append("current_task");
			if (panel.rays & OscilloscopeTemperatureDiode) osl_rays.append("temperature_diode");
			if (panel.rays & OscilloscopeTemperatureTransistor) osl_rays.append("temperature_transistor");
			osl_panel["rays"] = osl_rays;
			osl_panels.append(osl_panel);
		}
		osl["panels"] = osl_panels;
		conf["oscilloscope"] = osl;
		QJsonDocument doc(conf);
		confFile.write(doc.toJson());
	}
}

QString Resources::normalizeValue(double value)
{
	QString ret = QString::number(value, 'f', num_precision);
	int n = ret.indexOf('.') - 1;
	if ((n >= 0) && (n < num_digits))
	{
		n = num_digits - n - 1;
		if (n > 0) ret = QString(n, '0') + ret;
	}
	return ret;
}

quint16 Resources::valueToInt16(double value)
{
	return quint16(value * pow(10,  num_precision));
}

double Resources::int16ToValue(quint16 d)
{
	return d / pow(10, num_precision);
}

double Resources::intToValue(quint16 d)
{
	double p = pow(10, num_precision);
	if (d < p)
	{
		return d / p;
	}
	else
	{
		return  -(double(65536 - d) / p);
	}
}

void Resources::readPresets()
{
	QFile prsFile(presetsFile);
	if (prsFile.open(QIODevice::ReadOnly))
	{
		QJsonParseError parseError;
		QJsonDocument doc(QJsonDocument::fromJson(prsFile.readAll(), &parseError));
		if (parseError.error == QJsonParseError::NoError)
		{
			QJsonObject jpresets = doc.object();
			if (jpresets.contains("presets"))
			{
				presets.clear();
				QJsonArray arr = jpresets["presets"].toArray();
				for (int i = 0; i < arr.size(); ++i)
				{
					QJsonObject jpreset = arr[i].toObject();
					PresetRecord preset;
					preset.voltage = jpreset["V"].toDouble();
					preset.current = jpreset["A"].toDouble();
					preset.trigger = jpreset["T"].toBool();
					presets.append(preset);
				}
			}
		}
	}
}

void Resources::writePresets()
{
	QFile prsFile(presetsFile);
	if (prsFile.open(QIODevice::WriteOnly))
	{
		QJsonObject jroot;
		QJsonArray jpresets;
		for (int i = 0; i < presets.size(); ++i)
		{
			const PresetRecord &preset = presets.at(i);
			QJsonObject jpreset;
			jpreset["V"] = preset.voltage;
			jpreset["A"] = preset.current;
			jpreset["T"] = preset.trigger;
			jpresets.append(jpreset);
		}
		jroot["presets"] = jpresets;
		QJsonDocument doc(jroot);
		prsFile.write(doc.toJson());
	}
}
