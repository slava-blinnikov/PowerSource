#include <QHBoxLayout>
#include <QVBoxLayout>

#include "OscilloscopeWidget.h"

OscilloscopeWidget::OscilloscopeWidget(OscilloscopeRayList *rays, const OscilloscopeHandle &handle, QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *hblMain = new QHBoxLayout();
	hblMain->setMargin(0);
	hblMain->setContentsMargins(0, 0, 0, 0);

	m_oscilloscope = new Oscilloscope(rays, handle, this);
	hblMain->addWidget(m_oscilloscope);

	QVBoxLayout *vblTools = new QVBoxLayout();
	vblTools->setMargin(0);
	vblTools->setContentsMargins(0, 0, 0, 0);

	QHBoxLayout *hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	m_btnClose = new QPushButton(QIcon(":icons/remove.png"), QString(), this);
	m_btnClose->setToolTip(tr("Close current oscilloscope window."));
	m_btnClose->setFlat(true);
	m_btnClose->setFocusPolicy(Qt::NoFocus);
	m_btnClose->setIconSize(QSize(16, 16));
	hbl->addWidget(m_btnClose);
	hbl->addStretch();
	vblTools->addLayout(hbl);

	vblTools->addStretch();
	QGroupBox *grb = new QGroupBox(tr("Rays"), this);
	QVBoxLayout *vbl = new QVBoxLayout();
	grb->setLayout(vbl);
	m_chbVoltage = new QCheckBox(Resources::rayNames[OscilloscopeVoltage], grb);
	m_chbVoltage->setToolTip(tr("Voltage ray"));
	m_chbVoltage->setChecked(handle.rays & OscilloscopeVoltage);
	vbl->addWidget(m_chbVoltage);
	m_chbVoltageTask = new QCheckBox(Resources::rayNames[OscilloscopeVoltageTask], grb);
	m_chbVoltageTask->setToolTip(tr("Voltage task ray"));
	m_chbVoltageTask->setChecked(handle.rays & OscilloscopeVoltageTask);
	vbl->addWidget(m_chbVoltageTask);
	m_chbCurrent = new QCheckBox(Resources::rayNames[OscilloscopeCurrent], grb);
	m_chbCurrent->setToolTip(tr("Current ray"));
	m_chbCurrent->setChecked(handle.rays & OscilloscopeCurrent);
	vbl->addWidget(m_chbCurrent);
	m_chbCurrentTask = new QCheckBox(Resources::rayNames[OscilloscopeCurrentTask], grb);
	m_chbCurrentTask->setToolTip(tr("Current task ray"));
	m_chbCurrentTask->setChecked(handle.rays & OscilloscopeCurrentTask);
	vbl->addWidget(m_chbCurrentTask);
	m_chbTemperatureD = new QCheckBox(Resources::rayNames[OscilloscopeTemperatureDiode], grb);
	m_chbTemperatureD->setToolTip(tr("Diode temperature ray"));
	m_chbTemperatureD->setChecked(handle.rays & OscilloscopeTemperatureDiode);
	vbl->addWidget(m_chbTemperatureD);
	m_chbTemperatureT = new QCheckBox(Resources::rayNames[OscilloscopeTemperatureTransistor], grb);
	m_chbTemperatureT->setToolTip(tr("Transistor temperature ray"));
	m_chbTemperatureT->setChecked(handle.rays & OscilloscopeTemperatureTransistor);
	vbl->addWidget(m_chbTemperatureT);
	vblTools->addWidget(grb);

	QLabel *lbl;
	hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	lbl = new QLabel(tr("grid:"), this);
	hbl->addWidget(lbl);
	m_spbGrid = new QSpinBox(this);
	m_spbGrid->setToolTip(tr("Vertical grid lines count."));
	m_spbGrid->installEventFilter(this);
	m_spbGrid->setRange(3, 1000);
	m_spbGrid->setValue(handle.y_grid);
	hbl->addWidget(m_spbGrid);
	hbl->addStretch();
	vblTools->addLayout(hbl);

	hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	lbl = new QLabel(tr("min:"), this);
	hbl->addWidget(lbl);
	m_spbMin = new QDoubleSpinBox(this);
	m_spbMin->setToolTip(tr("Lower value bound."));
	m_spbMin->installEventFilter(this);
	m_spbMin->setRange(0, 1000);
	m_spbMin->setValue(handle.y_min);
	m_spbMin->setSingleStep(0.1);
	m_spbMin->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
	hbl->addWidget(m_spbMin);
	hbl->addStretch();
	lbl = new QLabel(tr("max:"), this);
	hbl->addWidget(lbl);
	m_spbMax = new QDoubleSpinBox(this);
	m_spbMax->setToolTip(tr("Upper value bound."));
	m_spbMax->installEventFilter(this);
	m_spbMax->setRange(0, 1000);
	m_spbMax->setValue(handle.y_max);
	m_spbMax->setSingleStep(0.1);
	m_spbMax->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
	hbl->addWidget(m_spbMax);
	vblTools->addLayout(hbl);

	m_btnApplyScale = new QPushButton(tr("Apply scale"), this);
	m_btnApplyScale->setToolTip(tr("Apply vertical scale for current oscilloscope."));
	m_btnApplyScale->setEnabled(false);
	vblTools->addWidget(m_btnApplyScale);
	vblTools->addStretch();
	hblMain->addLayout(vblTools);
	hblMain->setStretchFactor(m_oscilloscope, 100);
	hblMain->setStretchFactor(vblTools, 10);
	setLayout(hblMain);

	connect(m_btnClose, SIGNAL(clicked(bool)), this, SLOT(closeOscilloscopeWidget()));
	connect(m_chbVoltage, SIGNAL(toggled(bool)), this, SLOT(toggleRayState(bool)));
	connect(m_chbVoltageTask, SIGNAL(toggled(bool)), this, SLOT(toggleRayState(bool)));
	connect(m_chbCurrent, SIGNAL(toggled(bool)), this, SLOT(toggleRayState(bool)));
	connect(m_chbCurrentTask, SIGNAL(toggled(bool)), this, SLOT(toggleRayState(bool)));
	connect(m_chbTemperatureD, SIGNAL(toggled(bool)), this, SLOT(toggleRayState(bool)));
	connect(m_chbTemperatureT, SIGNAL(toggled(bool)), this, SLOT(toggleRayState(bool)));
	connect(m_btnApplyScale, SIGNAL(clicked(bool)), this, SLOT(applyScale()));

	connect(m_spbGrid, SIGNAL(valueChanged(int)), this, SLOT(onScaleEditChanged()));
	connect(m_spbMin, SIGNAL(valueChanged(double)), this, SLOT(onScaleEditChanged()));
	connect(m_spbMax, SIGNAL(valueChanged(double)), this, SLOT(onScaleEditChanged()));
}

