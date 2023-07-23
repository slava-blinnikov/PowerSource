#include <QGridLayout>
#include <QStatusBar>
#include <QDateTime>

#include "MainWindow.h"
#include "PresetsDialog.h"
#include "OptionsDialog.h"
#include "OnscreenKeyboard.h"

MainWindow::MainWindow(LBPDevice *device, QWidget *parent) : QMainWindow(parent)
{
	m_device = device;
	m_plotDialog = nullptr;
	QStatusBar *stb = statusBar();
	m_pxmConnected = QPixmap(":icons/connected.png");
	m_pxmDisconnected = QPixmap(":icons/disconnected.png");

	m_lblConnected = new QLabel(stb);
	m_lblConnected->setToolTip(tr("<html><body>Device disconnected.</body></html>"));
	m_lblConnected->setPixmap(m_pxmDisconnected);
	m_lblConnected->setMinimumSize(16, 16);
	stb->addWidget(m_lblConnected);

	m_lblNoDacU = new QLabel(stb);
	m_lblNoDacU->setText(tr("NO DAC U!"));
	stb->addWidget(m_lblNoDacU);
	m_lblNoDacU->setVisible(false);

	m_lblNoDacI = new QLabel(stb);
	m_lblNoDacI->setText(tr("NO DAC I!"));
	stb->addWidget(m_lblNoDacI);
	m_lblNoDacI->setVisible(false);

	m_lblNoAdc = new QLabel(stb);
	m_lblNoAdc->setText(tr("NO ADC!"));
	stb->addWidget(m_lblNoAdc);
	m_lblNoAdc->setVisible(false);

	QVBoxLayout *vbl;
	QHBoxLayout *hbl;
	QwtLinearColorMap *colorMap;

	setWindowTitle(tr("Power supply [protocol v-%1.%2]").arg(Resources::protocol.arr[1]).arg(Resources::protocol.arr[0]));
	QWidget *centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");
	QVBoxLayout *vblMain = new QVBoxLayout();
	vblMain->setContentsMargins(0, 0, 0, 0);
	centralWidget->setLayout(vblMain);
	setCentralWidget(centralWidget);

	hbl = new QHBoxLayout(); //Панель инструментов
	hbl->setContentsMargins(5, 3, 5, 3);
	//Кнопка панели калибровочных параметров
	m_btnCalibration = new QPushButton(centralWidget);
	m_btnCalibration->setToolTip(tr("<html><body>Show calibration panel<br/>Shortcut: <b>F12</b></body></html>"));
	m_btnCalibration->setCheckable(true);
	m_btnCalibration->setObjectName("buttonCalibration");
	hbl->addWidget(m_btnCalibration, 10);
	//Кнопка настроек программы
	m_btnOptions = new QPushButton(centralWidget);
	m_btnOptions->setToolTip(tr("<html><body>Show options dialog<br/>Shortcut: <b>F4</b></body></html>"));
	m_btnOptions->setObjectName("buttonOptions");
	hbl->addWidget(m_btnOptions, 10);
	//Кнопка пресетов
	m_btnPresets = new QToolButton(centralWidget);
	m_btnPresets->setObjectName("buttonPresets");
	m_btnPresets->setCheckable(false);
	m_btnPresets->setPopupMode(QToolButton::MenuButtonPopup);
	m_mnuPresets = new QMenu(QString(), m_btnPresets);
	m_mnuPresets->setObjectName("menuPresets");
	createPresetsMenu();
	m_btnPresets->setMenu(m_mnuPresets);
	m_btnPresets->setToolTip(tr("<html><body>Presets<br/>Shortcut: <b>F7</b></body></html>"));
	hbl->addWidget(m_btnPresets, 10);
	//Кнопка показать/спрятать аккумуляторную панель
	hbl->addSpacing(15);
	m_btnAccShow = new QPushButton(centralWidget);
	m_btnAccShow->setObjectName("buttonShowAccumulator");
	m_btnAccShow->setToolTip(tr("<html><body>Show accumulator panel.<br/>Shortcut: <b>F10</b></body></html>"));
	m_btnAccShow->setCheckable(true);
	hbl->addWidget(m_btnAccShow, 10);
	//Кнопка показать/спрятать окно осциллографа
	hbl->addSpacing(15);
	m_btnPlot = new QPushButton(centralWidget);
	m_btnPlot->setObjectName("buttonPlots");
	m_btnPlot->setToolTip(tr("<html><body>Show oscilloscope window.<br/>Shortcut: <b>F3</b></body></html>"));
	m_btnPlot->setCheckable(true);
	hbl->addWidget(m_btnPlot, 10);
	//Кнопка Старт/Стоп
	hbl->addSpacing(15);
	m_btnStart = new QPushButton(centralWidget);
	m_btnStart->setObjectName("buttonStart");
	m_btnStart->setToolTip(tr("<html><body>Start or stop device.<br/>Shortcut: <b>F9</b></body></html>"));
	m_btnStart->setCheckable(true);
	hbl->addWidget(m_btnStart, 10);
	//Текст тревоги
	m_lblAlarm = new QLabel(tr("No connection!"), centralWidget);
	m_lblAlarm->setObjectName("labelAlarmCaption");
	hbl->addWidget(m_lblAlarm, 100);
	hbl->addStretch();
	vblMain->addLayout(hbl);

	QHBoxLayout *hblMain = new QHBoxLayout();
	hblMain->setContentsMargins(0, 0, 0, 0);
	m_wdgCalibration = new CalibrationWidget(centralWidget);
	hblMain->addWidget(m_wdgCalibration, 5);
	m_wdgCalibration->setVisible(false);
	QGridLayout *grl = new QGridLayout();
	grl->setSpacing(0);
	grl->setContentsMargins(0, 0, 0, 0);
	double u_max = 80;
	double i_max = 10;
	vbl = new QVBoxLayout();
	vbl->addStretch();
	m_trmTaskU = new QwtThermo(centralWidget);
	m_trmTaskU->setOrientation(Qt::Horizontal);
	m_trmTaskU->setScalePosition(QwtThermo::TrailingScale);
	colorMap = new QwtLinearColorMap();
	colorMap->setColorInterval(Qt::blue, Qt::red);
	m_trmTaskU->setColorMap(colorMap);
	m_trmTaskU->setScale(0, u_max);
	m_trmTaskU->setValue(0);
	vbl->addWidget(m_trmTaskU);
	m_whlTaskU = new QwtWheel(centralWidget);
	m_whlTaskU->setRange(0, u_max);
	m_whlTaskU->setSingleStep(0.01);
	m_whlTaskU->setValue(0);
	m_whlTaskU->setWheelWidth(20);
	m_whlTaskU->setMass(1.0);
	m_whlTaskU->setOrientation(Qt::Horizontal);
	vbl->addWidget(m_whlTaskU);
	hbl = new QHBoxLayout();
	m_lblApplyU = new QLabel(centralWidget);
	m_lblApplyU->setObjectName("labelTaskUApply");
	m_lblApplyU->setVisible(false);
	hbl->addWidget(m_lblApplyU);
	m_leTaskU = new DigitalLineEdit(centralWidget);
	m_leTaskU->setObjectName("fieldTaskU");
	m_leTaskU->setToolTip(tr("<html><body>Voltage task. Click to edit.<br/>Shortcut: <b>F5</b></body></html>"));
	m_leTaskU->setScale(0, u_max);
	m_leTaskU->setValue(0);
	hbl->addWidget(m_leTaskU);
	vbl->addLayout(hbl);
	grl->addLayout(vbl, 1, 0);
	hbl = new QHBoxLayout();
	hbl->setContentsMargins(0, 0, 0, 0);
	m_lblU = new QLabel(QString("00.00"), centralWidget);
	m_lblU->setObjectName("labelU");
	hbl->addWidget(m_lblU);
	m_lblTitleU = new QLabel(tr("V"), centralWidget);
	m_lblTitleU->setObjectName("labelTitleU");
	m_lblTitleU->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
	hbl->addWidget(m_lblTitleU);
	grl->addLayout(hbl, 1, 1);
	vbl = new QVBoxLayout();
	vbl->addStretch();
	m_trmTaskI = new QwtThermo(centralWidget);
	m_trmTaskI->setOrientation(Qt::Horizontal);
	m_trmTaskI->setScalePosition(QwtThermo::TrailingScale);
	colorMap = new QwtLinearColorMap();
	colorMap->setColorInterval(Qt::blue, Qt::red);
	m_trmTaskI->setColorMap(colorMap);
	m_trmTaskI->setScale(0, i_max);
	m_trmTaskI->setValue(0);
	vbl->addWidget(m_trmTaskI);
	m_whlTaskI = new QwtWheel(centralWidget);
	m_whlTaskI->setRange(0, i_max);
	m_whlTaskI->setSingleStep(0.01);
	m_whlTaskI->setValue(0);
	m_whlTaskI->setWheelWidth(20);
	m_whlTaskI->setMass(1.0);
	m_whlTaskI->setOrientation(Qt::Horizontal);
	vbl->addWidget(m_whlTaskI);
	hbl = new QHBoxLayout();
	m_lblApplyI = new QLabel(centralWidget);
	m_lblApplyI->setObjectName("labelTaskIApply");
	m_lblApplyI->setVisible(false);
	hbl->addWidget(m_lblApplyI);
	m_leTaskI = new DigitalLineEdit(centralWidget);
	m_leTaskI->setObjectName("fieldTaskI");
	m_leTaskI->setToolTip(tr("<html><body>Current task. Click to edit.<br/>Shortcut: <b>F6</b></body></html>"));
	m_leTaskI->setScale(0, i_max);
	m_leTaskI->setValue(0);
	hbl->addWidget(m_leTaskI);
	vbl->addLayout(hbl);
	grl->addLayout(vbl, 2, 0);
	hbl = new QHBoxLayout();
	hbl->setContentsMargins(0, 0, 0, 0);
	vbl = new QVBoxLayout();
	m_btnOnOffTrigger = new QPushButton(centralWidget);
	m_btnOnOffTrigger->setObjectName("buttonOnOffTrigger");
	m_btnOnOffTrigger->setToolTip(tr("<html><body>On/Off trigger mode.<br/>Shortcut: <b>F8</b></body></html>"));
	m_btnOnOffTrigger->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	m_btnOnOffTrigger->setCheckable(true);
	vbl->addWidget(m_btnOnOffTrigger);
	m_chbIStable = new QCheckBox(centralWidget);
	m_chbIStable->setObjectName("checkboxIStable");
	m_chbIStable->setEnabled(false);
	m_chbIStable->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	vbl->addWidget(m_chbIStable);
	hbl->addLayout(vbl, 1);
	m_lblI = new QLabel(QString("00.00"), centralWidget);
	m_lblI->setObjectName("labelI");
	hbl->addWidget(m_lblI, 10);
	m_lblTitleI = new QLabel(tr("A"), centralWidget);
	m_lblTitleI->setObjectName("labelTitleI");
	m_lblTitleI->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
	hbl->addWidget(m_lblTitleI, 10);
	grl->addLayout(hbl, 2, 1);
	hbl = new QHBoxLayout();
	m_lblTempTransistorIndicator = new QLabel();
	m_lblTempTransistorIndicator->setObjectName("labelTemperatureTransistorIndicator");
	hbl->addWidget(m_lblTempTransistorIndicator, 1);
	m_lblTempTransistorTitle = new QLabel();
	m_lblTempTransistorTitle->setObjectName("labelTemperatureTransistorTitle");
	hbl->addWidget(m_lblTempTransistorTitle, 1);
	m_lblTempTransistorValue = new QLabel("0");
	m_lblTempTransistorValue->setObjectName("labelTemperatureTransistorValue");
	m_lblTempTransistorValue->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	m_lblTempTransistorValue->setVisible(false);
	hbl->addWidget(m_lblTempTransistorValue, 10);
	m_lblTempTransistorUnit = new QLabel(tr("NO SENSOR"));
	m_lblTempTransistorUnit->setObjectName("labelTemperatureTransistorUnit");
	hbl->addWidget(m_lblTempTransistorUnit, 1);
	hbl->addSpacing(10);
	m_lblTempDiodeIndicator = new QLabel();
	m_lblTempDiodeIndicator->setObjectName("labelTemperatureDiodeIndicator");
	hbl->addWidget(m_lblTempDiodeIndicator, 1);
	m_lblTempDiodeTitle = new QLabel();
	m_lblTempDiodeTitle->setObjectName("labelTemperatureDiodeTitle");
	hbl->addWidget(m_lblTempDiodeTitle, 1);
	m_lblTempDiodeValue = new QLabel("0");
	m_lblTempDiodeValue->setObjectName("labelTemperatureDiodeValue");
	m_lblTempDiodeValue->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	m_lblTempDiodeValue->setVisible(false);
	hbl->addWidget(m_lblTempDiodeValue, 10);
	m_lblTempDiodeUnit = new QLabel(tr("NO SENSOR"));
	m_lblTempDiodeUnit->setObjectName("labelTemperatureDiodeUnit");
	hbl->addWidget(m_lblTempDiodeUnit, 1);
	grl->addLayout(hbl, 3, 0, 1, 2);
	m_wdgAccumulator = new QWidget();
	m_wdgAccumulator->setObjectName("widgetAccumulatorPanel");
	hbl = new QHBoxLayout();
	hbl->setContentsMargins(3, 3, 3, 3);
	m_btnAccReset = new QPushButton(m_wdgAccumulator);
	m_btnAccReset->setObjectName("buttonAccumulatorReset");
	m_btnAccReset->setToolTip(tr("<html><body>Reset accumulator charge counter.<br/>Shortcut: <b>F11</b></body></html>"));
	hbl->addWidget(m_btnAccReset);
	hbl->addStretch();
	m_wdgAccumulator->setLayout(hbl);
	m_lblAccCapacity = new QLabel("0", m_wdgAccumulator);
	m_lblAccCapacity->setObjectName("labelAccumulatorCapacity");
	hbl->addWidget(m_lblAccCapacity);
	m_lblAccCapacityUnit = new QLabel(tr("Ah"), m_wdgAccumulator);
	m_lblAccCapacityUnit->setObjectName("labelAccumulatorCapacityUnit");
	hbl->addWidget(m_lblAccCapacityUnit);
	hbl->addSpacing(30);
	m_lblAccTimer = new QLabel("00:00:00", m_wdgAccumulator);
	m_lblAccTimer->setObjectName("labelAccumulatorTimer");
	hbl->addWidget(m_lblAccTimer);
	grl->addWidget(m_wdgAccumulator, 4, 0, 1, 2);
	grl->setColumnStretch(0, 40);
	grl->setColumnStretch(1, 100);
	hblMain->addLayout(grl, 20);
	vblMain->addLayout(hblMain);

	m_tmrTaskU.setInterval(Resources::applyInterval);
	m_tmrTaskU.setSingleShot(true);
	m_tmrTaskI.setInterval(Resources::applyInterval);
	m_tmrTaskI.setSingleShot(true);

	m_shcOptions = new QShortcut(this);
	m_shcOptions->setKey(Qt::Key_F4);
	m_shcEditTaskU = new QShortcut(this);
	m_shcEditTaskU->setKey(Qt::Key_F5);
	m_shcEditTaskI = new QShortcut(this);
	m_shcEditTaskI->setKey(Qt::Key_F6);
	m_shcPresets = new QShortcut(this);
	m_shcPresets->setKey(Qt::Key_F7);
	m_shcTriggerMode = new QShortcut(this);
	m_shcTriggerMode->setKey(Qt::Key_F8);
	m_shcStart = new QShortcut(this);
	m_shcStart->setKey(Qt::Key_F9);
	m_shcAccumulator = new QShortcut(this);
	m_shcAccumulator->setKey(Qt::Key_F10);
	m_shcChargeReset = new QShortcut(this);
	m_shcChargeReset->setKey(Qt::Key_F11);
	m_shcCalibration = new QShortcut(this);
	m_shcCalibration->setKey(Qt::Key_F12);

	setEnabledButtons(false);

	Resources::readWindowState();
	m_wdgAccumulator->setVisible(Resources::accumulator);
	m_btnAccShow->setChecked(Resources::accumulator);

	connect(m_btnCalibration, SIGNAL(clicked(bool)), m_wdgCalibration, SLOT(setVisible(bool)));
	connect(m_shcOptions, SIGNAL(activated()), this, SLOT(showOptionsDialog()));
	connect(m_shcCalibration, SIGNAL(activated()), this, SLOT(onCalibrationAction()));
	connect(m_shcEditTaskU, SIGNAL(activated()), this, SLOT(editTaskU()));
	connect(m_shcEditTaskI, SIGNAL(activated()), this, SLOT(editTaskI()));
	connect(m_shcPresets, SIGNAL(activated()), this, SLOT(showPresetsDialog()));
	connect(m_shcAccumulator, SIGNAL(activated()), this, SLOT(onAccumulatorAction()));
	connect(m_shcTriggerMode, SIGNAL(activated()), this, SLOT(toggleTrigger()));
	connect(m_shcStart, SIGNAL(activated()), this, SLOT(toggleRunning()));
	connect(m_shcChargeReset, SIGNAL(activated()), this, SIGNAL(resetAccumulator()));
	connect(m_whlTaskU, SIGNAL(valueChanged(double)), this, SLOT(onUTaskWheelChanged(double)));
	connect(m_whlTaskI, SIGNAL(valueChanged(double)), this, SLOT(onITaskWheelChanged(double)));
	connect(m_leTaskU, SIGNAL(editValueChanged(double)), this, SLOT(onUTaskEditChanged(double)));
	connect(m_leTaskI, SIGNAL(editValueChanged(double)), this, SLOT(onITaskEditChanged(double)));
	connect(&m_tmrTaskU, SIGNAL(timeout()), this, SLOT(onUTaskTimeout()));
	connect(&m_tmrTaskI, SIGNAL(timeout()), this, SLOT(onITaskTimeout()));
	connect(m_btnStart, SIGNAL(clicked(bool)), this, SIGNAL(controlRunningChanged()));
	connect(m_btnOnOffTrigger, SIGNAL(clicked(bool)), this, SIGNAL(controlTriggerModeChanged(bool)));
	connect(m_btnPresets, SIGNAL(clicked(bool)), this, SLOT(showPresetsDialog()));
	connect(m_leTaskU, SIGNAL(editActivated()), this, SLOT(onTaskEditActivated()));
	connect(m_leTaskI, SIGNAL(editActivated()), this, SLOT(onTaskEditActivated()));
	connect(m_btnAccReset, SIGNAL(clicked(bool)), this, SIGNAL(resetAccumulator()));
	connect(m_btnAccShow, SIGNAL(clicked(bool)), m_wdgAccumulator, SLOT(setVisible(bool)));
	connect(m_btnOptions, SIGNAL(clicked(bool)), this, SLOT(showOptionsDialog()));
	connect(m_btnPlot, SIGNAL(clicked(bool)), this, SLOT(showPlotDialog()));

	//------------------------------------- device -> window ----------------------------------------------------------
	CalibrationWidget *clb = m_wdgCalibration;
	connect(device, SIGNAL(connected(QString)), this, SLOT(onDeviceConnected(QString)));
	connect(device, SIGNAL(disconnected()), this, SLOT(onDeviceDisconnected()));
	connect(device, SIGNAL(runningState(bool)), this, SLOT(onRunningStateChanged(bool)));
	connect(device, SIGNAL(taskU(double)), this, SLOT(onTaskUChanged(double)));
	connect(device, SIGNAL(taskI(double)), this, SLOT(onTaskIChanged(double)));
	connect(device, SIGNAL(triggerMode(bool)), this, SLOT(onTriggerModeChanged(bool)));
	connect(device, SIGNAL(voltage(double)), this, SLOT(onVoltageChanged(double)));
	connect(device, SIGNAL(current(double)), this, SLOT(onCurrentChanged(double)));
	connect(device, SIGNAL(regimeU(quint16)), this, SLOT(onRegimeUChanged(quint16)));
	connect(device, SIGNAL(regimeI(quint16)), this, SLOT(onRegimeIChanged(quint16)));
	connect(device, SIGNAL(alarm(bool)), this, SLOT(onAlarm(bool)));
	connect(device, SIGNAL(overheat(bool)), this, SLOT(onOverheatChanged(bool)));
	connect(device, SIGNAL(overload(bool)), this, SLOT(onOverloadChanged(bool)));
	connect(device, SIGNAL(temperatureT(quint8)), this, SLOT(onTemperatureTChanged(quint8)));
	connect(device, SIGNAL(temperatureD(quint8)), this, SLOT(onTemperatureDChanged(quint8)));
	connect(device, SIGNAL(noDacU(bool)), this, SLOT(onNoDacUAlarmChanged(bool)));
	connect(device, SIGNAL(noDacI(bool)), this, SLOT(onNoDacIAlarmChanged(bool)));
	connect(device, SIGNAL(noAdc(bool)), this, SLOT(onNoAdcAlarmChanged(bool)));
	connect(device, SIGNAL(accumulatorTime(quint8,quint8,quint8)), this, SLOT(onAccumulatorTimeChanged(quint8,quint8,quint8)));
	connect(device, SIGNAL(accumulatorCapacity(double)), this, SLOT(onAccumulatorCapacityChanged(double)));
	connect(device, SIGNAL(regimeU(quint16)), clb, SLOT(onRegimeUChanged(quint16)));
	connect(device, SIGNAL(regimeI(quint16)), clb, SLOT(onRegimeIChanged(quint16)));
	connect(device, SIGNAL(taskUCorrection(double)), clb, SLOT(onTaskUCorrectionChanged(double)));
	connect(device, SIGNAL(calibrationAdcU(double)), clb, SLOT(onCalibrationAdcUChanged(double)));
	connect(device, SIGNAL(calibrationDacU(int)), clb, SLOT(onCalibrationDacUChanged(int)));
	connect(device, SIGNAL(taskICorrection(double)), clb, SLOT(onTaskICorrectionChanged(double)));
	connect(device, SIGNAL(calibrationAdcI(double)), clb, SLOT(onCalibrationAdcIChanged(double)));
	connect(device, SIGNAL(calibrationDacI(int)), clb, SLOT(onCalibrationDacIChanged(int)));
	connect(device, SIGNAL(fanOnTemperature(int)), clb, SLOT(onFanOnTemperatureChanged(int)));
	connect(device, SIGNAL(powerOffTemperature(int)), clb, SLOT(onPowerOffTemperatureChanged(int)));
	connect(device, SIGNAL(autocorrectionDac(bool)), clb, SLOT(onAutocorrectionDacChanged(bool)));

	//------------------------------------- window -> device ----------------------------------------------------------
	connect(this, SIGNAL(controlTaskUChanged(double)), device, SLOT(requestSetUTask(double)));
	connect(this, SIGNAL(controlTaskIChanged(double)), device, SLOT(requestSetITask(double)));
	connect(this, SIGNAL(controlRunningChanged()), device, SLOT(requestSwitchRunning()));
	connect(this, SIGNAL(controlTriggerModeChanged(bool)), device, SLOT(requestSetTriggerMode(bool)));
	connect(this, SIGNAL(presetSelected(PresetRecord)), device, SLOT(requestApplyPreset(PresetRecord)));
	connect(this, SIGNAL(parameterSelected(int)), device, SLOT(requestMoveToParameter(int)));
	connect(this, SIGNAL(resetAccumulator()), device, SLOT(requestResetAccumulator()));
	connect(this, SIGNAL(portSettingsChanged()), device, SLOT(onPortSettionsChanged()));
	connect(this, SIGNAL(requestIntervalChanged()), device, SLOT(onRequestIntervalChanged()));
	connect(clb, SIGNAL(controlRegimeUChanged(quint16)), device, SLOT(requestSetRegimeU(quint16)));
	connect(clb, SIGNAL(controlRegimeIChanged(quint16)), device, SLOT(requestSetRegimeI(quint16)));
	connect(clb, SIGNAL(controlTaskUCorrectionChanged(double)), device, SLOT(requestSetTaskUCorrection(double)));
	connect(clb, SIGNAL(controlCalibrationAdcUChanged(double)), device, SLOT(requestSetCalibrationAdcU(double)));
	connect(clb, SIGNAL(controlCalibrationDacUChanged(int)), device, SLOT(requestSetCalibrationDacU(int)));
	connect(clb, SIGNAL(controlTaskICorrectionChanged(double)), device, SLOT(requestSetTaskICorrection(double)));
	connect(clb, SIGNAL(controlCalibrationAdcIChanged(double)), device, SLOT(requestSetCalibrationAdcI(double)));
	connect(clb, SIGNAL(controlCalibrationDacIChanged(int)), device, SLOT(requestSetCalibrationDacI(int)));
	connect(clb, SIGNAL(controlFanOnTemperatureChanged(int)), device, SLOT(requestSetFanOnTemperature(int)));
	connect(clb, SIGNAL(controlPowerOffTemperatureChanged(int)), device, SLOT(requestSetPowerOffTemperature(int)));
	connect(clb, SIGNAL(controlAutocorrectionDacChanged(bool)), device, SLOT(requestSetAutocorrectionDac(bool)));
	connect(clb, SIGNAL(parameterSelected(int)), device, SLOT(requestMoveToParameter(int)));

	setGeometry(Resources::windowRect);
}

