#include "NumberToolButton.h"

NumberToolButton::NumberToolButton(quint8 number, QWidget *parent) : QToolButton(parent)
{
	m_number = number;
}

quint8 NumberToolButton::number()
{
	return m_number;
}
