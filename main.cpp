#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>

#include "MainWindow.h"
#include "LBPDevice.h"
#include "Resources.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTranslator translator;
	translator.load(QString("./translations/%1_%2.ts").arg(QApplication::applicationName()).arg(QLocale::system().name()));
	a.installTranslator(&translator);

	QString conf_file;
	conf_file = a.applicationFilePath();
#ifdef Q_OS_WIN
	conf_file.replace(".exe", ".conf");
#else
	conf_file += ".conf";
#endif
	if (!QFileInfo::exists(conf_file))
	{
		QMessageBox::critical(nullptr, QObject::tr("Power supply"), QObject::tr("No configuration file found: '%1'!").arg(conf_file));
		return 0;
	}
	Resources::configFile = conf_file;
	Resources::readConfig();
	QFile cssfile(Resources::cssFile);
	if (cssfile.open(QIODevice::ReadOnly))
	{
		QString css(cssfile.readAll());
		cssfile.close();
		a.setStyleSheet(css);
	}
	qRegisterMetaType<PresetRecord>();
	Resources::readPresets();
	LBPDevice device;
	MainWindow mainWindow(&device);
	mainWindow.setObjectName("mainWindow");

	mainWindow.showWindow();
	device.start();
	return a.exec();
}