void MainWindow::showWindow()
{
	if (Resources::windowState & Qt::WindowMinimized) showMinimized();
	else if (Resources::windowState & Qt::WindowMaximized) showMaximized();
	else if (Resources::windowState & Qt::WindowFullScreen) showFullScreen();
	else showNormal();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (m_plotDialog != nullptr)
	{
		m_plotDialog->blockSignals(true);
		m_plotDialog->close();
		delete m_plotDialog;
		m_plotDialog = nullptr;
	}
	if (windowState() & Qt::WindowMinimized) Resources::windowState = Qt::WindowMinimized;
	else if (windowState() & Qt::WindowMaximized) Resources::windowState = Qt::WindowMaximized;
	else if (windowState() & Qt::WindowFullScreen) Resources::windowState = Qt::WindowFullScreen;
	else Resources::windowState = Qt::WindowNoState;
	Resources::windowRect = geometry();
	Resources::accumulator = m_btnAccShow->isChecked();
	Resources::writeWindowState();
	QMainWindow::closeEvent(event);
}

void MainWindow::setEnabledButtons(bool enabled)
{
	m_shcEditTaskU->setEnabled(enabled);
	m_shcEditTaskI->setEnabled(enabled);
	m_shcTriggerMode->setEnabled(enabled);
	m_shcStart->setEnabled(enabled);
	m_btnStart->setEnabled(enabled);
	m_leTaskU->setReadyToEdit(enabled);
	m_leTaskI->setReadyToEdit(enabled);
	m_whlTaskU->setEnabled(enabled);
	m_whlTaskI->setEnabled(enabled);
	m_btnOnOffTrigger->setEnabled(enabled);
	m_btnAccReset->setEnabled(enabled);
	if (!enabled)
	{
		m_leTaskU->blockSignals(true);
		m_leTaskI->blockSignals(true);
		m_whlTaskU->blockSignals(true);
		m_whlTaskI->blockSignals(true);
		m_leTaskU->setValue(0);
		m_leTaskI->setValue(0);
		m_whlTaskU->setValue(0);
		m_whlTaskI->setValue(0);
		m_trmTaskU->setValue(0);
		m_trmTaskI->setValue(0);
		m_leTaskU->blockSignals(false);
		m_leTaskI->blockSignals(false);
		m_whlTaskU->blockSignals(false);
		m_whlTaskI->blockSignals(false);
	}
}

