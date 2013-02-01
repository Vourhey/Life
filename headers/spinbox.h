#ifndef SPINBOX_H
#define SPINBOX_H

#include <QWidget>

class QSpinBox;

class SpinBox : public QWidget
{
    Q_OBJECT
public:
    SpinBox(const QString &text, QWidget *parent = 0);

    QSpinBox *m_spin;
};

#endif // SPINBOX_H
