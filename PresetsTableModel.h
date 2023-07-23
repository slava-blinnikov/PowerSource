#ifndef PRESETSTABLEMODEL_H
#define PRESETSTABLEMODEL_H

#include <QAbstractTableModel>

#include "Resources.h"

class PresetsTableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit PresetsTableModel(QObject *parent = nullptr);
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool hasNewRecord();
	bool isRowEmpty(int row);
	PresetRecord preset(int row);
	const PresetsList &presets();
	bool isEdited();

private:
	enum PresetInitFlag
	{
		NoParameterInitedFlag = 0,
		AllParametersInitedFlag = 0x0007,
		VoltageInitedFlag = 0x0001,
		CurrentInitedFlag = 0x0002,
		TriggerInitedFlag = 0x0004
	};
	PresetsList m_records;
	bool m_edited;
};

#endif // PRESETSTABLEMODEL_H
