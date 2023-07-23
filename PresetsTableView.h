#ifndef PRESETSTABLEVIEW_H
#define PRESETSTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>

#include "PresetsTableModel.h"
#include "PresetsTableDelegate.h"

class PresetsTableView : public QTableView
{
	Q_OBJECT
public:
	explicit PresetsTableView(QWidget *parent = nullptr);
	const PresetsList &presets();
	bool isEdited();

protected:
	void keyPressEvent(QKeyEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	PresetsTableModel *m_model;
	void appendRow();

private slots:
	void currentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

public slots:
	void addNewRow();
	void removeCurrentRow();
};

#endif // PRESETSTABLEVIEW_H
