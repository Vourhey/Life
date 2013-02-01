#include <QtGui>
#include "mainwindow.h"
#include "spinbox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), timer(0), inprocess(false)
{
    fieldWidget = new FieldWidget(this);
    QScrollArea *sa = new QScrollArea;
    sa->setWidget(fieldWidget);
    setCentralWidget(sa);
    stateAge = fieldWidget->age();

    m_toolBar = addToolBar(tr("Main"));

    startPauseAct = m_toolBar->addAction(tr("Start"), this, SLOT(pauseStartSlot()));
    stopAct = m_toolBar->addAction(tr("Stop"), this, SLOT(stopSlot()));

    m_heightBox = new SpinBox(tr("h: "));
    m_heightBox->m_spin->setRange(5, 500);
    m_heightBox->m_spin->setValue(stateAge->height());
    connect(m_heightBox->m_spin, SIGNAL(valueChanged(int)), stateAge, SLOT(setHeight(int)));

    m_widthBox = new SpinBox(tr("w: "));
    m_widthBox->m_spin->setRange(5, 500);
    m_widthBox->m_spin->setValue(stateAge->width());
    connect(m_widthBox->m_spin, SIGNAL(valueChanged(int)), stateAge, SLOT(setWidth(int)));

    // почему-то этот спин не позволяет редактировать значение внутри него
    m_secBox = new SpinBox(tr("ms: "));
    period = 500;
    m_secBox->m_spin->setRange(50, 60000); // от 0.2 до 60 сек
    m_secBox->m_spin->setValue(period);
    connect(m_secBox->m_spin, SIGNAL(valueChanged(int)), SLOT(setPeriod(int)));

    m_toolBar->addWidget(m_heightBox);
    m_toolBar->addWidget(m_widthBox);
    m_toolBar->addWidget(m_secBox);
    clearAct = m_toolBar->addAction(tr("Clear"), stateAge, SLOT(clear()));

    xyLabel = new QLabel;
    connect(fieldWidget, SIGNAL(changedCoordinates(int,int)), SLOT(updateCoordinates(int,int)));

    QStatusBar *sBar = statusBar();
    sBar->addPermanentWidget(xyLabel);
}

void MainWindow::pauseStartSlot()
{
    if(inprocess) {
        startPauseAct->setText(tr("Start"));
        timer->stop();
    } else {
        startPauseAct->setText(tr("Pause"));
        enableTools(false);

        if(!timer) {
            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), stateAge, SLOT(nextAge()));
        }
        timer->start(period);
    }
}

void MainWindow::stopSlot()
{
    timer->stop();
    startPauseAct->setText(tr("Start"));
    enableTools(true);
}

void MainWindow::updateCoordinates(int x, int y)
{
    xyLabel->setText(tr("x: %1 y: %2").arg(x).arg(y));
}

void MainWindow::setPeriod(int p)
{
    period = p;
}

void MainWindow::enableTools(bool e)
{
    m_heightBox->setEnabled(e);
    m_widthBox->setEnabled(e);
    m_secBox->setEnabled(e);
    clearAct->setEnabled(e);
    fieldWidget->setReadOnly(!e);
    inprocess = !e;
}
