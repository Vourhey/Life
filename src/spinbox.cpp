#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include "spinbox.h"

SpinBox::SpinBox(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    QLabel *label = new QLabel(text);
    m_spin = new QSpinBox;

    QHBoxLayout *mainLayout = new QHBoxLayout;

    mainLayout->addWidget(label);
    mainLayout->addWidget(m_spin);

    setLayout(mainLayout);
}