void MainWindow::createPresetsMenu()
{
	m_mnuPresets->clear();
	QAction *action = new QAction(tr("Add current tasks as preset"), m_btnPresets);
	action->setData(QVariant(-1));
	connect(action, SIGNAL(triggered(bool)), this, SLOT(addStateToPresets()));
	m_mnuPresets->addAction(action);
	m_mnuPresets->addSeparator();
	for (int i = 0; i < Resources::presets.size(); ++i)
	{
		const PresetRecord &preset = Resources::presets.at(i);
		action = new QAction(QString("%1V; %2A; %3")
							 .arg(Resources::normalizeValue(preset.voltage))
							 .arg(Resources::normalizeValue(preset.current))
							 .arg(preset.trigger ? tr("TRG") : tr("STB")),
							 m_btnPresets);
		action->setData(QVariant(i));
		m_mnuPresets->addAction(action);
		connect(action, SIGNAL(triggered(bool)), this, SLOT(onPresetAction()));
	}
}

double MainWindow::scaleStep(quint16 value)
{
	double step;
	if (value % 11 == 0) step = value / 11.0;
	else if (value % 12 == 0) step = value / 12.0;
	else if (value % 13 == 0) step = value / 13.0;
	else if (value % 14 == 0) step = value / 14.0;
	else if (value % 16 == 0) step = value / 16.0;
	else if (value % 18 == 0) step = value / 9;
	else step = value / 10.0;
	return step;
}

