#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include "Resources.h"

class OptionsDialog : public QDialog
{
	Q_OBJECT
public:
	OptionsDialog(QWidget *parent = nullptr);
	bool isPortChanged();
	bool isRequestIntervalChanged();

private:
	bool m_portChanged;
	bool m_requestIntervalChanged;
	QComboBox *m_cbPort;
	QComboBox *m_cbProtocol;
	QComboBox *m_cbBaudrate;
	QComboBox *m_cbDatabits;
	QComboBox *m_cbFlowcontrol;
	QComboBox *m_cbParity;
	QComboBox *m_cbStopbits;
	QSpinBox *m_spbIOTimeout;
	QSpinBox *m_spbRequestInterval;
	QSpinBox *m_spbApplyTimeout;
	QCheckBox *m_chbFocusSync;
	QCheckBox *m_chbPortAutosearch;
	QSpinBox *m_spbReconnectInterval;
	QCheckBox *m_chbVirtualKeyboard;
	QPushButton *m_btnOk;
	QPushButton *m_btnCancel;

private slots:
	void onPortChanged();
	void onRequestIntervalChanged();
	void onOkButtonClicked();
};

#endif // OPTIONSDIALOG_H
