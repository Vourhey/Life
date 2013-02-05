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

    populationLabel = new QLabel;
    agesLabel = new QLabel;
    stateAge = fieldWidget->age();
    connect(stateAge, SIGNAL(ageChanged()), SLOT(updateLabels()));
    updateLabels();

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

    m_zoomBox = new SpinBox(tr("zoom: "));
    m_zoomBox->m_spin->setRange(1, 100);
    m_zoomBox->m_spin->setValue(fieldWidget->zoom());
    connect(m_zoomBox->m_spin, SIGNAL(valueChanged(int)), fieldWidget, SLOT(setZoom(int)));

    m_toolBar->addWidget(m_heightBox);
    m_toolBar->addWidget(m_widthBox);
    m_toolBar->addWidget(m_secBox);
    m_toolBar->addWidget(m_zoomBox);
    clearAct = m_toolBar->addAction(tr("Clear"), stateAge, SLOT(clear()));
    m_toolBar->addAction(tr("Save..."), this, SLOT(saveStateInFile()));
    m_toolBar->addAction(tr("Open..."), this, SLOT(loadStateFromFile()));
    m_toolBar->addAction(tr("Random"), this, SLOT(randomSlot()));
    m_toolBar->addAction(tr("About..."), this, SLOT(aboutSlot()));

    xyLabel = new QLabel;
    connect(fieldWidget, SIGNAL(changedCoordinates(int,int)), SLOT(updateCoordinates(int,int)));

    QStatusBar *sBar = statusBar();
    sBar->addPermanentWidget(xyLabel);
    sBar->addPermanentWidget(populationLabel);
    sBar->addPermanentWidget(agesLabel);
}

void MainWindow::pauseStartSlot()
{
    if(inprocess) {
        startPauseAct->setText(tr("Start"));
        timer->stop();
        inprocess = false;
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

void MainWindow::updateLabels()
{
    populationLabel->setText(tr("Population: %1").arg(stateAge->population()));
    agesLabel->setText(tr("Ages: %1").arg(stateAge->ageCount()));
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

void MainWindow::saveStateInFile()
{
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save State"), "examples");

   QFile saveFile(fileName);
   if(!saveFile.open(QIODevice::WriteOnly)) {
       qWarning("Can't open file %s", qPrintable(fileName));
       return;
   }

   QDataStream out(&saveFile);
   out << stateAge->height() << stateAge->width();
   out << stateAge->data();
}

void MainWindow::loadStateFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open State"), "example");

    QFile loadFile(fileName);
    if(!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Can't open file %s", qPrintable(fileName));
        return;
    }

    QDataStream in(&loadFile);
    int m;
    in >> m;
    stateAge->setHeight(m);
    in >> m;
    stateAge->setWidth(m);
    QVector<QBitArray> data;
    in >> data;
    stateAge->setData(data);
    fieldWidget->update();
}

void MainWindow::randomSlot()
{
    int h = stateAge->height();
    int w = stateAge->width();
    
    int cells = h * w / 3;  // треть поля

    qWarning("Cells: %d", cells);

    fieldWidget->setUpdatesEnabled(false);

    srand((unsigned)time(NULL));
    while(cells) {
        QPoint p;
        p.rx() = rand() % w;
        p.ry() = rand() % h;
        stateAge->setLife(p, true);
        --cells;
    }

    fieldWidget->setUpdatesEnabled(true);
}

void MainWindow::aboutSlot()
{
    QMessageBox::about(this, tr("About Life"), tr("Программа-симулятор для игры Conwey's Life.\n"
                        "Осторожно: использован самый неэффективный алгоритм, основанный на двух массивах"));
}

