#ifndef OSCILLOSCOPE_H
#define OSCILLOSCOPE_H

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_legend.h>
#include <QVector>
#include <QDateTime>

#include "Resources.h"

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;
class QwtPlotGrid;
class QwtPlotCurve;

class OsclCanvas: public QwtPlotCanvas
{
public:
	OsclCanvas(QwtPlot *plot = nullptr);

public:
	void setupPalette();
};

class ElapsedTimeScaleDraw: public QwtScaleDraw
{
public:
	ElapsedTimeScaleDraw();
	virtual QwtText label(double value) const;
};

class Oscilloscope : public QwtPlot
{
	Q_OBJECT
public:
	explicit Oscilloscope(OscilloscopeRayList *rays, const OscilloscopeHandle &handle, QWidget *parent = nullptr);
	void setDepth(qint64 msec);
	void setYScale(double min, double max, int grid);
	quint8 rayFlags();
	void toggleRayState(OscilloscopeRay flag, bool state);
	double minY();
	double maxY();
	int gridNum();
	void updateRayColors();
	void updateBackgroundColor();

private:
	OscilloscopeRayList *m_rays;
	QVector<QwtPlotCurve*> m_curves;
	QVector<OscilloscopeRay> m_index;
	OsclCanvas *m_oslCanvas;
	qint64 m_depth;
	double m_step;
	quint8 m_rayFlags;
	double m_min;
	double m_max;
	int m_grid;
	void addCurve(OscilloscopeRay flag);

public slots:
	void updateCurves(qint64 zero);

signals:
	void rayFlagsChanged();
};

#endif // OSCILLOSCOPE_H
