#include "PresetsTableView.h"

PresetsTableView::PresetsTableView(QWidget *parent) : QTableView(parent)
{
	m_model = new PresetsTableModel();
	setModel(m_model);
	setColumnWidth(0, 150);
	setColumnWidth(1, 150);
	setColumnWidth(2, 150);
	setItemDelegate(new PresetsTableDelegate());
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setStyleSheet(FOCUSED_CELL_STYLE);
	connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(currentSelectionChanged(QModelIndex, QModelIndex)));
}

const PresetsList &PresetsTableView::presets()
{
	return m_model->presets();
}

bool PresetsTableView::isEdited()
{
	return m_model->isEdited();
}

void PresetsTableView::keyPressEvent(QKeyEvent *event)
{
	if ((event->key() == Qt::Key_Insert) && (event->modifiers() == Qt::ControlModifier))
	{
		if (!m_model->hasNewRecord())
		{
			appendRow();
		}
		else
		{
			int n = m_model->rowCount();
			if (n > 0) setCurrentIndex(model()->index(n - 1, 0));
		}
		return;
	}
	if ((event->key() == Qt::Key_Delete) && (event->modifiers() == Qt::ControlModifier))
	{
		QTableView::keyPressEvent(event);
		removeCurrentRow();
		return;
	}
	if ((event->key() == Qt::Key_Down) && (event->modifiers() == Qt::NoModifier))
	{
		QAbstractItemModel *mdl = model();
		int n = mdl->rowCount();
		int curr_row = currentIndex().row();
		if (curr_row == n - 1)
		{
			if (!m_model->hasNewRecord()) appendRow();
		}
		QTableView::keyPressEvent(event);
		return;
	}
	if ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Space))
	{
		QModelIndex idx = currentIndex();
		if (idx.isValid() && (idx.column() == 2))
		{
			bool v = false;
			QVariant d = idx.data(Qt::EditRole);
			if (!d.isNull()) v = !d.toBool();
			m_model->setData(idx, QVariant(v), Qt::EditRole);
		}
		return;
	}

	QTableView::keyPressEvent(event);
}

void PresetsTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QModelIndex idx = indexAt(event->pos());
		if (idx.isValid() && (idx.column() == 2))
		{
			bool v = false;
			QVariant d = idx.data(Qt::EditRole);
			if (!d.isNull()) v = !d.toBool();
			m_model->setData(idx, QVariant(v), Qt::EditRole);
		}
		return;
	}
	QTableView::mouseDoubleClickEvent(event);
}

void PresetsTableView::appendRow()
{
	int row = model()->rowCount();
	if (model()->insertRow(row)) setCurrentIndex(model()->index(row, 0));
}

void PresetsTableView::currentSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
	if (previous.isValid() && current.isValid())
	{
		if (current.row() != previous.row())
		{
			if (m_model->isRowEmpty(previous.row())) m_model->removeRow(previous.row());
		}
	}
}

void PresetsTableView::addNewRow()
{
	setFocus();
	if (!m_model->hasNewRecord()) appendRow();
	else
	{
		int n = m_model->rowCount();
		if (n > 0) setCurrentIndex(model()->index(n - 1, 0));
	}
}

void PresetsTableView::removeCurrentRow()
{
	QModelIndex idx = currentIndex();
	if (idx.isValid())
	{
		selectionModel()->blockSignals(true);
		model()->removeRow(idx.row());
		selectionModel()->blockSignals(false);
	}
}
