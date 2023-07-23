#include <QDateTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include "PlotDialog.h"

PlotDialog::PlotDialog(QWidget *parent) : QDialog(parent)
{
	//setWindowTitle(tr("Power supply [protocol v-%1.%2]").arg(Resources::protocol.arr[1]).arg(Resources::protocol.arr[0]));
	m_dlgColours = nullptr;
	m_changed = false;
	m_depth = Resources::oscilloscope.depth;
	m_rays.clear();
	m_rayFlags = Resources::oscilloscope.rays;
	if (m_rayFlags & OscilloscopeVoltage)
	{
		m_rays[OscilloscopeVoltage] = OscilloscopeRayData();
		if (Resources::lastValues.contains(OscilloscopeVoltage))
		{
			OscilloscopeValue lv = Resources::lastValues[OscilloscopeVoltage];
			m_rays[OscilloscopeVoltage].append(lv);
			lv.setX(QDateTime::currentMSecsSinceEpoch());
			m_rays[OscilloscopeVoltage].append(lv);
		}
	}
	if (m_rayFlags & OscilloscopeVoltageTask)
	{
		m_rays[OscilloscopeVoltageTask] = OscilloscopeRayData();
		if (Resources::lastValues.contains(OscilloscopeVoltageTask))
		{
			OscilloscopeValue lv = Resources::lastValues[OscilloscopeVoltageTask];
			m_rays[OscilloscopeVoltageTask].append(lv);
			lv.setX(QDateTime::currentMSecsSinceEpoch());
			m_rays[OscilloscopeVoltageTask].append(lv);
		}
	}
	if (m_rayFlags & OscilloscopeCurrent)
	{
		m_rays[OscilloscopeCurrent] = OscilloscopeRayData();
		if (Resources::lastValues.contains(OscilloscopeCurrent))
		{
			OscilloscopeValue lv = Resources::lastValues[OscilloscopeCurrent];
			m_rays[OscilloscopeCurrent].append(lv);
			lv.setX(QDateTime::currentMSecsSinceEpoch());
			m_rays[OscilloscopeCurrent].append(lv);
		}
	}
	if (m_rayFlags & OscilloscopeCurrentTask)
	{
		m_rays[OscilloscopeCurrentTask] = OscilloscopeRayData();
		if (Resources::lastValues.contains(OscilloscopeCurrentTask))
		{
			OscilloscopeValue lv = Resources::lastValues[OscilloscopeCurrentTask];
			m_rays[OscilloscopeCurrentTask].append(lv);
			lv.setX(QDateTime::currentMSecsSinceEpoch());
			m_rays[OscilloscopeCurrentTask].append(lv);
		}
	}
	if (m_rayFlags & OscilloscopeTemperatureTransistor)
	{
		m_rays[OscilloscopeTemperatureTransistor] = OscilloscopeRayData();
		if (Resources::lastValues.contains(OscilloscopeTemperatureTransistor))
		{
			OscilloscopeValue lv = Resources::lastValues[OscilloscopeTemperatureTransistor];
			m_rays[OscilloscopeTemperatureTransistor].append(lv);
			lv.setX(QDateTime::currentMSecsSinceEpoch());
			m_rays[OscilloscopeTemperatureTransistor].append(lv);
		}
	}
	if (m_rayFlags & OscilloscopeTemperatureDiode)
	{
		m_rays[OscilloscopeTemperatureDiode] = OscilloscopeRayData();
		if (Resources::lastValues.contains(OscilloscopeTemperatureDiode))
		{
			OscilloscopeValue lv = Resources::lastValues[OscilloscopeTemperatureDiode];
			m_rays[OscilloscopeTemperatureDiode].append(lv);
			lv.setX(QDateTime::currentMSecsSinceEpoch());
			m_rays[OscilloscopeTemperatureDiode].append(lv);
		}
	}
	QVBoxLayout *vblMain = new QVBoxLayout();
	vblMain->setContentsMargins(0, 0, 0, 0);
	vblMain->setMargin(0);
	QHBoxLayout *hblTools = new QHBoxLayout();
	hblTools->setContentsMargins(0, 0, 0, 0);
	hblTools->setMargin(0);
	QSize icnSize(32, 32);
	m_btnAddOscilloscope = new QPushButton(QIcon(":icons/append.png"), QString(), this);
	m_btnAddOscilloscope->setToolTip(tr("Add new oscilloscope window."));
	m_btnAddOscilloscope->setIconSize(icnSize);
	hblTools->addWidget(m_btnAddOscilloscope);
	QLabel *lbl;
	qint64 d = Resources::oscilloscope.depth / 1000;
	QGroupBox *grb = new QGroupBox(tr("Depth"), this);
	QHBoxLayout *hbl = new QHBoxLayout();
	hbl->setContentsMargins(3, 0, 3, 0);
	lbl = new QLabel(tr("grid:"), grb);
	hbl->addWidget(lbl);
	m_spbGrid = new QSpinBox(grb);
	m_spbGrid->setToolTip(tr("Horizontal grid lines count."));
	m_spbGrid->installEventFilter(this);
	m_spbGrid->setRange(2, 1000);
	m_spbGrid->setValue(Resources::oscilloscope.x_grid);
	hbl->addWidget(m_spbGrid);
	lbl = new QLabel(tr("min.:"), grb);
	hbl->addWidget(lbl);
	m_spbMinutes = new QSpinBox(grb);
	m_spbMinutes->setToolTip(tr("Timeline depth (minutes)."));
	m_spbMinutes->installEventFilter(this);
	m_spbMinutes->setRange(0, 1000);
	m_spbMinutes->setValue(int(d / 60));
	hbl->addWidget(m_spbMinutes);
	lbl = new QLabel(tr("sec.:"), grb);
	hbl->addWidget(lbl);
	m_spbSeconds = new QSpinBox(grb);
	m_spbSeconds->setToolTip(tr("Timeline depth (seconds)."));
	m_spbSeconds->installEventFilter(this);
	m_spbSeconds->setRange(0, 1000);
	m_spbSeconds->setValue(int(d % 60));
	hbl->addWidget(m_spbSeconds);
	m_btnApplyDepth = new QPushButton(QIcon(":icons/checked.png"), QString(), grb);
	m_btnApplyDepth->setToolTip(tr("Apply horizontal grid and timeline depth."));
	m_btnApplyDepth->setFlat(true);
	m_btnApplyDepth->setEnabled(false);
	m_btnApplyDepth->setIconSize(QSize(24, 24));
	hbl->addWidget(m_btnApplyDepth);
	grb->setLayout(hbl);
	hblTools->addWidget(grb);
	hblTools->addStretch();
	m_btnColors = new QPushButton(QIcon(":icons/rgb.png"), QString(), this);
	m_btnColors->setToolTip(tr("Open oscilloscope colors dialog."));
	m_btnColors->setIconSize(icnSize);
	hblTools->addWidget(m_btnColors);
	vblMain->addLayout(hblTools);
	m_vblPlots = new QVBoxLayout();
	vblMain->addLayout(m_vblPlots);
	QStatusBar *statusBar = new QStatusBar();
	vblMain->addWidget(statusBar);
	setLayout(vblMain);
	int n = Resources::oscilloscope.handles.size();
	for (int i = 0; i < n; i++)
	{
		const OscilloscopeHandle &handle = Resources::oscilloscope.handles.at(i);
		OscilloscopeWidget *oslw = new OscilloscopeWidget(&m_rays, handle, this);
		m_vblPlots->addWidget(oslw);
		Oscilloscope *osl = oslw->oscilloscope();
		m_plots.append(osl);
		connect(oslw, SIGNAL(closeButtonClicked(QWidget*)), this, SLOT(closeOscilloscopeWidget(QWidget *)));
		connect(oslw, SIGNAL(oscilloscopeChanged(QWidget*)), this, SLOT(onOscilloscopeChanged(QWidget *)));
		connect(osl, SIGNAL(rayFlagsChanged()), this, SLOT(onRayFlagsChanged()));
	}
	QList<QScreen*> screens = QGuiApplication::screens();
	QScreen *scr;
	int sh;
	int h = -1;
	for (int i = 0; i < screens.size(); i++)
	{
		scr = screens.at(i);
		sh = scr->size().height();
		if (h >= 0)
		{
			if (h > sh) h = sh;
		}
		else h = sh;
	}
	QRect r = Resources::oscilloscope.windowRect;
	if (h >= 0)
	{
		if (r.y() > h) r.setY(20);
	}
	setGeometry(r);
#ifdef PWRSRC_TEST_SIGNAL
	connect(&test, SIGNAL(voltage(double)), this, SLOT(onVoltageChanged(double)));
	connect(&test, SIGNAL(current(double)), this, SLOT(onCurrentChanged(double)));
	connect(&test, SIGNAL(taskU(double)), this, SLOT(onTaskUChanged(double)));
	connect(&test, SIGNAL(taskI(double)), this, SLOT(onTaskIChanged(double)));
	connect(&test, SIGNAL(temperatureT(quint8)), this, SLOT(onTemperatureTChanged(quint8)));
	connect(&test, SIGNAL(temperatureD(quint8)), this, SLOT(onTemperatureDChanged(quint8)));
	test.setDepth(m_depth);
	test.start();
#endif
	connect(m_btnAddOscilloscope, SIGNAL(clicked(bool)), this, SLOT(addOscilloscope()));
	connect(m_btnApplyDepth, SIGNAL(clicked(bool)), this, SLOT(applyDepth()));
	connect(m_spbGrid, SIGNAL(valueChanged(int)), this, SLOT(onDepthEditChanged()));
	connect(m_spbMinutes, SIGNAL(valueChanged(int)), this, SLOT(onDepthEditChanged()));
	connect(m_spbSeconds, SIGNAL(valueChanged(int)), this, SLOT(onDepthEditChanged()));
	connect(m_btnColors, SIGNAL(clicked(bool)), this, SLOT(showColorsDialog()));

	m_timer_id = startTimer(Resources::oscilloscope.update_interval);
}

