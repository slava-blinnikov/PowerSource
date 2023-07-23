#ifndef ONSCREENKEYBOARD_H
#define ONSCREENKEYBOARD_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QGridLayout>
#include <QPaintEvent>

#include "NumberToolButton.h"

class OnscreenKeyboard : public QWidget
{
	Q_OBJECT
public:
	explicit OnscreenKeyboard(QWidget *parent = nullptr);

private:
	QToolButton *m_btnCancel;
	QToolButton *m_btnOk;
	QToolButton *m_btnDot;
	QToolButton *m_btnDel;

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	void onNumberKeyPressed();

signals:
	void numberKey(quint8 number);
	void dotKey();
	void deleteEdit();
	void cancelEdit();
	void applyEdit();
};
#endif // ONSCREENKEYBOARD_H
