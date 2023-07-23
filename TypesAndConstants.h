#ifndef TYPESANDCONSTANTS_H
#define TYPESANDCONSTANTS_H

#define WINDOW_STATE_FILE "windowstate.conf"
#define EDITED_PARAMETER_STYLE "QWidget{color: red}"
#define FOCUSED_CELL_STYLE "QTableView::item:focus{selection-background-color: green;}"
#define PROTOCOL_VER_4_0 0x0400
#define PROTOCOL_VER_4_1 0x0401

#include <QtDebug>

#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QRect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFont>
#include <QColor>
#include <QSerialPort>
#include <QHash>

union RequestType
{
	RequestType()
	{
		value = 0;
	}
	char arr[4];
	quint32 value;
};

union DeviceValue16
{
	DeviceValue16()
	{
		value = 0;
	}
	char arr[2];
	quint16 value;
};

typedef RequestType DeviceValue32;

struct PowerStateHandle
{
	PowerStateHandle()
	{
		temperatureT = 0;
		temperatureD = 0;
		running = false;
		overheat = false;
		overload = false;
		noAdc = false;
		noDacI = false;
		noDacU = false;
	}
	DeviceValue16 numUref;
	DeviceValue16 Uref;
	DeviceValue16 numIref;
	DeviceValue16 Iref;
	DeviceValue16 regimeU;
	DeviceValue16 corrUref;
	DeviceValue16 calibrAdcU;
	DeviceValue16 calibrDacU;
	DeviceValue16 regimeI;
	DeviceValue16 corrIref;
	DeviceValue16 calibrAdcI;
	DeviceValue16 calibrDacI;
	DeviceValue16 coolerOn;
	DeviceValue16 powerOff;
	DeviceValue16 trigger;
	DeviceValue16 corrDACs;
	DeviceValue16 voltage;
	DeviceValue16 current;
	quint8 temperatureT;
	quint8 temperatureD;
	DeviceValue32 time;
	DeviceValue32 capacity;
	bool running;
	bool overheat;
	bool overload;
	bool noAdc;
	bool noDacI;
	bool noDacU;
};

enum PowerStateFlag
{
	PowerRunning = 0x01,
	PowerOverheat = 0x04,
	PowerOverload = 0x08,
	PowerNoADC = 0x20,
	PowerNoDACI = 0x40,
	PowerNoDACU = 0x80
};

struct PresetRecord
{
	PresetRecord()
	{
		initFlags = 0x07;
		voltage = 0.0;
		current = 0.0;
		trigger = false;
	}
	quint8 initFlags;
	double voltage;
	double current;
	bool trigger;
};
Q_DECLARE_METATYPE(PresetRecord)
typedef QVector<PresetRecord> PresetsList;

struct SerialPortOptions
{
	bool portAutosearch;
	QString port;
	int ioTimeout;
	int reconnectDelay;
	qint32 baudrate;
	QSerialPort::DataBits databits;
	QSerialPort::Parity parity;
	QSerialPort::StopBits stopbits;
	QSerialPort::FlowControl flowcontrol;
	int reqestInterval;
};

union ProtocolVersion
{
	quint8 arr[2];
	quint16 ver;
};

enum OscilloscopeRay
{
	OscilloscopeVoltage = 0x01,
	OscilloscopeVoltageTask = 0x02,
	OscilloscopeCurrent = 0x04,
	OscilloscopeCurrentTask = 0x08,
	OscilloscopeTemperatureTransistor = 0x10,
	OscilloscopeTemperatureDiode = 0x20
};
typedef QPointF OscilloscopeValue;
typedef QPolygonF OscilloscopeRayData;
typedef QHash<OscilloscopeRay, OscilloscopeRayData> OscilloscopeRayList;
typedef QHash<OscilloscopeRay, OscilloscopeValue> OscilloscopeValueList;
typedef QHash<OscilloscopeRay, QColor> OscilloscopeRayColors;
typedef QHash<OscilloscopeRay, QString> OscilloscopeRayNames;

struct OscilloscopeHandle
{
	quint8 rays;
	double y_min;
	double y_max;
	int y_grid;
};

typedef QVector<OscilloscopeHandle> OscilloscopeHandleList;
struct OscilloscopeConfig
{
	QRect windowRect;
	qint64 depth;
	int x_grid;
	quint8 rays;
	OscilloscopeRayColors colors;
	QColor background;
	OscilloscopeHandleList handles;
	int update_interval;
};

#endif // TYPESANDCONSTANTS_H
