#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fieldwidget.h"

class QTimer;
class SpinBox;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void pauseStartSlot();
    void stopSlot();
    void setPeriod(int p);
    void updateCoordinates(int x, int y);
    void updateLabels();
    void saveStateInFile();
    void loadStateFromFile();
    void randomSlot();
    void aboutSlot();
    
private:
    void enableTools(bool e);

    FieldWidget *fieldWidget;
    Age *stateAge;
    QToolBar *m_toolBar;
    SpinBox *m_heightBox;
    SpinBox *m_widthBox;
    SpinBox *m_secBox;
    SpinBox *m_zoomBox;
    QLabel *xyLabel;
    QLabel *populationLabel;
    QLabel *agesLabel;

    QAction *startPauseAct;
    QAction *stopAct;
    QAction *clearAct;

    QTimer *timer;
    int period;
    bool inprocess;
};

#endif // MAINWINDOW_H