void MainWindow::onUTaskTimeout()
{
	m_lblApplyU->setVisible(false);
	emit controlTaskUChanged(m_taskU);
}

void MainWindow::onITaskTimeout()
{
	m_lblApplyI->setVisible(false);
	emit controlTaskIChanged(m_taskI);
}

void MainWindow::onUTaskWheelChanged(double v)
{
	m_taskU = v;
	m_leTaskU->setValue(v);
	m_trmTaskU->setValue(v);
	m_lblApplyU->setVisible(true);
	m_tmrTaskU.start();
}

void MainWindow::onITaskWheelChanged(double v)
{
	m_taskI = v;
	m_leTaskI->setValue(v);
	m_trmTaskI->setValue(v);
	m_lblApplyI->setVisible(true);
	m_tmrTaskI.start();
}

void MainWindow::onUTaskEditChanged(double v)
{
	m_whlTaskU->blockSignals(true);
	m_whlTaskU->setValue(v);
	m_trmTaskU->setValue(v);
	m_whlTaskU->blockSignals(false);
	emit controlTaskUChanged(v);
}

void MainWindow::onITaskEditChanged(double v)
{
	m_whlTaskI->blockSignals(true);
	m_whlTaskI->setValue(v);
	m_trmTaskI->setValue(v);
	m_whlTaskI->blockSignals(false);
	emit controlTaskIChanged(v);
}

