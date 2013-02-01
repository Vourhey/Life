#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>
#include "age.h"

class FieldWidget : public QWidget
{
    Q_OBJECT
public:
    FieldWidget(QWidget *parent = 0);

    void setReadOnly(bool ro);
  //  void setAge(const Age &age);

    Age *age();

    QSize sizeHint() const;
 //   QSize minimumSizeHint() const;

signals:
    void changedCoordinates(int x, int y);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
 //   void mouseReleaseEvent(QMouseEvent *);

private:
    bool m_readOnly;
    Age *m_age;
    int squareSize;
    QPoint lastPos;
};

#endif // FIELDWIDGET_H