Oscilloscope *OscilloscopeWidget::oscilloscope()
{
	return m_oscilloscope;
}

bool OscilloscopeWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		int key = keyEvent->key();
		if ((key == Qt::Key_Return) || (key == Qt::Key_Enter))
		{
			applyScale();
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

void OscilloscopeWidget::closeOscilloscopeWidget()
{
	emit closeButtonClicked(this);
}

void OscilloscopeWidget::toggleRayState(bool enabled)
{
	QCheckBox *chb = qobject_cast<QCheckBox*>(sender());
	if (chb != nullptr)
	{
		if (chb == m_chbVoltage) m_oscilloscope->toggleRayState(OscilloscopeVoltage, enabled);
		if (chb == m_chbVoltageTask) m_oscilloscope->toggleRayState(OscilloscopeVoltageTask, enabled);
		if (chb == m_chbCurrent) m_oscilloscope->toggleRayState(OscilloscopeCurrent, enabled);
		if (chb == m_chbCurrentTask) m_oscilloscope->toggleRayState(OscilloscopeCurrentTask, enabled);
		if (chb == m_chbTemperatureD) m_oscilloscope->toggleRayState(OscilloscopeTemperatureDiode, enabled);
		if (chb == m_chbTemperatureT) m_oscilloscope->toggleRayState(OscilloscopeTemperatureTransistor, enabled);
		m_btnApplyScale->setEnabled(false);
		emit oscilloscopeChanged(m_oscilloscope);
	}
}

void OscilloscopeWidget::applyScale()
{
	m_oscilloscope->setYScale(m_spbMin->value(), m_spbMax->value(), m_spbGrid->value());
	m_oscilloscope->replot();
	m_btnApplyScale->setEnabled(false);
	emit oscilloscopeChanged(m_oscilloscope);
}

void OscilloscopeWidget::onScaleEditChanged()
{
	m_btnApplyScale->setEnabled(true);
}