PlotDialog::~PlotDialog()
{
#ifdef PWRSRC_TEST_SIGNAL
	test.stop();
#endif
	if (m_timer_id != 0) killTimer(m_timer_id);
	m_rays.clear();
}

void PlotDialog::closeEvent(QCloseEvent *event)
{
	Resources::oscilloscope.windowRect = geometry();
	Resources::writeWindowState();
	if (m_changed) Resources::writeConfig();
	emit dialogClosed();
	QDialog::closeEvent(event);
}

void PlotDialog::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_timer_id) updatePlots();
	QDialog::timerEvent(event);
}

bool PlotDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		int key = keyEvent->key();
		if ((key == Qt::Key_Return) || (key == Qt::Key_Enter))
		{
			applyDepth();
			return true;
		}
	}
	return QDialog::eventFilter(obj, event);
}

void PlotDialog::truncToDepth(OscilloscopeRayData &data)
{
	qint64 ctime = QDateTime::currentMSecsSinceEpoch();
	qint64 stime = ctime - m_depth;
	int n = data.size();
	int pri = -1;
	for (int i = 0; i < n; i++)
	{
		const OscilloscopeValue &v = data.at(i);
		if (v.x() < stime) pri = i; else break;
	}
	if (pri >= 0)
	{
		data[pri].setX(stime);
		if (pri > 100) data.remove(0, pri);
	}
	/*{
		const OscilloscopeValue &v = data.at(pri);
		OscilloscopeValue nv(stime, v.y());
		data.remove(0, pri + 1);
		data.prepend(nv);
	}*/
	n = data.size();
	if (n > 0)
	{
		int i = n - 1;
		const QPointF &p = data.at(i);
		if (qint64(p.x()) < ctime) data[i].setX(ctime);
		/*{
			if (n > 1) data[i].setX(ctime); else data.append(QPointF(ctime, p.x()));
		}*/
	}
}

