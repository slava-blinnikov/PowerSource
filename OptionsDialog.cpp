#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSerialPortInfo>

#include "OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent)
{
	m_portChanged = false;
	m_requestIntervalChanged = false;
	QHBoxLayout *hbl;
	QVBoxLayout *vbl;
	QLabel *lbl;
	QVBoxLayout *vblMain = new QVBoxLayout();

	m_chbPortAutosearch = new QCheckBox(tr("Port autosearch"), this);
	m_chbPortAutosearch->setToolTip(tr("Automatic port search from the available ports."));
	m_chbPortAutosearch->setChecked(Resources::portOptions.portAutosearch);
	vblMain->addWidget(m_chbPortAutosearch);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Port:"), this);
	hbl->addWidget(lbl);
	m_cbPort = new QComboBox(this);
	m_cbPort->setToolTip(tr("Serial port name."));
	lbl->setToolTip(m_cbPort->toolTip());
	m_cbPort->setEditable(true);
	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	foreach (QSerialPortInfo info, ports)
	{
		m_cbPort->addItem(info.portName());
	}
	m_cbPort->setEditText(Resources::portOptions.port);
	m_cbPort->setEnabled(!m_chbPortAutosearch->isChecked());
	hbl->addWidget(m_cbPort);
	vblMain->addLayout(hbl);

	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Protocol:"), this);
	hbl->addWidget(lbl);
	m_cbProtocol = new QComboBox(this);
	m_cbProtocol->setToolTip(tr("Microcontroller's firmware version."));
	m_cbProtocol->addItem(QString("4.0"), QVariant(PROTOCOL_VER_4_0));
	m_cbProtocol->addItem(QString("4.1"), QVariant(PROTOCOL_VER_4_1));
	if (Resources::protocol.ver == PROTOCOL_VER_4_0) m_cbProtocol->setCurrentIndex(0); else m_cbProtocol->setCurrentIndex(1);
	lbl->setToolTip(m_cbProtocol->toolTip());
	hbl->addWidget(m_cbProtocol);
	vblMain->addLayout(hbl);

	QGroupBox *grb = new QGroupBox(tr("Port setting"), this);
	grb->setToolTip(tr("Serial port settings"));
	vbl = new QVBoxLayout();
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Baudrate:"), grb);
	hbl->addWidget(lbl);
	m_cbBaudrate = new QComboBox(grb);
	m_cbBaudrate->setToolTip(tr("Serial port baudrate."));
	lbl->setToolTip(m_cbBaudrate->toolTip());
	m_cbBaudrate->addItem("1200");
	m_cbBaudrate->addItem("2400");
	m_cbBaudrate->addItem("4800");
	m_cbBaudrate->addItem("9600");
	m_cbBaudrate->addItem("19200");
	m_cbBaudrate->addItem("38400");
	m_cbBaudrate->addItem("57600");
	m_cbBaudrate->addItem("115200");
	m_cbBaudrate->setEditable(true);
	m_cbBaudrate->setEditText(QString::number(Resources::portOptions.baudrate));
	hbl->addWidget(m_cbBaudrate);
	vbl->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Databits:"), grb);
	hbl->addWidget(lbl);
	m_cbDatabits = new QComboBox(grb);
	m_cbDatabits->setToolTip(tr("Data bits."));
	lbl->setToolTip(m_cbDatabits->toolTip());
	m_cbDatabits->addItem("5");
	m_cbDatabits->addItem("6");
	m_cbDatabits->addItem("7");
	m_cbDatabits->addItem("8");
	m_cbDatabits->setCurrentIndex(Resources::portOptions.databits - 5);
	hbl->addWidget(m_cbDatabits);
	vbl->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Flow control:"), grb);
	hbl->addWidget(lbl);
	m_cbFlowcontrol = new QComboBox(grb);
	m_cbFlowcontrol->setToolTip(tr("Flow control."));
	lbl->setToolTip(m_cbFlowcontrol->toolTip());
	m_cbFlowcontrol->addItem("OFF");
	m_cbFlowcontrol->addItem("HARDWARE");
	m_cbFlowcontrol->addItem("SOFTWARE");
	switch (Resources::portOptions.flowcontrol)
	{
		case QSerialPort::NoFlowControl: m_cbFlowcontrol->setCurrentIndex(0); break;
		case QSerialPort::HardwareControl: m_cbFlowcontrol->setCurrentIndex(1); break;
		case QSerialPort::SoftwareControl: m_cbFlowcontrol->setCurrentIndex(2); break;
		default: m_cbFlowcontrol->setCurrentIndex(0); break;
	}
	hbl->addWidget(m_cbFlowcontrol);
	vbl->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Parity:"), grb);
	hbl->addWidget(lbl);
	m_cbParity = new QComboBox(grb);
	m_cbParity->setToolTip(tr("Parity."));
	lbl->setToolTip(m_cbParity->toolTip());
	m_cbParity->addItem("NONE");
	m_cbParity->addItem("EVEN");
	m_cbParity->addItem("ODD");
	m_cbParity->addItem("SPACE");
	m_cbParity->addItem("MARK");
	switch (Resources::portOptions.parity)
	{
		case QSerialPort::NoParity: m_cbParity->setCurrentIndex(0); break;
		case QSerialPort::EvenParity: m_cbParity->setCurrentIndex(1); break;
		case QSerialPort::OddParity: m_cbParity->setCurrentIndex(2); break;
		case QSerialPort::SpaceParity: m_cbParity->setCurrentIndex(3); break;
		case QSerialPort::MarkParity: m_cbParity->setCurrentIndex(4); break;
		default: m_cbParity->setCurrentIndex(0); break;
	}
	hbl->addWidget(m_cbParity);
	vbl->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Stopbits:"), grb);
	hbl->addWidget(lbl);
	m_cbStopbits = new QComboBox(grb);
	m_cbStopbits->setToolTip(tr("Stopbits."));
	lbl->setToolTip(m_cbStopbits->toolTip());
	m_cbStopbits->addItem("1");
	m_cbStopbits->addItem("1.5");
	m_cbStopbits->addItem("2");
	switch (Resources::portOptions.stopbits)
	{
		case QSerialPort::OneStop: m_cbStopbits->setCurrentIndex(0); break;
		case QSerialPort::OneAndHalfStop: m_cbStopbits->setCurrentIndex(1); break;
		case QSerialPort::TwoStop: m_cbStopbits->setCurrentIndex(2); break;
		default: m_cbStopbits->setCurrentIndex(0); break;
	}
	hbl->addWidget(m_cbStopbits);
	vbl->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("I/O timeout (msec):"), grb);
	hbl->addWidget(lbl);
	m_spbIOTimeout = new QSpinBox(grb);
	m_spbIOTimeout->setToolTip(tr("Input/output operations timeout."));
	lbl->setToolTip(m_spbIOTimeout->toolTip());
	m_spbIOTimeout->setRange(100, 60000);
	m_spbIOTimeout->setValue(Resources::portOptions.ioTimeout);
	hbl->addWidget(m_spbIOTimeout);
	vbl->addLayout(hbl);
	grb->setLayout(vbl);
	vblMain->addWidget(grb);
	grb->setVisible(false);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Request interval (msec):"), this);
	hbl->addWidget(lbl);
	m_spbRequestInterval = new QSpinBox(this);
	m_spbRequestInterval->setToolTip(tr("Device request interval (msec)."));
	lbl->setToolTip(m_spbRequestInterval->toolTip());
	m_spbRequestInterval->setRange(100, 60000);
	m_spbRequestInterval->setValue(Resources::portOptions.reqestInterval);
	hbl->addWidget(m_spbRequestInterval);
	vblMain->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Reconnect interval (sec):"), this);
	hbl->addWidget(lbl);
	m_spbReconnectInterval = new QSpinBox(this);
	m_spbReconnectInterval->setToolTip(tr("Reconnect to device interval (sec)."));
	lbl->setToolTip(m_spbReconnectInterval->toolTip());
	m_spbReconnectInterval->setRange(1, 15*60);
	m_spbReconnectInterval->setValue(Resources::portOptions.reconnectDelay);
	hbl->addWidget(m_spbReconnectInterval);
	vblMain->addLayout(hbl);
	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("Apply timeout (msec):"), this);
	hbl->addWidget(lbl);
	m_spbApplyTimeout = new QSpinBox(this);
	m_spbApplyTimeout->setToolTip(tr("Tasks changes apply timeout (msec)."));
	lbl->setToolTip(m_spbApplyTimeout->toolTip());
	m_spbApplyTimeout->setRange(10, 60000);
	m_spbApplyTimeout->setValue(Resources::applyInterval);
	hbl->addWidget(m_spbApplyTimeout);
	vblMain->addLayout(hbl);
	m_chbFocusSync = new QCheckBox(tr("Focus synchronize"), this);
	m_chbFocusSync->setToolTip(tr("Synchronize focus with device display."));
	m_chbFocusSync->setChecked(Resources::focusSync);
	vblMain->addWidget(m_chbFocusSync);
	m_chbVirtualKeyboard = new QCheckBox(tr("Use virtual keyboard"), this);
	m_chbVirtualKeyboard->setToolTip(tr("Use virtual keyboard for input voltage and current task."));
	m_chbVirtualKeyboard->setChecked(Resources::virtualKeyboard);
	vblMain->addWidget(m_chbVirtualKeyboard);
	vblMain->addStretch();
	hbl = new QHBoxLayout();
	m_btnOk = new QPushButton(tr("Ok"), this);
	hbl->addWidget(m_btnOk);
	hbl->addStretch();
	m_btnCancel = new QPushButton(tr("Cancel"), this);
	hbl->addWidget(m_btnCancel);
	vblMain->addLayout(hbl);
	setLayout(vblMain);
	resize(300, 200);

	connect(m_chbPortAutosearch, SIGNAL(toggled(bool)), this, SLOT(onPortChanged()));
	connect(m_cbPort, SIGNAL(editTextChanged(QString)), this, SLOT(onPortChanged()));
	connect(m_cbProtocol, SIGNAL(currentIndexChanged(int)), this, SLOT(onPortChanged()));
	connect(m_cbBaudrate, SIGNAL(editTextChanged(QString)), this, SLOT(onPortChanged()));
	connect(m_cbDatabits, SIGNAL(currentIndexChanged(int)), this, SLOT(onPortChanged()));
	connect(m_cbFlowcontrol, SIGNAL(currentIndexChanged(int)), this, SLOT(onPortChanged()));
	connect(m_cbParity, SIGNAL(currentIndexChanged(int)), this, SLOT(onPortChanged()));
	connect(m_cbStopbits, SIGNAL(currentIndexChanged(int)), this, SLOT(onPortChanged()));
	connect(m_spbIOTimeout, SIGNAL(valueChanged(int)), this, SLOT(onPortChanged()));
	connect(m_spbRequestInterval, SIGNAL(valueChanged(int)), this, SLOT(onRequestIntervalChanged()));
	connect(m_btnOk, SIGNAL(clicked(bool)), this, SLOT(onOkButtonClicked()));
	connect(m_btnCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

bool OptionsDialog::isPortChanged()
{
	return m_portChanged;
}

bool OptionsDialog::isRequestIntervalChanged()
{
	return m_requestIntervalChanged;
}

void OptionsDialog::onPortChanged()
{
	m_portChanged = true;
	if (sender() == m_chbPortAutosearch) m_cbPort->setEnabled(!m_chbPortAutosearch->isChecked());
}

void OptionsDialog::onRequestIntervalChanged()
{
	m_requestIntervalChanged = true;
}

void OptionsDialog::onOkButtonClicked()
{
	Resources::portOptions.port = m_cbPort->currentText().trimmed();
	Resources::protocol.ver = quint16(m_cbProtocol->currentData().toUInt());
	Resources::portOptions.baudrate = m_cbBaudrate->currentText().toInt();
	switch (m_cbDatabits->currentIndex())
	{
		case 0: Resources::portOptions.databits = QSerialPort::Data5; break;
		case 1: Resources::portOptions.databits = QSerialPort::Data6; break;
		case 2: Resources::portOptions.databits = QSerialPort::Data7; break;
		case 3: Resources::portOptions.databits = QSerialPort::Data8; break;
	}
	switch (m_cbFlowcontrol->currentIndex())
	{
		case 0: Resources::portOptions.flowcontrol = QSerialPort::NoFlowControl; break;
		case 1: Resources::portOptions.flowcontrol = QSerialPort::HardwareControl; break;
		case 2: Resources::portOptions.flowcontrol = QSerialPort::SoftwareControl; break;
	}
	switch (m_cbParity->currentIndex())
	{
		case 0: Resources::portOptions.parity = QSerialPort::NoParity; break;
		case 1: Resources::portOptions.parity = QSerialPort::EvenParity; break;
		case 2: Resources::portOptions.parity = QSerialPort::OddParity; break;
		case 3: Resources::portOptions.parity = QSerialPort::SpaceParity; break;
		case 4: Resources::portOptions.parity = QSerialPort::MarkParity; break;
	}
	switch (m_cbStopbits->currentIndex())
	{
		case 0: Resources::portOptions.stopbits = QSerialPort::OneStop; break;
		case 1: Resources::portOptions.stopbits = QSerialPort::OneAndHalfStop; break;
		case 2: Resources::portOptions.stopbits = QSerialPort::TwoStop; break;
	}
	Resources::portOptions.ioTimeout = m_spbIOTimeout->value();
	Resources::portOptions.reqestInterval = m_spbRequestInterval->value();
	Resources::applyInterval = m_spbApplyTimeout->value();
	Resources::focusSync = m_chbFocusSync->isChecked();
	Resources::virtualKeyboard = m_chbVirtualKeyboard->isChecked();
	Resources::portOptions.portAutosearch = m_chbPortAutosearch->isChecked();
	Resources::portOptions.reconnectDelay = m_spbReconnectInterval->value();
	Resources::writeConfig();
	accept();
}
