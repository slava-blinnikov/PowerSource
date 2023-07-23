#include "PresetsTableModel.h"

PresetsTableModel::PresetsTableModel(QObject *parent) : QAbstractTableModel(parent)
{
	m_records = Resources::presets;
	m_edited = false;
}

bool PresetsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid())
	{
		if (role == Qt::EditRole)
		{
			int row = index.row();
			int col = index.column();
			switch (col)
			{
				case 0:
				{
					if (!value.isNull())
					{
						m_records[row].voltage = value.toDouble();
						m_records[row].initFlags |= VoltageInitedFlag;
					}
					else m_records[row].initFlags &= (~VoltageInitedFlag);
					m_edited = true;
					emit dataChanged(index, index);
				}
				break;
				case 1:
				{
					if (!value.isNull())
					{
						m_records[row].current = value.toDouble();
						m_records[row].initFlags |= CurrentInitedFlag;
					}
					else m_records[row].initFlags &= (~CurrentInitedFlag);
					m_edited = true;
					emit dataChanged(index, index);
				}
				break;
				case 2:
				{
					if (!value.isNull())
					{
						m_records[row].trigger = value.toBool();
						m_records[row].initFlags |= TriggerInitedFlag;
					}
					else m_records[row].initFlags &= (~TriggerInitedFlag);
					m_edited = true;
					emit dataChanged(index, index);
				}
				break;
			}
			return true;
		}
	}
	return false;
}

int PresetsTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_records.size();
}

int PresetsTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 3;
}

QVariant PresetsTableModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid())
	{
		const PresetRecord &rec = m_records.at(index.row());
		if (role == Qt::DisplayRole)
		{
			switch (index.column())
			{
				case 0: return ((rec.initFlags & VoltageInitedFlag) != 0) ? QVariant(Resources::normalizeValue(rec.voltage)) : QVariant();
				case 1: return ((rec.initFlags & CurrentInitedFlag) != 0) ? QVariant(Resources::normalizeValue(rec.current)) : QVariant();
				case 2: return ((rec.initFlags & TriggerInitedFlag) != 0) ? QVariant(rec.trigger) : QVariant();
			}
		}
		else if (role == Qt::EditRole)
		{
			switch (index.column())
			{
				case 0: return ((rec.initFlags & VoltageInitedFlag) != 0) ? QVariant(rec.voltage) : QVariant();
				case 1: return ((rec.initFlags & CurrentInitedFlag) != 0) ? QVariant(rec.current) : QVariant();
				case 2: return ((rec.initFlags & TriggerInitedFlag) != 0) ? QVariant(rec.trigger) : QVariant();
			}
		}
		else if (role == Qt::TextAlignmentRole)
		{
			if (index.column() < 2) return QVariant(int(Qt::AlignRight | Qt::AlignVCenter));
			else return QVariant(int(Qt::AlignCenter));
		}
	}
	return QVariant();
}

QVariant PresetsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
				case 0: return QVariant(QString("%1\n(%2)").arg(tr("Voltage")).arg(tr("V")));
				case 1: return QVariant(QString("%1\n(%2)").arg(tr("Current")).arg(tr("A")));
				case 2: return QVariant(tr("Trigger mode"));
			}
		}
	}
	else if (orientation == Qt::Vertical)
	{
		if (role == Qt::DisplayRole) return QVariant(section + 1);
	}
	return QVariant();
}

Qt::ItemFlags PresetsTableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flg = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (index.column() < 2) flg |= Qt::ItemIsEditable;
	return flg;
}

bool PresetsTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent)
	if (count == 1)
	{
		beginInsertRows(parent, row, row);
		PresetRecord rec;
		rec.initFlags = NoParameterInitedFlag;
		m_records.insert(row, rec);
		m_edited = true;
		endInsertRows();
		return true;
	}
	return false;
}

bool PresetsTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent)
	if (row + count <= m_records.size())
	{
		beginRemoveRows(parent, row, row + count - 1);
		m_records.remove(row, count);
		m_edited = true;
		endRemoveRows();
		return true;
	}
	return false;
}

bool PresetsTableModel::hasNewRecord()
{
	int n = m_records.size();
	if (n > 0) return (m_records.at(n - 1).initFlags != AllParametersInitedFlag);
	return false;
}

bool PresetsTableModel::isRowEmpty(int row)
{
	if (row >= 0 && row < m_records.size()) return m_records.at(row).initFlags == NoParameterInitedFlag;
	return true;
}

PresetRecord PresetsTableModel::preset(int row)
{
	if (row >= 0 && row < m_records.size()) return m_records.at(row);
	PresetRecord nullp;
	nullp.initFlags = NoParameterInitedFlag;
	return nullp;
}

const PresetsList &PresetsTableModel::presets()
{
	return m_records;
}

bool PresetsTableModel::isEdited()
{
	return m_edited;
}