void PlotDialog::truncRays()
{
	QList<OscilloscopeRay> keys = m_rays.keys();
	for (int i = 0; i < keys.size(); i++) truncToDepth(m_rays[keys.at(i)]);
}

void PlotDialog::updatePlots()
{
	truncRays();
	qint64 ct = QDateTime::currentMSecsSinceEpoch();
	int n = m_plots.size();
	for (int i = 0; i < n; i++)
	{
		Oscilloscope *osl = m_plots.at(i);
		osl->updateCurves(ct);
	}
}

void PlotDialog::invalidateRayData(OscilloscopeRay flag)
{
	if (m_rayFlags & flag)
	{
		if (!m_rays.contains(flag))
		{
			m_rays[flag] = OscilloscopeRayData();
            if (Resources::lastValues.contains(flag))
            {
                OscilloscopeValue lv = Resources::lastValues[flag];
                m_rays[flag].append(lv);
                lv.setX(QDateTime::currentMSecsSinceEpoch());
                m_rays[flag].append(lv);
            }
		}
	}
	else
	{
		if (m_rays.contains(flag)) m_rays.remove(flag);
	}
}

void PlotDialog::updateColors()
{
	int n = m_plots.size();
	for (int i = 0; i < n; i++)
	{
		Oscilloscope *osl = m_plots.at(i);
		osl->updateBackgroundColor();
		osl->updateRayColors();
	}
}

