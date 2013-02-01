#include <QPainter>
#include <QPaintEvent>
#include <Qt>
#include "fieldwidget.h"

#include <stdio.h>
#include <QDebug>

void printBitArray(const QBitArray &ba)
{
    for(int i=0; i<ba.size(); ++i) {
        fprintf(stderr, "%d ", ba[i] ? 1 : 0);
    }
    fprintf(stderr, "\n");
}

void printVector(const QVector<QBitArray> &vec)
{
    for(int i=0; i<vec.size(); ++i) {
         printBitArray( vec[i]); // << '\n';
    }
    fprintf(stderr, "\n");
}

/* TODO
 * виджет не должен расширяться дальше поля (сейчас появляется пустая область)
 */

FieldWidget::FieldWidget(QWidget *parent)
    : QWidget(parent)
{
    m_age = new Age;
    connect(m_age, SIGNAL(ageChanged()), SLOT(repaint()));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    squareSize = 16;
}

Age *FieldWidget::age()
{
    return m_age;
}

void FieldWidget::setReadOnly(bool ro)
{
    m_readOnly = ro;

    if(ro) {
        setCursor(Qt::ArrowCursor);
    } else {
        setCursor(Qt::PointingHandCursor);
    }
}

QSize FieldWidget::sizeHint() const
{
    int w = squareSize * m_age->width();
    int h = squareSize * m_age->height();

    return QSize(w, h);
}

void FieldWidget::mousePressEvent(QMouseEvent *e)
{
    qDebug() << e->button();
    if(e->button() == Qt::LeftButton && !m_readOnly) {
        int row = e->pos().y() / squareSize;
        int column =  e->pos().x() / squareSize;

        bool current = m_age->isLive(row, column);
      //  printVector(m_age.data());
        m_age->setLife(QPoint(column, row), !current);
      //  printVector(m_age.data());
        repaint();
    }
}

void FieldWidget::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->pos().x() / squareSize;
    int y = e->pos().y() / squareSize;

    qDebug() << e->button();
    // пока что не работает.. почему?..
    if(e->button() & Qt::LeftButton) {
        qDebug() << "I'm here\n";
        bool current = m_age->isLive(y, x);
        m_age->setLife(QPoint(x, y), !current);
        repaint();
    }
    qDebug() << "emit...";

    emit changedCoordinates(x, y);
}

void FieldWidget::paintEvent(QPaintEvent *e)
{
    resize(sizeHint());

    QPainter p(this);
    p.fillRect(e->rect(), QBrush(Qt::white));

    QRect redrawRect = e->rect();
    int beginRow = redrawRect.top()/squareSize;
    int endRow = redrawRect.bottom()/squareSize;
    int beginColumn = redrawRect.left()/squareSize;
    int endColumn = redrawRect.right()/squareSize;

    //qDebug("%d %d\n", endRow, endColumn);
  //  qDebug() << redrawRect << visibleRegion();

    p.setPen(QPen(Qt::gray));
    for (int row = beginRow; row <= endRow && row < m_age->height(); ++row) {
        for (int column = beginColumn; column <= endColumn && column < m_age->width(); ++column) {
            if(m_age->isLive(row, column)) {
                p.setBrush(Qt::black);
            }

            p.drawRect(column*squareSize, row*squareSize, squareSize, squareSize);
            p.setBrush(Qt::white);
        }
    }
}