void MainWindow::editTaskU()
{
	m_leTaskU->activateEdit();
	m_leTaskU->setFocus();
	m_leTaskU->selectAll();
}

void MainWindow::editTaskI()
{
	m_leTaskI->activateEdit();
	m_leTaskI->setFocus();
	m_leTaskI->selectAll();
}

void MainWindow::toggleRunning()
{
	bool run = !m_btnStart->isChecked();
	m_btnStart->blockSignals(true);
	m_btnStart->setChecked(run);
	m_btnStart->blockSignals(false);
	emit controlRunningChanged();
}

void MainWindow::toggleTrigger()
{
	bool trig = !m_btnOnOffTrigger->isChecked();
	m_btnOnOffTrigger->blockSignals(true);
	m_btnOnOffTrigger->setChecked(trig);
	m_btnOnOffTrigger->blockSignals(false);
	emit controlTriggerModeChanged(trig);
}

void MainWindow::onDeviceConnected(const QString &portName)
{
	m_lblAlarm->setVisible(false);
	m_lblNoDacU->setVisible(false);
	m_lblNoDacI->setVisible(false);
	m_lblNoAdc->setVisible(false);
	m_lblConnected->setToolTip(tr("<html><body>Device connected to port: <b>%1</b></body></html>").arg(portName));
	m_lblConnected->setPixmap(m_pxmConnected);
	setEnabledButtons(true);
	m_wdgCalibration->setEnableControls(true);
}