void PlotDialog::invalidateRayFlags()
{
	quint8 flags = 0;
	int n = m_plots.size();
	for (int i = 0; i < n; i++)
	{
		Oscilloscope *osl = m_plots.at(i);
		flags |= osl->rayFlags();
	}
	m_rayFlags = flags;
	invalidateRayData(OscilloscopeVoltage);
	invalidateRayData(OscilloscopeVoltageTask);
	invalidateRayData(OscilloscopeCurrent);
	invalidateRayData(OscilloscopeCurrentTask);
	invalidateRayData(OscilloscopeTemperatureTransistor);
	invalidateRayData(OscilloscopeTemperatureDiode);
}

void PlotDialog::closeOscilloscopeWidget(QWidget *widget)
{
	OscilloscopeWidget *oslw = qobject_cast<OscilloscopeWidget *>(widget);
	if (oslw != nullptr)
	{
		Oscilloscope *osl = oslw->oscilloscope();
		int idx = m_plots.indexOf(osl);
		if (idx >= 0) m_plots.remove(idx);
		m_vblPlots->removeWidget(widget);
		Resources::oscilloscope.handles.remove(idx);
		delete oslw;
		invalidateRayFlags();
		m_changed = true;
	}
}

void PlotDialog::onOscilloscopeChanged(QWidget *oslw)
{
	Oscilloscope *osl = qobject_cast<Oscilloscope*>(oslw);
	int idx = m_plots.indexOf(osl);
	if (idx >= 0)
	{
		Resources::oscilloscope.handles[idx].rays = osl->rayFlags();
		Resources::oscilloscope.handles[idx].y_min = osl->minY();
		Resources::oscilloscope.handles[idx].y_max = osl->maxY();
		Resources::oscilloscope.handles[idx].y_grid = osl->gridNum();
		m_changed = true;
	}
}

void PlotDialog::onRayFlagsChanged()
{
	invalidateRayFlags();
	m_changed = true;
}

void PlotDialog::addOscilloscope()
{
	OscilloscopeHandle handle;
	handle.rays = 0;
	handle.y_min = 0;
	handle.y_max = 10;
	handle.y_grid = 10;
	OscilloscopeWidget *oslw = new OscilloscopeWidget(&m_rays, handle, this);
	m_vblPlots->addWidget(oslw);
	Oscilloscope *osl = oslw->oscilloscope();
	m_plots.append(osl);
	connect(oslw, SIGNAL(closeButtonClicked(QWidget*)), this, SLOT(closeOscilloscopeWidget(QWidget *)));
	connect(oslw, SIGNAL(oscilloscopeChanged(QWidget*)), this, SLOT(onOscilloscopeChanged(QWidget *)));
	connect(osl, SIGNAL(rayFlagsChanged()), this, SLOT(onRayFlagsChanged()));
	Resources::oscilloscope.handles.append(handle);
	m_changed = true;
}

void PlotDialog::applyDepth()
{
	Resources::oscilloscope.x_grid = m_spbGrid->value();
	qint64 d = ((m_spbMinutes->value() * 60) + m_spbSeconds->value()) * 1000;
	Resources::oscilloscope.depth = d;
	int n = m_plots.size();
	for (int i = 0; i < n; i++)
	{
		Oscilloscope *osl = m_plots.at(i);
		osl->setDepth(d);
		osl->replot();
	}
	m_depth = d;
	m_changed = true;
	m_btnApplyDepth->setEnabled(false);
}

void PlotDialog::onDepthEditChanged()
{
	m_btnApplyDepth->setEnabled(true);
}

void PlotDialog::showColorsDialog()
{
	if (m_dlgColours == nullptr)
	{
		m_dlgColours = new OscilloscopeColorsDialog(this);
		m_dlgColours->setWindowTitle(QString("%1 - %2").arg(windowTitle()).arg(tr("Colours")));
		connect(m_dlgColours, SIGNAL(dialogClosed()), m_dlgColours, SLOT(deleteLater()));
		connect(m_dlgColours, SIGNAL(dialogClosed()), this, SLOT(onColorsDialogClosed()));
		connect(m_dlgColours, SIGNAL(colorsChanged()), this, SLOT(onColorsChanged()));
		m_dlgColours->showNormal();
	}
}

void PlotDialog::onColorsDialogClosed()
{
	m_dlgColours = nullptr;
}

void PlotDialog::onColorsChanged()
{
	updateColors();
}

void PlotDialog::onVoltageChanged(double value)
{
	if (m_rayFlags & OscilloscopeVoltage)
	{
		m_rays[OscilloscopeVoltage].append(OscilloscopeValue(QDateTime::currentMSecsSinceEpoch(), value));
	}
}

