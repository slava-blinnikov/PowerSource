#ifndef RESOURCES_H
#define RESOURCES_H

#include "TypesAndConstants.h"

class Resources
{
public:
	static QString configFile;
	static QString cssFile;
	static QString presetsFile;
	static QStringList fonts;
	static QRect windowRect;
	static Qt::WindowState windowState;
	static int applyInterval;
	static int num_digits;
	static int num_precision;
	static PresetsList presets;
	static SerialPortOptions portOptions;
	static bool focusSync;
	static bool accumulator;
	static bool virtualKeyboard;
	static ProtocolVersion protocol;
	static OscilloscopeRayNames rayNames;
	static OscilloscopeValueList lastValues;
	static OscilloscopeConfig oscilloscope;
	static void readWindowState();
	static void writeWindowState();
	static void readConfig();
	static void writeConfig();
	static void readPresets();
	static void writePresets();
	static QString normalizeValue(double value);
	static quint16 valueToInt16(double value);
	static double int16ToValue(quint16 d);
	static double intToValue(quint16 d);
};

#endif // RESOURCES_H