void MainWindow::onDeviceDisconnected()
{
	//Resources::lastValues.clear();
	m_lblAlarm->setText(tr("No connection!"));
	m_lblAlarm->setVisible(true);
	m_lblNoDacU->setVisible(false);
	m_lblNoDacI->setVisible(false);
	m_lblNoAdc->setVisible(false);
	m_lblConnected->setToolTip(tr("<html><body>Device disconnected.</body></html>"));
	m_lblConnected->setPixmap(m_pxmDisconnected);
	setEnabledButtons(false);
	if (m_lblTempTransistorValue->isVisible())
	{
		m_lblTempTransistorValue->setVisible(false);
		m_lblTempTransistorUnit->setText(tr("NO SENSOR"));
	}
	if (m_lblTempDiodeValue->isVisible())
	{
		m_lblTempDiodeValue->setVisible(false);
		m_lblTempDiodeUnit->setText(tr("NO SENSOR"));
	}
	m_lblAccCapacity->setText("0");
	m_lblAccTimer->setText("00:00:00");
	m_wdgCalibration->setEnableControls(false);
}

void MainWindow::onRunningStateChanged(bool isRunning)
{
	m_btnStart->blockSignals(true);
	m_btnStart->setChecked(isRunning);
	m_btnStart->blockSignals(false);
}

