#include <QStyleOption>
#include <QPainter>

#include "OnscreenKeyboard.h"

OnscreenKeyboard::OnscreenKeyboard(QWidget *parent) : QWidget(parent, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
{
	setAutoFillBackground(true);
	setObjectName("onscreenKeyboard");
	setStyleSheet("QWidget#onscreenKeyboard{background-color: rgb(219, 255, 255);}");
	QGridLayout *grbMain = new QGridLayout();
	grbMain->setContentsMargins(2, 2, 2, 2);
	NumberToolButton *btn;
	quint8 key = 0;
	for (int row = 0; row < 2; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			btn = new NumberToolButton(key, this);
			connect(btn, SIGNAL(clicked(bool)), this, SLOT(onNumberKeyPressed()));
			btn->setObjectName(QString("buttonKeyNumber%1").arg(key++));
			grbMain->addWidget(btn, row, col);
		}
	}
	m_btnDot = new QToolButton(this);
	m_btnDot->setObjectName("buttonEditDot");
	grbMain->addWidget(m_btnDot, 2, 0);
	m_btnDel = new QToolButton(this);
	m_btnDel->setObjectName("buttonEditDel");
	grbMain->addWidget(m_btnDel, 2, 1);
	setLayout(grbMain);

	m_btnCancel = new QToolButton(this);
	m_btnCancel->setObjectName("buttonEditCancel");
	grbMain->addWidget(m_btnCancel, 2, 3);
	setLayout(grbMain);

	m_btnOk = new QToolButton(this);
	m_btnOk->setObjectName("buttonEditOk");
	grbMain->addWidget(m_btnOk, 2, 4);
	setLayout(grbMain);

	connect(m_btnDot, SIGNAL(clicked(bool)), this, SIGNAL(dotKey()));
	connect(m_btnDel, SIGNAL(clicked(bool)), this, SIGNAL(deleteEdit()));
	connect(m_btnCancel, SIGNAL(clicked(bool)), this, SIGNAL(cancelEdit()));
	connect(m_btnOk, SIGNAL(clicked(bool)), this, SIGNAL(applyEdit()));
}

void OnscreenKeyboard::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OnscreenKeyboard::onNumberKeyPressed()
{
	NumberToolButton *btn = qobject_cast<NumberToolButton *>(sender());
	if (btn != nullptr) emit numberKey(btn->number());
}