void PlotDialog::onCurrentChanged(double value)
{
	if (m_rayFlags & OscilloscopeCurrent)
	{
		m_rays[OscilloscopeCurrent].append(OscilloscopeValue(QDateTime::currentMSecsSinceEpoch(), value));
	}
}

void PlotDialog::onTaskUChanged(double value)
{
	if (m_rayFlags & OscilloscopeVoltageTask)
	{
		m_rays[OscilloscopeVoltageTask].append(OscilloscopeValue(QDateTime::currentMSecsSinceEpoch(), value));
	}
}

void PlotDialog::onTaskIChanged(double value)
{
	if (m_rayFlags & OscilloscopeCurrentTask)
	{
		m_rays[OscilloscopeCurrentTask].append(OscilloscopeValue(QDateTime::currentMSecsSinceEpoch(), value));
	}
}

void PlotDialog::onTemperatureTChanged(quint8 t)
{
	if ((m_rayFlags & OscilloscopeTemperatureTransistor) && (t != 0))
	{
		m_rays[OscilloscopeTemperatureTransistor].append(OscilloscopeValue(QDateTime::currentMSecsSinceEpoch(), t));
	}
}

void PlotDialog::onTemperatureDChanged(quint8 t)
{
	if ((m_rayFlags & OscilloscopeTemperatureDiode) && (t != 0))
	{
		m_rays[OscilloscopeTemperatureDiode].append(OscilloscopeValue(QDateTime::currentMSecsSinceEpoch(), t));
	}
}

#ifdef PWRSRC_TEST_SIGNAL
TestSignalUnit::TestSignalUnit(QObject *parent) : QObject(parent)
{
	m_worker = nullptr;
	m_thread = nullptr;
}

void TestSignalUnit::setDepth(qint64 depth)
{
	m_depth = depth;
}

void TestSignalUnit::start()
{
	if (m_thread == nullptr)
	{
		m_worker = new TestSignalUnitWorker();
		m_worker->setDepth(m_depth);
		m_thread = new QThread();
		m_worker->moveToThread(m_thread);
		connect(m_thread, SIGNAL(started()), m_worker, SLOT(start()));
		connect(m_thread, SIGNAL(finished()), m_worker, SLOT(stop()));
		connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
		connect(m_thread, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
		connect(m_thread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
		connect(m_worker, SIGNAL(voltage(double)), this, SIGNAL(voltage(double)));
		connect(m_worker, SIGNAL(current(double)), this, SIGNAL(current(double)));
		connect(m_worker, SIGNAL(taskU(double)), this, SIGNAL(taskU(double)));
		connect(m_worker, SIGNAL(taskI(double)), this, SIGNAL(taskI(double)));
		connect(m_worker, SIGNAL(temperatureT(quint8)), this, SIGNAL(temperatureT(quint8)));
		connect(m_worker, SIGNAL(temperatureD(quint8)), this, SIGNAL(temperatureD(quint8)));
		m_thread->start();
	}
}

void TestSignalUnit::stop()
{
	if (m_thread != nullptr) m_thread->quit();
}

void TestSignalUnit::onThreadFinished()
{
	m_worker = nullptr;
	m_thread = nullptr;
}

TestSignalUnitWorker::TestSignalUnitWorker(QObject *parent) : QObject(parent)
{
	m_timer = nullptr;
}

void TestSignalUnitWorker::setDepth(qint64 depth)
{
	m_period = (2 * M_PI) / double(depth);
}

void TestSignalUnitWorker::start()
{
	if (m_timer == nullptr)
	{
		m_timer = new QTimer(this);
		m_timer->setInterval(100);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
		m_timer->start();
	}
}

void TestSignalUnitWorker::stop()
{
	if (m_timer != nullptr)
	{
		m_timer->stop();
		delete m_timer;
	}
}

void TestSignalUnitWorker::timeout()
{
	qint64 ct = QDateTime::currentMSecsSinceEpoch();

	emit taskU((qSin((ct * m_period) * 3) * 15.0) + 15.0);
	emit voltage((qSin(ct * m_period) * 15.0) + 15.0);

	emit taskI((qCos((ct * m_period) * 3) * 5.0) + 5.0);
	emit current((qCos(ct * m_period) * 5.0) + 5.0);

	emit temperatureT(quint8((qSin((ct * m_period) * 5) * 40.0) + 40.0));
	emit temperatureD(quint8((qCos((ct * m_period) * 5) * 40.0) + 40.0));
}
#endif
