#include <QHBoxLayout>
#include <QVBoxLayout>

#include "OscilloscopeColorsDialog.h"

OscilloscopeColorsDialog::OscilloscopeColorsDialog(QWidget *parent) : QDialog(parent)
{
	m_colors = Resources::oscilloscope.colors;
	m_clrBackground = Resources::oscilloscope.background;
	QVBoxLayout *vbl_main = new QVBoxLayout();
	QHBoxLayout *hbl;

	QSize icnSize(100, 30);
	m_markStyle = QString("background-color: %1; border:2px solid black;");

	hbl = new QHBoxLayout();
	m_btnBackground = new QPushButton(tr("Window background"), this);
	hbl->addWidget(m_btnBackground);
	m_lblBackground = new QLabel(this);
	m_lblBackground->setStyleSheet(m_markStyle.arg(m_clrBackground.name()));
	m_lblBackground->setMinimumSize(icnSize);
	m_lblBackground->setMaximumSize(icnSize);
	hbl->addWidget(m_lblBackground);
	vbl_main->addLayout(hbl);

	QLabel *lbl = new QLabel(this);
	lbl->setStyleSheet("background-color: black; border:1px solid black;");
	lbl->setMaximumHeight(3);
	vbl_main->addWidget(lbl);

	hbl = new QHBoxLayout();
	m_btnVoltage = new QPushButton(Resources::rayNames[OscilloscopeVoltage], this);
	hbl->addWidget(m_btnVoltage);
	m_lblVoltage = new QLabel(this);
	m_lblVoltage->setStyleSheet(m_markStyle.arg(m_colors[OscilloscopeVoltage].name()));
	m_lblVoltage->setMinimumSize(icnSize);
	m_lblVoltage->setMaximumSize(icnSize);
	hbl->addWidget(m_lblVoltage);
	vbl_main->addLayout(hbl);

	hbl = new QHBoxLayout();
	m_btnVoltageTask = new QPushButton(Resources::rayNames[OscilloscopeVoltageTask], this);
	hbl->addWidget(m_btnVoltageTask);
	m_lblVoltageTask = new QLabel(this);
	m_lblVoltageTask->setStyleSheet(m_markStyle.arg(m_colors[OscilloscopeVoltageTask].name()));
	m_lblVoltageTask->setMinimumSize(icnSize);
	m_lblVoltageTask->setMaximumSize(icnSize);
	hbl->addWidget(m_lblVoltageTask);
	vbl_main->addLayout(hbl);

	hbl = new QHBoxLayout();
	m_btnCurrent = new QPushButton(Resources::rayNames[OscilloscopeCurrent], this);
	hbl->addWidget(m_btnCurrent);
	m_lblCurrent = new QLabel(this);
	m_lblCurrent->setStyleSheet(m_markStyle.arg(m_colors[OscilloscopeCurrent].name()));
	m_lblCurrent->setMinimumSize(icnSize);
	m_lblCurrent->setMaximumSize(icnSize);
	hbl->addWidget(m_lblCurrent);
	vbl_main->addLayout(hbl);

	hbl = new QHBoxLayout();
	m_btnCurrentTask = new QPushButton(Resources::rayNames[OscilloscopeCurrentTask], this);
	hbl->addWidget(m_btnCurrentTask);
	m_lblCurrentTask = new QLabel(this);
	m_lblCurrentTask->setStyleSheet(m_markStyle.arg(m_colors[OscilloscopeCurrentTask].name()));
	m_lblCurrentTask->setMinimumSize(icnSize);
	m_lblCurrentTask->setMaximumSize(icnSize);
	hbl->addWidget(m_lblCurrentTask);
	vbl_main->addLayout(hbl);

	hbl = new QHBoxLayout();
	m_btnTemperatureT = new QPushButton(Resources::rayNames[OscilloscopeTemperatureTransistor], this);
	hbl->addWidget(m_btnTemperatureT);
	m_lblTemperatureT = new QLabel(this);
	m_lblTemperatureT->setStyleSheet(m_markStyle.arg(m_colors[OscilloscopeTemperatureTransistor].name()));
	m_lblTemperatureT->setMinimumSize(icnSize);
	m_lblTemperatureT->setMaximumSize(icnSize);
	hbl->addWidget(m_lblTemperatureT);
	vbl_main->addLayout(hbl);

	hbl = new QHBoxLayout();
	m_btnTemperatureD = new QPushButton(Resources::rayNames[OscilloscopeTemperatureDiode], this);
	hbl->addWidget(m_btnTemperatureD);
	m_lblTemperatureD = new QLabel(this);
	m_lblTemperatureD->setStyleSheet(m_markStyle.arg(m_colors[OscilloscopeTemperatureDiode].name()));
	m_lblTemperatureD->setMinimumSize(icnSize);
	m_lblTemperatureD->setMaximumSize(icnSize);
	hbl->addWidget(m_lblTemperatureD);
	vbl_main->addLayout(hbl);

	vbl_main->addStretch();

	hbl = new QHBoxLayout();
	hbl->addStretch();
	m_btnOk = new QPushButton(tr("Ok"), this);
	hbl->addWidget(m_btnOk);
	m_btnClose = new QPushButton(tr("Cancel"), this);
	hbl->addWidget(m_btnClose);
	m_btnApply = new QPushButton(tr("Apply"), this);
	hbl->addWidget(m_btnApply);
	vbl_main->addLayout(hbl);

	setLayout(vbl_main);
	resize(450, 300);

	connect(m_btnBackground, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnVoltage, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnVoltageTask, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnCurrent, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnCurrentTask, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnTemperatureT, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnTemperatureD, SIGNAL(clicked(bool)), this, SLOT(showColorDialog()));
	connect(m_btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(m_btnApply, SIGNAL(clicked(bool)), this, SLOT(applyColors()));
	connect(m_btnOk, SIGNAL(clicked(bool)), this, SLOT(acceptColors()));
}

void OscilloscopeColorsDialog::closeEvent(QCloseEvent *event)
{
	emit dialogClosed();
	QDialog::closeEvent(event);
}

void OscilloscopeColorsDialog::showColorDialog()
{
	QPushButton *btn = qobject_cast<QPushButton*>(sender());
	if (btn != nullptr)
	{
		int id = -1;
		QColor color;
		if (btn == m_btnBackground)
		{
			color = m_clrBackground;
			id = 0;
		}
		else if (btn == m_btnVoltage)
		{
			color = m_colors[OscilloscopeVoltage];
			id = 1;
		}
		else if (btn == m_btnVoltageTask)
		{
			color = m_colors[OscilloscopeVoltageTask];
			id = 2;
		}
		else if (btn == m_btnCurrent)
		{
			color = m_colors[OscilloscopeCurrent];
			id = 3;
		}
		else if (btn == m_btnCurrentTask)
		{
			color = m_colors[OscilloscopeCurrentTask];
			id = 4;
		}
		else if (btn == m_btnTemperatureT)
		{
			color = m_colors[OscilloscopeTemperatureTransistor];
			id = 5;
		}
		else if (btn == m_btnTemperatureD)
		{
			color = m_colors[OscilloscopeTemperatureDiode];
			id = 6;
		}
		QColorDialog dlg(color, this);
		if (dlg.exec() == QDialog::Accepted)
		{
			color = dlg.selectedColor();
			switch (id)
			{
				case 0:
					m_clrBackground = color;
					m_lblBackground->setStyleSheet(m_markStyle.arg(color.name()));
					break;
				case 1:
					m_colors[OscilloscopeVoltage] = color;
					m_lblVoltage->setStyleSheet(m_markStyle.arg(color.name()));
					break;
				case 2:
					m_colors[OscilloscopeVoltageTask] = color;
					m_lblVoltageTask->setStyleSheet(m_markStyle.arg(color.name()));
					break;
				case 3:
					m_colors[OscilloscopeCurrent] = color;
					m_lblCurrent->setStyleSheet(m_markStyle.arg(color.name()));
					break;
				case 4:
					m_colors[OscilloscopeCurrentTask] = color;
					m_lblCurrentTask->setStyleSheet(m_markStyle.arg(color.name()));
					break;
				case 5:
					m_colors[OscilloscopeTemperatureTransistor] = color;
					m_lblTemperatureT->setStyleSheet(m_markStyle.arg(color.name()));
					break;
				case 6:
					m_colors[OscilloscopeTemperatureDiode] = color;
					m_lblTemperatureD->setStyleSheet(m_markStyle.arg(color.name()));
					break;
			}
		}
	}
}

void OscilloscopeColorsDialog::applyColors()
{
	Resources::oscilloscope.background = m_clrBackground;
	Resources::oscilloscope.colors = m_colors;
	emit colorsChanged();
}

void OscilloscopeColorsDialog::acceptColors()
{
	applyColors();
	close();
}
