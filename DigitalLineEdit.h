#ifndef DIGITALLINEEDIT_H
#define DIGITALLINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>

#include "Resources.h"

class DigitalLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit DigitalLineEdit(QWidget *parent = nullptr);
	void setScale(double min, double max);
	void setReadyToEdit(bool active);
	double value();

private:
	bool m_isReadyToEdit;
	double m_scaleMin;
	double m_scaleMax;
	QString m_savedText;

protected:
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void focusOutEvent(QFocusEvent *event) override;

public slots:
	void activateEdit();
	void cancelEdit();
	void applyEdit();
	void setValue(double value);
	void numberKey(quint8 number);
	void dotKey();
	void deleteEdit();

signals:
	void editValueChanged(double value);
	void editActivated();
	void editDeactivated();
};

#endif // DIGITALLINEEDIT_H
