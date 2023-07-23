#include <QtMath>

#include "DigitalLineEdit.h"

DigitalLineEdit::DigitalLineEdit(QWidget *parent) : QLineEdit(parent)
{
	m_isReadyToEdit = false;
	m_scaleMin = 0;
	m_scaleMax = 100;
	setInputMask(QString("%1.%2").arg(QString(Resources::num_digits, '0')).arg(QString(Resources::num_precision, '0')));
	setReadOnly(true);
}

void DigitalLineEdit::setScale(double min, double max)
{
	if (min < max)
	{
		m_scaleMin = min;
		m_scaleMax = max;
	}
	else
	{
		m_scaleMin = max;
		m_scaleMax = min;
	}
}

void DigitalLineEdit::setReadyToEdit(bool active)
{
	m_isReadyToEdit = active;
}

double DigitalLineEdit::value()
{
	QString s = text().trimmed();
	bool ok;
	double v = s.toDouble(&ok);
	if (ok) if (v >= m_scaleMin && v <= m_scaleMax) return v;
	return 0;
}

void DigitalLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
	activateEdit();
	QLineEdit::mouseReleaseEvent(event);
}

void DigitalLineEdit::keyPressEvent(QKeyEvent *event)
{
	//qDebug() << event->key();
	if (!isReadOnly())
	{
		int key = event->key();
		if (key == Qt::Key_Escape) cancelEdit();
		else if (key == Qt::Key_Return || key == Qt::Key_Enter) applyEdit();
		else if (key == Qt::Key_Up || key == Qt::Key_Down)
		{
			int cp = cursorPosition();
			QString s = text();
			if (cp >= 0 && cp < s.length())
			{
				bool ok;
				int n = QString(s.at(cp)).toInt(&ok);
				if (ok)
				{
					if (key == Qt::Key_Up) n++; else n--;
					if (n >= 0 && n <= 9)
					{
						s[cp] = QChar(QString::number(n).at(0));
						setText(s);
						setCursorPosition(cp);
					}
				}
			}
		}
	}
	QLineEdit::keyPressEvent(event);
}

void DigitalLineEdit::focusOutEvent(QFocusEvent *event)
{
	cancelEdit();
	QLineEdit::focusOutEvent(event);
}

void DigitalLineEdit::activateEdit()
{
	if (m_isReadyToEdit && isReadOnly())
	{
		m_savedText = text();
		setReadOnly(false);
		emit editActivated();
	}
}

void DigitalLineEdit::cancelEdit()
{
	if (!isReadOnly())
	{
		setText(m_savedText);
		setReadOnly(true);
		emit editDeactivated();
	}
}

void DigitalLineEdit::applyEdit()
{
	QString s = text().trimmed();
	bool ok;
	double v = s.toDouble(&ok);
	if (ok)
	{
		if (v >= m_scaleMin && v <= m_scaleMax)
		{
			setReadOnly(true);
			setText(Resources::normalizeValue(v));
			emit editDeactivated();
			emit editValueChanged(v);
		}
	}
}

void DigitalLineEdit::setValue(double value)
{
	QString s = Resources::normalizeValue(value);
	if (isReadOnly()) setText(s); else m_savedText = s;
}

void DigitalLineEdit::numberKey(quint8 number)
{
	int cp = cursorPosition();
	QString s = text();
	s[cp] = QChar(QString::number(number).at(0));
	setText(s);
	setCursorPosition(cp);
	cursorForward(false);
}

void DigitalLineEdit::dotKey()
{
	int cp = cursorPosition();
	if (cp < Resources::num_digits) setCursorPosition(Resources::num_digits + 1);
}

void DigitalLineEdit::deleteEdit()
{
	int cp = cursorPosition();
	QString s = text();
	s[cp] = ' ';
	setText(s);
	setCursorPosition(cp);
	cursorBackward(false);
}
