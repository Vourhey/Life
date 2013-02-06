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
    squareSize = 10;
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

int FieldWidget::zoom() const
{
    return squareSize;
}

void FieldWidget::setZoom(int z)
{
    squareSize = z;
    if(squareSize <= 0) {
        squareSize = 1;
    }
    repaint();
}

/*
void FieldWidget::zoomIn()
{
    setZoom(squareSize + 1);
}

void FieldWidget::zoomOut()
{
    --squareSize;
    if(squareSize <= 0) {
        squareSize = 1;
    }
    repaint();
}
*/

QSize FieldWidget::sizeHint() const
{
    int w = squareSize * m_age->width();
    int h = squareSize * m_age->height();

    return QSize(w, h);
}

void FieldWidget::setCell(const QPoint &p)
{
    if(!m_readOnly) {
        m_age->setLife(p);
    }
}

void FieldWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        int row = e->pos().y() / squareSize;
        int column =  e->pos().x() / squareSize;

        setCell(QPoint(row, column));
    }
}

void FieldWidget::mouseMoveEvent(QMouseEvent *e)
{
    static QPoint lastPos;

    int x = e->pos().x() / squareSize;
    int y = e->pos().y() / squareSize;

    if(lastPos == QPoint(x, y))
        return;

    lastPos = QPoint(x, y);

    if(e->buttons() & Qt::LeftButton) {
        setCell(QPoint(y, x));
    }

    emit changedCoordinates(x, y);
}

void FieldWidget::wheelEvent(QWheelEvent *e)
{
    int numStep = e->delta() / 8 / 15;

    setZoom(squareSize + numStep);
    emit changedZoom(squareSize);
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

    if(squareSize > 4 && !m_readOnly) {
        p.setPen(QPen(Qt::gray));
    } else {
        p.setPen(Qt::NoPen);
    }

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