void MainWindow::onVoltageChanged(double value)
{
	//OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), value);
	//Resources::lastValues[OscilloscopeVoltage] = v;
	m_lblU->setText(Resources::normalizeValue(value));
}

void MainWindow::onCurrentChanged(double value)
{
	//OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), value);
	//Resources::lastValues[OscilloscopeCurrent] = v;
	m_lblI->setText(Resources::normalizeValue(value));
}

void MainWindow::onTaskUChanged(double value)
{
	//OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), value);
	//Resources::lastValues[OscilloscopeVoltageTask] = v;
	if (m_leTaskU->isReadOnly() && m_lblApplyU->isHidden())
	{
		m_leTaskU->setValue(value);
		m_trmTaskU->setValue(value);
		m_whlTaskU->blockSignals(true);
		m_whlTaskU->setValue(value);
		m_whlTaskU->blockSignals(false);
	}
}

void MainWindow::onTaskIChanged(double value)
{
	//OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), value);
	//Resources::lastValues[OscilloscopeCurrentTask] = v;
	if (m_leTaskI->isReadOnly() && m_lblApplyI->isHidden())
	{
		m_leTaskI->setValue(value);
		m_trmTaskI->setValue(value);
		m_whlTaskI->blockSignals(true);
		m_whlTaskI->setValue(value);
		m_whlTaskI->blockSignals(false);
	}
}

void MainWindow::onTriggerModeChanged(bool trigger)
{
	m_btnOnOffTrigger->blockSignals(true);
	m_btnOnOffTrigger->setChecked(trigger);
	m_btnOnOffTrigger->blockSignals(false);
}

void MainWindow::onRegimeUChanged(quint16 value)
{
	m_leTaskU->setScale(0, value);
	m_trmTaskU->setScale(0, value);
	m_trmTaskU->setScaleStepSize(scaleStep(value));
	m_whlTaskU->setRange(0, value);
}

void MainWindow::onRegimeIChanged(quint16 value)
{
	m_leTaskI->setScale(0, value);
	m_trmTaskI->setScale(0, value);
	m_trmTaskI->setScaleStepSize(scaleStep(value));
	m_whlTaskI->setRange(0, value);
}

void MainWindow::onAlarm(bool ok)
{
	m_lblAlarm->clear();
	m_lblAlarm->setVisible(ok);
}

void MainWindow::onOverheatChanged(bool ok)
{
	if (ok) m_lblAlarm->setText(tr("OVERHEAT!"));
}

void MainWindow::onOverloadChanged(bool ok)
{
	if (ok) m_lblAlarm->setText(tr("OVERLOAD!"));
}

void MainWindow::onTemperatureTChanged(quint8 t)
{
	if (t != 0)
	{
		//OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), t);
		//Resources::lastValues[OscilloscopeTemperatureTransistor] = v;
		if (!m_lblTempTransistorValue->isVisible())
		{
			m_lblTempTransistorValue->setVisible(true);
			m_lblTempTransistorUnit->setText(tr("°C"));
		}
		m_lblTempTransistorValue->setText(QString::number(t));
	}
	else
	{
		if (m_lblTempTransistorValue->isVisible())
		{
			m_lblTempTransistorValue->setVisible(false);
			m_lblTempTransistorUnit->setText(tr("NO SENSOR"));
		}
	}
}

void MainWindow::onTemperatureDChanged(quint8 t)
{
	if (t != 0)
	{
		//OscilloscopeValue v(QDateTime::currentMSecsSinceEpoch(), t);
		//Resources::lastValues[OscilloscopeTemperatureDiode] = v;
		if (!m_lblTempDiodeValue->isVisible())
		{
			m_lblTempDiodeValue->setVisible(true);
			m_lblTempDiodeUnit->setText(tr("°C"));
		}
		m_lblTempDiodeValue->setText(QString::number(t));
	}
	else
	{
		if (m_lblTempDiodeValue->isVisible())
		{
			m_lblTempDiodeValue->setVisible(false);
			m_lblTempDiodeUnit->setText(tr("NO SENSOR"));
		}
	}
}

void MainWindow::onNoDacUAlarmChanged(bool ok)
{
	m_lblNoDacU->setVisible(ok);
}

void MainWindow::onNoDacIAlarmChanged(bool ok)
{
	m_lblNoDacI->setVisible(ok);
}

void MainWindow::onNoAdcAlarmChanged(bool ok)
{
	m_lblNoAdc->setVisible(ok);
}

