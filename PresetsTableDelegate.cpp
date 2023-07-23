#include<QLineEdit>

#include "PresetsTableDelegate.h"

PresetsTableDelegate::PresetsTableDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
	m_iconTriggerOn = QIcon(":/icons/trigger-on.png");
	m_iconTriggerOff = QIcon(":/icons/trigger-off.png");
}

QWidget *PresetsTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	if (index.column() < 2)
	{
		QLineEdit *le = new QLineEdit(parent);
		le->setInputMask(QString("%1.%2").arg(QString(Resources::num_digits, '0')).arg(QString(Resources::num_precision, '0')));
		le->setText(index.data(Qt::DisplayRole).toString());
		return le;
	}
	return nullptr;
}

void PresetsTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	Q_UNUSED(editor)
	Q_UNUSED(index)
}

void PresetsTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() < 2)
	{
		QLineEdit *le = qobject_cast<QLineEdit *>(editor);
		QString s = le->text().trimmed();
		bool ok;
		double value = s.toDouble(&ok);
		if (ok) model->setData(index, QVariant(value)); else model->setData(index, QVariant());
	}
}

void PresetsTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 2)
	{
		QVariant vd = index.data(Qt::EditRole);
		if (!vd.isNull())
		{
			if (option.state & QStyle::State_HasFocus) QStyledItemDelegate::paint(painter, option, index);
			else if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.highlight());
			if (vd.toBool()) m_iconTriggerOn.paint(painter, option.rect); else m_iconTriggerOff.paint(painter, option.rect);
			return;
		}
	}
	QStyledItemDelegate::paint(painter, option, index);
}
