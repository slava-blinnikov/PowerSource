#ifndef PRESETSTABLEDELEGATE_H
#define PRESETSTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QIcon>

#include "Resources.h"

class PresetsTableDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit PresetsTableDelegate(QObject *parent = nullptr);
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QIcon m_iconTriggerOn;
	QIcon m_iconTriggerOff;
};

#endif // PRESETSTABLEDELEGATE_H
