#ifndef OSCILLOSCOPECOLORSDIALOG_H
#define OSCILLOSCOPECOLORSDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>

#include "Resources.h"

class OscilloscopeColorsDialog : public QDialog
{
	Q_OBJECT
public:
	explicit OscilloscopeColorsDialog(QWidget *parent = nullptr);

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	QString m_markStyle;
	OscilloscopeRayColors m_colors;
	QColor m_clrBackground;
	QLabel *m_lblBackground;
	QLabel *m_lblVoltage;
	QLabel *m_lblVoltageTask;
	QLabel *m_lblCurrent;
	QLabel *m_lblCurrentTask;
	QLabel *m_lblTemperatureT;
	QLabel *m_lblTemperatureD;
	QPushButton *m_btnBackground;
	QPushButton *m_btnVoltage;
	QPushButton *m_btnVoltageTask;
	QPushButton *m_btnCurrent;
	QPushButton *m_btnCurrentTask;
	QPushButton *m_btnTemperatureT;
	QPushButton *m_btnTemperatureD;
	QPushButton *m_btnClose;
	QPushButton *m_btnApply;
	QPushButton *m_btnOk;

private slots:
	void showColorDialog();
	void applyColors();
	void acceptColors();

signals:
	void dialogClosed();
	void colorsChanged();
};

#endif // OSCILLOSCOPECOLORSDIALOG_H