void MainWindow::onAccumulatorTimeChanged(quint8 hour, quint8 min, quint8 sec)
{
	QString s;
	if (hour < 10) s = "0";
	s += QString::number(hour);
	if (min < 10) s += ":0"; else s += ":";
	s += QString::number(min);
	if (sec < 10) s += ":0"; else s += ":";
	s += QString::number(sec);
	m_lblAccTimer->setText(s);
}

void MainWindow::onAccumulatorCapacityChanged(double value)
{
	m_lblAccCapacity->setText(QString::number(value));
}

void MainWindow::showPresetsDialog()
{
	PresetsDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		Resources::writePresets();
		if (dlg.isEdited()) createPresetsMenu();
		if (dlg.applyRow() >= 0) emit presetSelected(Resources::presets.at(dlg.applyRow()));
	}
}

void MainWindow::onPresetAction()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action != nullptr)
	{
		int i = action->data().toInt();
		if (i >= 0 && i < Resources::presets.size())
		{
			const PresetRecord &preset = Resources::presets.at(i);
			emit presetSelected(preset);
		}
	}
}

void MainWindow::addStateToPresets()
{
	PresetRecord preset;
	preset.voltage = m_leTaskU->value();
	preset.current = m_leTaskI->value();
	preset.trigger = m_btnOnOffTrigger->isChecked();
	Resources::presets.append(preset);
	Resources::writePresets();
	createPresetsMenu();
}

void MainWindow::onTaskEditActivated()
{
	QWidget *control = qobject_cast<QWidget *>(sender());
	if (control != nullptr)
	{
		if (control == m_leTaskU) emit parameterSelected(1);
		else if (control == m_leTaskI) emit parameterSelected(3);
		if (Resources::virtualKeyboard)
		{
			DigitalLineEdit *de = qobject_cast<DigitalLineEdit *>(control);
			if (de != nullptr)
			{
				OnscreenKeyboard *kbd = new OnscreenKeyboard(this);
				connect(de, SIGNAL(editDeactivated()), kbd, SLOT(hide()));
				connect(de, SIGNAL(editDeactivated()), kbd, SLOT(deleteLater()));
				connect(kbd, SIGNAL(cancelEdit()), de, SLOT(cancelEdit()));
				connect(kbd, SIGNAL(applyEdit()), de, SLOT(applyEdit()));
				connect(kbd, SIGNAL(numberKey(quint8)), de, SLOT(numberKey(quint8)));
				connect(kbd, SIGNAL(dotKey()), de, SLOT(dotKey()));
				connect(kbd, SIGNAL(deleteEdit()), de, SLOT(deleteEdit()));
				kbd->resize(kbd->sizeHint());
				QPoint pos = control->pos();
				pos.setY(pos.y());
				pos.setX(pos.x() + control->width());
				kbd->move(pos);
				kbd->show();
			}
		}
	}
}

void MainWindow::onAccumulatorAction()
{
	m_btnAccShow->blockSignals(true);
	m_btnAccShow->setChecked(!m_btnAccShow->isChecked());
	m_btnAccShow->blockSignals(false);
	m_wdgAccumulator->setVisible(m_btnAccShow->isChecked());
}

void MainWindow::onCalibrationAction()
{
	m_btnCalibration->blockSignals(true);
	m_btnCalibration->setChecked(!m_btnCalibration->isChecked());
	m_btnCalibration->blockSignals(false);
	m_wdgCalibration->setVisible(m_btnCalibration->isChecked());
}

void MainWindow::showOptionsDialog()
{
	OptionsDialog dlg(this);
	dlg.setWindowTitle(tr("Options"));
	if (dlg.exec() == QDialog::Accepted)
	{
		setWindowTitle(tr("Power supply [protocol v-%1.%2]").arg(Resources::protocol.arr[1]).arg(Resources::protocol.arr[0]));
		m_wdgCalibration->updateProtocol();
		if (dlg.isPortChanged()) emit portSettingsChanged();
		if (dlg.isRequestIntervalChanged()) emit requestIntervalChanged();
	}
}

void MainWindow::showPlotDialog()
{
	if (m_plotDialog == nullptr)
	{
		m_plotDialog = new PlotDialog(nullptr);
		m_plotDialog->setWindowTitle(QString("%1 - %2").arg(windowTitle()).arg(tr("Oscilloscope")));
		connect(m_plotDialog, SIGNAL(dialogClosed()), this, SLOT(onPlotDialogClosed()));
#ifndef PWRSRC_TEST_SIGNAL
		connect(m_device, SIGNAL(taskU(double)), m_plotDialog, SLOT(onTaskUChanged(double)));
		connect(m_device, SIGNAL(taskI(double)), m_plotDialog, SLOT(onTaskIChanged(double)));
		connect(m_device, SIGNAL(voltage(double)), m_plotDialog, SLOT(onVoltageChanged(double)));
		connect(m_device, SIGNAL(current(double)), m_plotDialog, SLOT(onCurrentChanged(double)));
		connect(m_device, SIGNAL(temperatureT(quint8)), m_plotDialog, SLOT(onTemperatureTChanged(quint8)));
		connect(m_device, SIGNAL(temperatureD(quint8)), m_plotDialog, SLOT(onTemperatureDChanged(quint8)));
#endif
		m_plotDialog->show();
	}
	else m_plotDialog->close();
}

void MainWindow::onPlotDialogClosed()
{
	if (m_plotDialog != nullptr)
	{
		m_btnPlot->blockSignals(true);
		m_btnPlot->setChecked(false);
		m_btnPlot->blockSignals(false);
		delete m_plotDialog;
		m_plotDialog = nullptr;
	}
}
