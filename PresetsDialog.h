#ifndef PRESETSDIALOG_H
#define PRESETSDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "PresetsTableView.h"

class PresetsDialog : public QDialog
{
	Q_OBJECT
public:
	explicit PresetsDialog(QWidget *parent = nullptr);
	int applyRow();
	bool isEdited();

private:
	PresetsTableView *m_table;
	QPushButton *m_btnAppend;
	QPushButton *m_btnDelete;
	QPushButton *m_btnCancel;
	QPushButton *m_btnApply;
	QPushButton *m_btnOk;
	int m_applyRow;

private slots:
	void onApplyClicked();
	void onOkClicked();
};

#endif // PRESETSDIALOG_H
