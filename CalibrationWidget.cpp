#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "CalibrationWidget.h"

CalibrationWidget::CalibrationWidget(QWidget *parent) : QWidget(parent)
{
	m_edited = NoCalibrationParameterFlag;
	QVBoxLayout *vblMain = new QVBoxLayout();
	vblMain->setContentsMargins(5, 3, 5, 3);
	setLayout(vblMain);
	QLabel *lbl;
	QGridLayout *grl;

	lbl = new QLabel(QString("--------%1--------").arg(tr("Voltage")), this);
	lbl->setAlignment(Qt::AlignCenter);
	vblMain->addWidget(lbl);

	grl = new QGridLayout();
	m_lblRegimeU = new QLabel(tr("Regime"), this);
	grl->addWidget(m_lblRegimeU, 0, 0);
	m_spbRegimeU = new QSpinBox(this);
	m_spbRegimeU->setRange(20, 80);
	m_spbRegimeU->setSingleStep(1);
	m_spbRegimeU->clear();
	grl->addWidget(m_spbRegimeU, 0, 1);
	lbl = new QLabel(tr("V"), this);
	grl->addWidget(lbl, 0, 2);

	m_lblCorrUref = new QLabel(tr("Task correction"), this);
	grl->addWidget(m_lblCorrUref, 1, 0);
	m_spbCorrUref = new QDoubleSpinBox(this);
	m_spbCorrUref->setRange(-0.2, 0.2);
	m_spbCorrUref->setSingleStep(0.01);
	m_spbCorrUref->clear();
	grl->addWidget(m_spbCorrUref, 1, 1);
	lbl = new QLabel(tr("V"), this);
	grl->addWidget(lbl, 1, 2);

	m_lblCalibrAdcU = new QLabel(tr("ADC calibr.factor"), this);
	grl->addWidget(m_lblCalibrAdcU, 2, 0);
	m_spbCalibrAdcU = new QDoubleSpinBox(this);
	m_spbCalibrAdcU->setRange(39.0, 85.0);
	m_spbCalibrAdcU->setSingleStep(0.01);
	m_spbCalibrAdcU->clear();
	grl->addWidget(m_spbCalibrAdcU, 2, 1);
	lbl = new QLabel(tr("V"), this);
	grl->addWidget(lbl, 2, 2);

	m_lblCalibrDacU = new QLabel(tr("DAC calibr.factor"), this);
	grl->addWidget(m_lblCalibrDacU, 3, 0);
	m_spbCalibrDacU = new QSpinBox(this);
	m_spbCalibrDacU->setRange(3000, 4095);
	m_spbCalibrDacU->setSingleStep(1);
	m_spbCalibrDacU->clear();
	grl->addWidget(m_spbCalibrDacU, 3, 1);

	vblMain->addLayout(grl);

	vblMain->addSpacing(15);
	lbl = new QLabel(QString("--------%1--------").arg(tr("Current")), this);
	lbl->setAlignment(Qt::AlignCenter);
	vblMain->addWidget(lbl);

	grl = new QGridLayout();
	m_lblRegimeI = new QLabel(tr("Regime"), this);
	grl->addWidget(m_lblRegimeI, 0, 0);
	m_spbRegimeI = new QSpinBox(this);
	m_spbRegimeI->setRange(3, 30);
	m_spbRegimeI->setSingleStep(1);
	m_spbRegimeI->clear();
	grl->addWidget(m_spbRegimeI, 0, 1);
	lbl = new QLabel("A", this);
	grl->addWidget(lbl, 0, 2);

	m_lblCorrIref = new QLabel(tr("Task correction"), this);
	grl->addWidget(m_lblCorrIref, 1, 0);
	m_spbCorrIref = new QDoubleSpinBox(this);
	m_spbCorrIref->setRange(-0.5, 0.5);
	m_spbCorrIref->setSingleStep(0.01);
	m_spbCorrIref->clear();
	grl->addWidget(m_spbCorrIref, 1, 1);
	lbl = new QLabel("A", this);
	grl->addWidget(lbl, 1, 2);

	m_lblCalibrAdcI = new QLabel(tr("ADC calibr.factor"), this);
	grl->addWidget(m_lblCalibrAdcI, 2, 0);
	m_spbCalibrAdcI = new QDoubleSpinBox(this);
	m_spbCalibrAdcI->setRange(2.5, 33.0);
	m_spbCalibrAdcI->setSingleStep(0.01);
	m_spbCalibrAdcI->clear();
	grl->addWidget(m_spbCalibrAdcI, 2, 1);
	lbl = new QLabel(tr("A"), this);
	grl->addWidget(lbl, 2, 2);

	m_lblCalibrDacI = new QLabel(tr("DAC calibr.factor"), this);
	grl->addWidget(m_lblCalibrDacI, 3, 0);
	m_spbCalibrDacI = new QSpinBox(this);
	m_spbCalibrDacI->setRange(3000, 4095);
	m_spbCalibrDacI->setSingleStep(1);
	m_spbCalibrDacI->clear();
	grl->addWidget(m_spbCalibrDacI, 3, 1);

	vblMain->addLayout(grl);

	vblMain->addSpacing(15);
	lbl = new QLabel(QString("--------%1--------").arg(tr("Temperature")), this);
	lbl->setAlignment(Qt::AlignCenter);
	vblMain->addWidget(lbl);

	grl = new QGridLayout();
	m_lblTemperatureFanOn = new QLabel(tr("Fan on"), this);
	grl->addWidget(m_lblTemperatureFanOn, 0, 0);
	m_spbTemperatureFanOn = new QSpinBox(this);
	m_spbTemperatureFanOn->setRange(0, 70);
	m_spbTemperatureFanOn->setSingleStep(1);
	m_spbTemperatureFanOn->clear();
	grl->addWidget(m_spbTemperatureFanOn, 0, 1);
	lbl = new QLabel(tr("°C"), this);
	grl->addWidget(lbl, 0, 2);

	m_lblTemperaturePowerOff = new QLabel(tr("Power off"), this);
	grl->addWidget(m_lblTemperaturePowerOff, 1, 0);
	m_spbTemperaturePowerOff = new QSpinBox(this);
	m_spbTemperaturePowerOff->setRange(30, 100);
	m_spbTemperaturePowerOff->setSingleStep(1);
	m_spbTemperaturePowerOff->clear();
	grl->addWidget(m_spbTemperaturePowerOff, 1, 1);
	lbl = new QLabel(tr("°C"), this);
	grl->addWidget(lbl, 1, 2);
	vblMain->addLayout(grl);

	vblMain->addSpacing(10);
	m_chbAutocorrDac = new QCheckBox(tr("DACs autocorrection"), this);
	vblMain->addWidget(m_chbAutocorrDac);
	m_chbAutocorrDac->setVisible(Resources::protocol.ver >= PROTOCOL_VER_4_1);

	vblMain->addStretch();

	QHBoxLayout *hbl = new QHBoxLayout();
	m_btnApply = new QPushButton(tr("Apply"), this);
	hbl->addWidget(m_btnApply);
	hbl->addStretch();
	m_btnCancel = new QPushButton(tr("Cancel"), this);
	hbl->addWidget(m_btnCancel);
	vblMain->addLayout(hbl);

	connect(m_chbAutocorrDac, SIGNAL(clicked(bool)), this, SIGNAL(controlAutocorrectionDacChanged(bool)));
	connect(m_spbRegimeU, SIGNAL(valueChanged(int)), this, SLOT(onEditParameter()));
	connect(m_spbCorrUref, SIGNAL(valueChanged(double)), this, SLOT(onEditParameter()));
	connect(m_spbCalibrAdcU, SIGNAL(valueChanged(double)), this, SLOT(onEditParameter()));
	connect(m_spbCalibrDacU, SIGNAL(valueChanged(int)), this, SLOT(onEditParameter()));
	connect(m_spbRegimeI, SIGNAL(valueChanged(int)), this, SLOT(onEditParameter()));
	connect(m_spbCorrIref, SIGNAL(valueChanged(double)), this, SLOT(onEditParameter()));
	connect(m_spbCalibrAdcI, SIGNAL(valueChanged(double)), this, SLOT(onEditParameter()));
	connect(m_spbCalibrDacI, SIGNAL(valueChanged(int)), this, SLOT(onEditParameter()));
	connect(m_spbTemperatureFanOn, SIGNAL(valueChanged(int)), this, SLOT(onEditParameter()));
	connect(m_spbTemperaturePowerOff, SIGNAL(valueChanged(int)), this, SLOT(onEditParameter()));
	m_spbRegimeU->installEventFilter(this);
	m_spbCorrUref->installEventFilter(this);
	m_spbCalibrDacU->installEventFilter(this);
	m_spbCalibrAdcU->installEventFilter(this);
	m_spbRegimeI->installEventFilter(this);
	m_spbCorrIref->installEventFilter(this);
	m_spbCalibrDacI->installEventFilter(this);
	m_spbCalibrAdcI->installEventFilter(this);
	m_spbTemperatureFanOn->installEventFilter(this);
	m_spbTemperaturePowerOff->installEventFilter(this);

	connect(m_btnCancel, SIGNAL(clicked(bool)), this, SLOT(cancelEdit()));
	connect(m_btnApply, SIGNAL(clicked(bool)), this, SLOT(applyEdit()));

	setEnableControls(false);
}

