#include <QHBoxLayout>
#include <QVBoxLayout>

#include "PresetsDialog.h"

PresetsDialog::PresetsDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(QString("%1 - %2").arg(tr("Power supply")).arg(tr("Presets")));
	m_applyRow = -1;
	QVBoxLayout *vblMain = new QVBoxLayout();
	vblMain->setContentsMargins(2, 3, 2, 3);

	QHBoxLayout *hbl = new QHBoxLayout();
	QSize icn_size(20, 20);
	m_btnAppend = new QPushButton(QIcon(":/icons/append.png"), QString(), this);
	m_btnAppend->setIconSize(icn_size);
	hbl->addWidget(m_btnAppend);
	m_btnDelete = new QPushButton(QIcon(":/icons/remove.png"), QString(), this);
	m_btnDelete->setIconSize(icn_size);
	hbl->addWidget(m_btnDelete);
	hbl->addStretch();
	vblMain->addLayout(hbl);

	m_table = new PresetsTableView(this);
	vblMain->addWidget(m_table);

	hbl = new QHBoxLayout();
	m_btnApply = new QPushButton(tr("Apply"), this);
	hbl->addWidget(m_btnApply);
	m_btnOk = new QPushButton(tr("Ok"), this);
	hbl->addWidget(m_btnOk);
	vblMain->addLayout(hbl);
	hbl->addStretch();
	m_btnCancel = new QPushButton(tr("Cancel"), this);
	hbl->addWidget(m_btnCancel);

	setLayout(vblMain);
	resize(500, 400);

	connect(m_btnCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(m_btnAppend, SIGNAL(clicked(bool)), m_table, SLOT(addNewRow()));
	connect(m_btnDelete, SIGNAL(clicked(bool)), m_table, SLOT(removeCurrentRow()));
	connect(m_btnApply, SIGNAL(clicked(bool)), this, SLOT(onApplyClicked()));
	connect(m_btnOk, SIGNAL(clicked(bool)), this, SLOT(onOkClicked()));
}

int PresetsDialog::applyRow()
{
	return m_applyRow;
}

bool PresetsDialog::isEdited()
{
	return m_table->isEdited();
}

void PresetsDialog::onApplyClicked()
{
	QModelIndexList lst = m_table->selectionModel()->selectedRows();
	if (!lst.isEmpty()) m_applyRow = lst.at(0).row();
	Resources::presets = m_table->presets();
	accept();
}

void PresetsDialog::onOkClicked()
{
	m_applyRow = -1;
	Resources::presets = m_table->presets();
	accept();
}
