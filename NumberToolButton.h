#ifndef NUMBERTOOLBUTTON_H
#define NUMBERTOOLBUTTON_H

#include <QToolButton>

class NumberToolButton : public QToolButton
{
	Q_OBJECT
public:
	explicit NumberToolButton(quint8 number, QWidget *parent = nullptr);
	quint8 number();

private:
	quint8 m_number;
};

#endif // NUMBERTOOLBUTTON_H
