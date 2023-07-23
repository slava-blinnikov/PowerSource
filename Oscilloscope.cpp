#include "Oscilloscope.h"

Oscilloscope::Oscilloscope(OscilloscopeRayList *rays, const OscilloscopeHandle &handle, QWidget *parent) : QwtPlot(parent)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	m_rays = rays;
	m_rayFlags = handle.rays;

	setAutoReplot(false);
	m_oslCanvas = new OsclCanvas();
	m_oslCanvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
	m_oslCanvas->setFocusPolicy(Qt::StrongFocus);
	m_oslCanvas->setCursor(Qt::ArrowCursor);
	setCanvas(m_oslCanvas);

	plotLayout()->setAlignCanvasToScales(true);

	enableAxis(QwtPlot::yRight);
	setDepth(Resources::oscilloscope.depth);
	m_min = handle.y_min;
	m_max = handle.y_max;
	m_grid = handle.y_grid;
	setYScale(handle.y_min, handle.y_max, handle.y_grid);

	QwtPlotGrid *grid = new QwtPlotGrid();
	grid->setPen(Qt::black, 1, Qt::DotLine);
	grid->enableX(true);
	grid->enableXMin(false);
	grid->enableY(true);
	grid->enableYMin(false);
	grid->attach(this);

	if (m_rayFlags & OscilloscopeVoltage) addCurve(OscilloscopeVoltage);
	if (m_rayFlags & OscilloscopeVoltageTask) addCurve(OscilloscopeVoltageTask);
	if (m_rayFlags & OscilloscopeCurrent) addCurve(OscilloscopeCurrent);
	if (m_rayFlags & OscilloscopeCurrentTask) addCurve(OscilloscopeCurrentTask);
	if (m_rayFlags & OscilloscopeTemperatureTransistor) addCurve(OscilloscopeTemperatureTransistor);
	if (m_rayFlags & OscilloscopeTemperatureDiode) addCurve(OscilloscopeTemperatureDiode);
	QwtLegend *legend = new QwtLegend();
	insertLegend(legend, QwtPlot::LegendPosition(TopLegend));

	setAxisScaleDraw(QwtPlot::xBottom, new ElapsedTimeScaleDraw());
}

void Oscilloscope::setDepth(qint64 msec)
{
	m_depth = msec;
	m_step = double(m_depth) / Resources::oscilloscope.x_grid;
	setAxisScale(QwtPlot::xBottom, -1.0 * m_depth, 0.0, m_step);
}

void Oscilloscope::setYScale(double min, double max, int grid)
{
	m_min = min;
	m_max = max;
	m_grid = grid;
	setAxisScale(QwtPlot::yLeft, min, max, qAbs(max - min) / grid);
	setAxisScale(QwtPlot::yRight, min, max, qAbs(max - min) / grid);
}

quint8 Oscilloscope::rayFlags()
{
	return m_rayFlags;
}

void Oscilloscope::toggleRayState(OscilloscopeRay flag, bool state)
{
	int idx = m_index.indexOf(flag);
	if (state)
	{
		if (idx < 0)
		{
			addCurve(flag);
			replot();
			legend()->show();
		}
		m_rayFlags |= flag;
	}
	else
	{
		if (idx >= 0)
		{
			m_index.remove(idx);
			QwtPlotCurve *curve = m_curves.at(idx);
			m_curves.remove(idx);
			curve->detach();
			delete curve;
			replot();
		}
		m_rayFlags &= ~flag;
	}
	emit rayFlagsChanged();
}

double Oscilloscope::minY()
{
	return m_min;
}

double Oscilloscope::maxY()
{
	return m_max;
}

int Oscilloscope::gridNum()
{
	return m_grid;
}

void Oscilloscope::updateRayColors()
{
	int n = m_index.size();
	if (n > 0)
	{
		OscilloscopeRay flag;
		for (int i = 0; i < n; i++)
		{
			flag = m_index.at(i);
			m_curves[i]->setPen(Resources::oscilloscope.colors.value(flag), 2.0, Qt::SolidLine);
		}
		replot();
	}
}

void Oscilloscope::updateBackgroundColor()
{
	m_oslCanvas->setupPalette();
	replot();
}

void Oscilloscope::addCurve(OscilloscopeRay flag)
{
	QwtPlotCurve *curve;
	curve = new QwtPlotCurve(Resources::rayNames[flag]);
	curve->setPen(Resources::oscilloscope.colors.value(flag), 2.0, Qt::SolidLine);
	curve->setLegendIconSize(QSize(20, 15));
	curve->setStyle(QwtPlotCurve::Steps);
	m_index.append(flag);
	m_curves.append(curve);
	curve->attach(this);
}

void Oscilloscope::updateCurves(qint64 zero)
{
	QwtPlotCurve *crv;
	OscilloscopeRay rayType;
	int n = m_index.size();
	for (int i = 0; i < n; i++)
	{
		rayType = m_index.at(i);
		crv = m_curves.at(i);
		const QPolygonF &line = m_rays->value(rayType);
		QPolygonF tline;
		int n = line.size();
		for (int l = 0; l < n; l++)
		{
			const QPointF &p = line.at(l);
			tline.append(QPointF(p.x() - zero, p.y()));
		}
		crv->setSamples(tline);
	}
	replot();
}

OsclCanvas::OsclCanvas(QwtPlot *plot): QwtPlotCanvas(plot)
{
	setPaintAttribute(QwtPlotCanvas::BackingStore, false);
	setBorderRadius(8);
	if (QwtPainter::isX11GraphicsSystem())
	{
#if QT_VERSION < 0x050000
		setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif
		if (testPaintAttribute(QwtPlotCanvas::BackingStore))
		{
			setAttribute(Qt::WA_PaintOnScreen, true);
			setAttribute(Qt::WA_NoSystemBackground, true);
		}
	}
	setupPalette();
}

void OsclCanvas::setupPalette()
{
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QBrush(Resources::oscilloscope.background));
	pal.setColor(QPalette::WindowText, Qt::green);
	setPalette(pal);
}

ElapsedTimeScaleDraw::ElapsedTimeScaleDraw()
{
}

QwtText ElapsedTimeScaleDraw::label(double value) const
{
	qint64 t = -1 * qint64(value);
	int h = int(t / 3600000);
	t = t % 3600000;
	int min = int(t / 60000);
	t = t % 60000;
	int sec = int(t / 1000);
	QString s;
	QString sm = QString::number(min);
	if (min < 10) sm.prepend('0');
	QString st = QString::number(sec);
	if (sec < 10) st.prepend('0');
	s = QString("%1:%2").arg(sm).arg(st);
	if (h > 0)
	{
		QString sh = QString::number(h);
		if (h < 10) sh.prepend('0');
		s = QString("%1:%2").arg(sh).arg(s);
	}
	return s;
}