void CalibrationWidget::onEditParameter()
{
	QWidget *obj = qobject_cast<QWidget *>(sender());
	if (obj != nullptr)
	{
		/*if (obj == m_chbAutocorrDac)
		{
			if ((m_edited & DACAutocorrectionFlag) == 0)
			{
				m_edited |= DACAutocorrectionFlag;
				m_chbAutocorrDac->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else*/
		if (obj == m_spbRegimeU)
		{
			if ((m_edited & RegimeUFlag) == 0)
			{
				m_edited |= RegimeUFlag;
				m_lblRegimeU->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbCorrUref)
		{
			if ((m_edited & CorrectionUrefFlag) == 0)
			{
				m_edited |= CorrectionUrefFlag;
				m_lblCorrUref->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbCalibrDacU)
		{
			if ((m_edited & CalibrationDacUFlag) == 0)
			{
				m_edited |= CalibrationDacUFlag;
				m_lblCalibrDacU->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbCalibrAdcU)
		{
			if ((m_edited & CalibrationAdcUFlag) == 0)
			{
				m_edited |= CalibrationAdcUFlag;
				m_lblCalibrAdcU->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbRegimeI)
		{
			if ((m_edited & RegimeIFlag) == 0)
			{
				m_edited |= RegimeIFlag;
				m_lblRegimeI->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbCorrIref)
		{
			if ((m_edited & CorrectionIrefFlag) == 0)
			{
				m_edited |= CorrectionIrefFlag;
				m_lblCorrIref->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbCalibrDacI)
		{
			if ((m_edited & CalibrationDacIFlag) == 0)
			{
				m_edited |= CalibrationDacIFlag;
				m_lblCalibrDacI->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbCalibrAdcI)
		{
			if ((m_edited & CalibrationAdcIFlag) == 0)
			{
				m_edited |= CalibrationAdcIFlag;
				m_lblCalibrAdcI->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbTemperatureFanOn)
		{
			if ((m_edited & FanOnTFlag) == 0)
			{
				m_edited |= FanOnTFlag;
				m_lblTemperatureFanOn->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		else if (obj == m_spbTemperaturePowerOff)
		{
			if ((m_edited & PowerOffTFlag) == 0)
			{
				m_edited |= PowerOffTFlag;
				m_lblTemperaturePowerOff->setStyleSheet(EDITED_PARAMETER_STYLE);
			}
		}
		m_btnApply->setEnabled(m_edited != NoCalibrationParameterFlag);
		m_btnCancel->setEnabled(m_btnApply->isEnabled());
	}
}

void CalibrationWidget::cancelEdit()
{
	if ((m_edited & RegimeUFlag) != 0)
	{
		m_spbRegimeU->blockSignals(true);
		m_spbRegimeU->setValue(m_saveRegimeU);
		m_spbRegimeU->blockSignals(false);
		m_lblRegimeU->setStyleSheet("");
	}
	if ((m_edited & RegimeIFlag) != 0)
	{
		m_spbRegimeI->blockSignals(true);
		m_spbRegimeI->setValue(m_saveRegimeI);
		m_spbRegimeI->blockSignals(false);
		m_lblRegimeI->setStyleSheet("");
	}
	if ((m_edited & CorrectionUrefFlag) != 0)
	{
		m_spbCorrUref->blockSignals(true);
		m_spbCorrUref->setValue(m_saveCorrUref);
		m_spbCorrUref->blockSignals(false);
		m_lblCorrUref->setStyleSheet("");
	}
	if ((m_edited & CalibrationAdcUFlag) != 0)
	{
		m_spbCalibrAdcU->blockSignals(true);
		m_spbCalibrAdcU->setValue(m_saveCalibrAdcU);
		m_spbCalibrAdcU->blockSignals(false);
		m_lblCalibrAdcU->setStyleSheet("");
	}
	if ((m_edited & CalibrationDacUFlag) != 0)
	{
		m_spbCalibrDacU->blockSignals(true);
		m_spbCalibrDacU->setValue(m_saveCalibrDacU);
		m_spbCalibrDacU->blockSignals(false);
		m_lblCalibrDacU->setStyleSheet("");
	}
	if ((m_edited & CorrectionIrefFlag) != 0)
	{
		m_spbCorrIref->blockSignals(true);
		m_spbCorrIref->setValue(m_saveCorrIref);
		m_spbCorrIref->blockSignals(false);
		m_lblCorrIref->setStyleSheet("");
	}
	if ((m_edited & CalibrationAdcIFlag) != 0)
	{
		m_spbCalibrAdcI->blockSignals(true);
		m_spbCalibrAdcI->setValue(m_saveCalibrAdcI);
		m_spbCalibrAdcI->blockSignals(false);
		m_lblCalibrAdcI->setStyleSheet("");
	}
	if ((m_edited & CalibrationDacIFlag) != 0)
	{
		m_spbCalibrDacI->blockSignals(true);
		m_spbCalibrDacI->setValue(m_saveCalibrDacI);
		m_spbCalibrDacI->blockSignals(false);
		m_lblCalibrDacI->setStyleSheet("");
	}
	if ((m_edited & FanOnTFlag) != 0)
	{
		m_spbTemperatureFanOn->blockSignals(true);
		m_spbTemperatureFanOn->setValue(m_saveTemperatureFanOn);
		m_spbTemperatureFanOn->blockSignals(false);
		m_lblTemperatureFanOn->setStyleSheet("");
	}
	if ((m_edited & PowerOffTFlag) != 0)
	{
		m_spbTemperaturePowerOff->blockSignals(true);
		m_spbTemperaturePowerOff->setValue(m_saveTemperaturePowerOff);
		m_spbTemperaturePowerOff->blockSignals(false);
		m_lblTemperaturePowerOff->setStyleSheet("");
	}
	/*if ((m_edited & DACAutocorrectionFlag) != 0)
	{
		m_chbAutocorrDac->blockSignals(true);
		m_chbAutocorrDac->setChecked(m_saveAutocorrDac);
		m_chbAutocorrDac->blockSignals(false);
		m_chbAutocorrDac->setStyleSheet("");
	}*/
	m_edited = NoCalibrationParameterFlag;
	m_btnApply->setEnabled(false);
	m_btnCancel->setEnabled(false);
}

void CalibrationWidget::applyEdit()
{
	if ((m_edited & RegimeUFlag) != 0)
	{
		emit controlRegimeUChanged(quint16(m_spbRegimeU->value()));
		m_lblRegimeU->setStyleSheet("");
	}
	if ((m_edited & RegimeIFlag) != 0)
	{
		emit controlRegimeIChanged(quint16(m_spbRegimeI->value()));
		m_lblRegimeI->setStyleSheet("");
	}
	if ((m_edited & CorrectionUrefFlag) != 0)
	{
		emit controlTaskUCorrectionChanged(m_spbCorrUref->value());
		m_lblCorrUref->setStyleSheet("");
	}
	if ((m_edited & CalibrationAdcUFlag) != 0)
	{
		emit controlCalibrationAdcUChanged(m_spbCalibrAdcU->value());
		m_lblCalibrAdcU->setStyleSheet("");
	}
	if ((m_edited & CalibrationDacUFlag) != 0)
	{
		emit controlCalibrationDacUChanged(m_spbCalibrDacU->value());
		m_lblCalibrDacU->setStyleSheet("");
	}
	if ((m_edited & CorrectionIrefFlag) != 0)
	{
		emit controlTaskICorrectionChanged(m_spbCorrIref->value());
		m_lblCorrIref->setStyleSheet("");
	}
	if ((m_edited & CalibrationAdcIFlag) != 0)
	{
		emit controlCalibrationAdcIChanged(m_spbCalibrAdcI->value());
		m_lblCalibrAdcI->setStyleSheet("");
	}
	if ((m_edited & CalibrationDacIFlag) != 0)
	{
		emit controlCalibrationDacIChanged(m_spbCalibrDacI->value());
		m_lblCalibrDacI->setStyleSheet("");
	}
	if ((m_edited & FanOnTFlag) != 0)
	{
		emit controlFanOnTemperatureChanged(m_spbTemperatureFanOn->value());
		m_lblTemperatureFanOn->setStyleSheet("");
	}
	if ((m_edited & PowerOffTFlag) != 0)
	{
		emit controlPowerOffTemperatureChanged(m_spbTemperaturePowerOff->value());
		m_lblTemperaturePowerOff->setStyleSheet("");
	}
	/*if ((m_edited & DACAutocorrectionFlag) != 0)
	{
		emit controlAutocorrectionDacChanged(m_chbAutocorrDac->isChecked());
		m_chbAutocorrDac->setStyleSheet("");
	}*/
	m_edited = NoCalibrationParameterFlag;
	m_btnApply->setEnabled(false);
	m_btnCancel->setEnabled(false);
}

void CalibrationWidget::onControlReturnPressed(QWidget *control)
{
	if (control == m_spbRegimeU)
	{
		if ((m_edited & RegimeUFlag) != 0)
		{
			m_edited ^= RegimeUFlag;
			emit controlRegimeUChanged(quint16(m_spbRegimeU->value()));
			m_lblRegimeU->setStyleSheet("");
		}
	}
	else if (control == m_spbCorrUref)
	{
		if ((m_edited & CorrectionUrefFlag) != 0)
		{
			m_edited ^= CorrectionUrefFlag;
			emit controlTaskUCorrectionChanged(m_spbCorrUref->value());
			m_lblCorrUref->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrDacU)
	{
		if ((m_edited & CalibrationDacUFlag) != 0)
		{
			m_edited ^= CalibrationDacUFlag;
			emit controlCalibrationDacUChanged(m_spbCalibrDacU->value());
			m_lblCalibrDacU->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrAdcU)
	{
		if ((m_edited & CalibrationAdcUFlag) != 0)
		{
			m_edited ^= CalibrationAdcUFlag;
			emit controlCalibrationAdcUChanged(m_spbCalibrAdcU->value());
			m_lblCalibrAdcU->setStyleSheet("");
		}
	}
	else if (control == m_spbRegimeI)
	{
		if ((m_edited & RegimeIFlag) != 0)
		{
			m_edited ^= RegimeIFlag;
			emit controlRegimeIChanged(quint16(m_spbRegimeI->value()));
			m_lblRegimeI->setStyleSheet("");
		}
	}
	else if (control == m_spbCorrIref)
	{
		if ((m_edited & CorrectionIrefFlag) != 0)
		{
			m_edited ^= CorrectionIrefFlag;
			emit controlTaskICorrectionChanged(m_spbCorrIref->value());
			m_lblCorrIref->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrDacI)
	{
		if ((m_edited & CalibrationDacIFlag) != 0)
		{
			m_edited ^= CalibrationDacIFlag;
			emit controlCalibrationDacIChanged(m_spbCalibrDacI->value());
			m_lblCalibrDacI->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrAdcI)
	{
		if ((m_edited & CalibrationAdcIFlag) != 0)
		{
			m_edited ^= CalibrationAdcIFlag;
			emit controlCalibrationAdcIChanged(m_spbCalibrAdcI->value());
			m_lblCalibrAdcI->setStyleSheet("");
		}
	}
	else if (control == m_spbTemperatureFanOn)
	{
		if ((m_edited & FanOnTFlag) != 0)
		{
			m_edited ^= FanOnTFlag;
			emit controlFanOnTemperatureChanged(m_spbTemperatureFanOn->value());
			m_lblTemperatureFanOn->setStyleSheet("");
		}
	}
	else if (control == m_spbTemperaturePowerOff)
	{
		if ((m_edited & PowerOffTFlag) != 0)
		{
			m_edited ^= PowerOffTFlag;
			emit controlPowerOffTemperatureChanged(m_spbTemperaturePowerOff->value());
			m_lblTemperaturePowerOff->setStyleSheet("");
		}
	}
	if (m_edited == NoCalibrationParameterFlag)
	{
		m_btnApply->setEnabled(false);
		m_btnCancel->setEnabled(false);
	}
}

void CalibrationWidget::onControlEscapePressed(QWidget *control)
{
	if (control == m_spbRegimeU)
	{
		if ((m_edited & RegimeUFlag) != 0)
		{
			m_edited ^= RegimeUFlag;
			m_spbRegimeU->blockSignals(true);
			m_spbRegimeU->setValue(m_saveRegimeU);
			m_spbRegimeU->blockSignals(false);
			m_lblRegimeU->setStyleSheet("");
		}
	}
	else if (control == m_spbCorrUref)
	{
		if ((m_edited & CorrectionUrefFlag) != 0)
		{
			m_edited ^= CorrectionUrefFlag;
			m_spbCorrUref->blockSignals(true);
			m_spbCorrUref->setValue(m_saveCorrUref);
			m_spbCorrUref->blockSignals(false);
			m_lblCorrUref->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrDacU)
	{
		if ((m_edited & CalibrationDacUFlag) != 0)
		{
			m_edited ^= CalibrationDacUFlag;
			m_spbCalibrDacU->blockSignals(true);
			m_spbCalibrDacU->setValue(m_saveCalibrDacU);
			m_spbCalibrDacU->blockSignals(false);
			m_lblCalibrDacU->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrAdcU)
	{
		if ((m_edited & CalibrationAdcUFlag) != 0)
		{
			m_edited ^= CalibrationAdcUFlag;
			m_spbCalibrAdcU->blockSignals(true);
			m_spbCalibrAdcU->setValue(m_saveCalibrAdcU);
			m_spbCalibrAdcU->blockSignals(false);
			m_lblCalibrAdcU->setStyleSheet("");
		}
	}
	else if (control == m_spbRegimeI)
	{
		if ((m_edited & RegimeIFlag) != 0)
		{
			m_edited ^= RegimeIFlag;
			m_spbRegimeI->blockSignals(true);
			m_spbRegimeI->setValue(m_saveRegimeI);
			m_spbRegimeI->blockSignals(false);
			m_lblRegimeI->setStyleSheet("");
		}
	}
	else if (control == m_spbCorrIref)
	{
		if ((m_edited & CorrectionIrefFlag) != 0)
		{
			m_edited ^= CorrectionIrefFlag;
			m_spbCorrIref->blockSignals(true);
			m_spbCorrIref->setValue(m_saveCorrIref);
			m_spbCorrIref->blockSignals(false);
			m_lblCorrIref->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrDacI)
	{
		if ((m_edited & CalibrationDacIFlag) != 0)
		{
			m_edited ^= CalibrationDacIFlag;
			m_spbCalibrDacI->blockSignals(true);
			m_spbCalibrDacI->setValue(m_saveCalibrDacI);
			m_spbCalibrDacI->blockSignals(false);
			m_lblCalibrDacI->setStyleSheet("");
		}
	}
	else if (control == m_spbCalibrAdcI)
	{
		if ((m_edited & CalibrationAdcIFlag) != 0)
		{
			m_edited ^= CalibrationAdcIFlag;
			m_spbCalibrAdcI->blockSignals(true);
			m_spbCalibrAdcI->setValue(m_saveCalibrAdcI);
			m_spbCalibrAdcI->blockSignals(false);
			m_lblCalibrAdcI->setStyleSheet("");
		}
	}
	else if (control == m_spbTemperatureFanOn)
	{
		if ((m_edited & FanOnTFlag) != 0)
		{
			m_edited ^= FanOnTFlag;
			m_spbTemperatureFanOn->blockSignals(true);
			m_spbTemperatureFanOn->setValue(m_saveTemperatureFanOn);
			m_spbTemperatureFanOn->blockSignals(false);
			m_lblTemperatureFanOn->setStyleSheet("");
		}
	}
	else if (control == m_spbTemperaturePowerOff)
	{
		if ((m_edited & PowerOffTFlag) != 0)
		{
			m_edited ^= PowerOffTFlag;
			m_spbTemperaturePowerOff->blockSignals(true);
			m_spbTemperaturePowerOff->setValue(m_saveTemperaturePowerOff);
			m_spbTemperaturePowerOff->blockSignals(false);
			m_lblTemperaturePowerOff->setStyleSheet("");
		}
	}
	if (m_edited == NoCalibrationParameterFlag)
	{
		m_btnApply->setEnabled(false);
		m_btnCancel->setEnabled(false);
	}
}

void CalibrationWidget::setEnableControls(bool enabled)
{
	if (enabled)
	{
		m_btnApply->setEnabled(m_edited != NoCalibrationParameterFlag);
	}
	else
	{
		m_edited = NoCalibrationParameterFlag;
		m_btnApply->setEnabled(false);
		m_spbRegimeU->clear();
		m_spbCorrUref->clear();
		m_spbCalibrDacU->clear();
		m_spbCalibrAdcU->clear();
		m_spbRegimeI->clear();
		m_spbCorrIref->clear();
		m_spbCalibrDacI->clear();
		m_spbCalibrAdcI->clear();
		m_spbTemperatureFanOn->clear();
		m_spbTemperaturePowerOff->clear();
	}
	m_btnCancel->setEnabled(m_btnApply->isEnabled());

	m_chbAutocorrDac->setEnabled(enabled);
	m_spbRegimeU->setEnabled(enabled);
	m_spbCorrUref->setEnabled(enabled);
	m_spbCalibrDacU->setEnabled(enabled);
	m_spbCalibrAdcU->setEnabled(enabled);
	m_spbRegimeI->setEnabled(enabled);
	m_spbCorrIref->setEnabled(enabled);
	m_spbCalibrDacI->setEnabled(enabled);
	m_spbCalibrAdcI->setEnabled(enabled);
	m_spbTemperatureFanOn->setEnabled(enabled);
	m_spbTemperaturePowerOff->setEnabled(enabled);
}

void CalibrationWidget::updateProtocol()
{
	m_chbAutocorrDac->setVisible(Resources::protocol.ver >= PROTOCOL_VER_4_1);
}

bool CalibrationWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke != nullptr)
		{
			if ((ke->key() == Qt::Key_Return) || (ke->key() == Qt::Key_Enter))
			{
				QWidget *control = qobject_cast<QWidget *>(obj);
				if (control != nullptr) onControlReturnPressed(control);
				return true;
			}
			else if (ke->key() == Qt::Key_Escape)
			{
				QWidget *control = qobject_cast<QWidget *>(obj);
				if (control != nullptr) onControlEscapePressed(control);
				return true;
			}
		}
	}
	if (event->type() == QEvent::FocusIn)
	{
		if (Resources::focusSync)
		{
			QWidget *control = qobject_cast<QWidget *>(obj);
			if (control != nullptr)
			{
				if (control == m_spbRegimeU)
				{
					emit parameterSelected(4);
					return false;
				}
				else if (control == m_spbCorrUref)
				{
					emit parameterSelected(5);
					return false;
				}
				else if (control == m_spbCalibrDacU)
				{
					emit parameterSelected(7);
					return false;
				}
				else if (control == m_spbCalibrAdcU)
				{
					emit parameterSelected(6);
					return false;
				}
				else if (control == m_spbRegimeI)
				{
					emit parameterSelected(8);
					return false;
				}
				else if (control == m_spbCorrIref)
				{
					emit parameterSelected(9);
					return false;
				}
				else if (control == m_spbCalibrDacI)
				{
					emit parameterSelected(11);
					return false;
				}
				else if (control == m_spbCalibrAdcI)
				{
					emit parameterSelected(10);
					return false;
				}
				else if (control == m_spbTemperatureFanOn)
				{
					emit parameterSelected(12);
					return false;
				}
				else if (control == m_spbTemperaturePowerOff)
				{
					emit parameterSelected(13);
					return false;
				}
			}
		}
	}
	return QWidget::eventFilter(obj, event);
}

void CalibrationWidget::onRegimeUChanged(quint16 value)
{
	m_saveRegimeU = value;
	if ((m_edited & RegimeUFlag) == 0)
	{
		m_spbRegimeU->blockSignals(true);
		m_spbRegimeU->setValue(value);
		m_spbRegimeU->blockSignals(false);
	}
}

void CalibrationWidget::onRegimeIChanged(quint16 value)
{
	m_saveRegimeI = value;
	if ((m_edited & RegimeIFlag) == 0)
	{
		m_spbRegimeI->blockSignals(true);
		m_spbRegimeI->setValue(value);
		m_spbRegimeI->blockSignals(false);
	}
}

void CalibrationWidget::onTaskUCorrectionChanged(double value)
{
	m_saveCorrUref = value;
	if ((m_edited & CorrectionUrefFlag) == 0)
	{
		m_spbCorrUref->blockSignals(true);
		m_spbCorrUref->setValue(value);
		m_spbCorrUref->blockSignals(false);
	}
}

void CalibrationWidget::onCalibrationAdcUChanged(double value)
{
	m_saveCalibrAdcU = value;
	if ((m_edited & CalibrationAdcUFlag) == 0)
	{
		m_spbCalibrAdcU->blockSignals(true);
		m_spbCalibrAdcU->setValue(value);
		m_spbCalibrAdcU->blockSignals(false);
	}
}

void CalibrationWidget::onCalibrationDacUChanged(int value)
{
	m_saveCalibrDacU = value;
	if ((m_edited & CalibrationDacUFlag) == 0)
	{
		m_spbCalibrDacU->blockSignals(true);
		m_spbCalibrDacU->setValue(value);
		m_spbCalibrDacU->blockSignals(false);
	}
}

void CalibrationWidget::onTaskICorrectionChanged(double value)
{
	m_saveCorrIref = value;
	if ((m_edited & CorrectionIrefFlag) == 0)
	{
		m_spbCorrIref->blockSignals(true);
		m_spbCorrIref->setValue(value);
		m_spbCorrIref->blockSignals(false);
	}
}

void CalibrationWidget::onCalibrationAdcIChanged(double value)
{
	m_saveCalibrAdcI = value;
	if ((m_edited & CalibrationAdcIFlag) == 0)
	{
		m_spbCalibrAdcI->blockSignals(true);
		m_spbCalibrAdcI->setValue(value);
		m_spbCalibrAdcI->blockSignals(false);
	}
}

void CalibrationWidget::onCalibrationDacIChanged(int value)
{
	m_saveCalibrDacI = value;
	if ((m_edited & CalibrationDacIFlag) == 0)
	{
		m_spbCalibrDacI->blockSignals(true);
		m_spbCalibrDacI->setValue(value);
		m_spbCalibrDacI->blockSignals(false);
	}
}

void CalibrationWidget::onFanOnTemperatureChanged(int value)
{
	m_saveTemperatureFanOn = value;
	if ((m_edited & FanOnTFlag) == 0)
	{
		m_spbTemperatureFanOn->blockSignals(true);
		m_spbTemperatureFanOn->setValue(value);
		m_spbTemperatureFanOn->blockSignals(false);
	}
}

void CalibrationWidget::onPowerOffTemperatureChanged(int value)
{
	m_saveTemperaturePowerOff = value;
	if ((m_edited & PowerOffTFlag) == 0)
	{
		m_spbTemperaturePowerOff->blockSignals(true);
		m_spbTemperaturePowerOff->setValue(value);
		m_spbTemperaturePowerOff->blockSignals(false);
	}
}

void CalibrationWidget::onAutocorrectionDacChanged(bool value)
{
	/*m_saveAutocorrDac = value;
	if ((m_edited & DACAutocorrectionFlag) == 0)
	{
		m_chbAutocorrDac->blockSignals(true);
		m_chbAutocorrDac->setChecked(value);
		m_chbAutocorrDac->blockSignals(false);
	}*/
	m_chbAutocorrDac->blockSignals(true);
	m_chbAutocorrDac->setChecked(value);
	m_chbAutocorrDac->blockSignals(false);
}
