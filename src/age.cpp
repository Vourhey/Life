#include "age.h"

#include <QDebug>

/*!
 * ageChanged() испускается всякий раз, при
 * изменении поколения, а именно:
 * изменение размеров поля и состояния клеток
 */

Age::Age(QObject *parent)
    : QObject(parent)
{
    setHeight(15);
    setWidth(15);
}

Age::Age(int height, int width, QObject *parent)
    : QObject(parent)
{
    setHeight(height);
    setWidth(width);
}

void Age::setHeight(int height)
{
    qDebug() << "Height: " << height;

    field.resize(height);
    m_height = height;
    setWidth(m_width);

    emit ageChanged();
}

int Age::height() const
{
    return m_height;
}

void Age::setWidth(int width)
{
    m_width = width;
    for(int i=0; i<m_height; ++i) {
        field[i].resize(width);
    }
    emit ageChanged();
}

int Age::width() const
{
    return m_width;
}

// не проверяется на выход за пределы
void Age::setLife(const QPoint &point, bool l)
{
 //   qWarning() << "setLife(" << l << ")";
 //   qWarning() << point;
    field[point.y()][point.x()] =  l;
}

// возвращает значение в (y, x)
bool Age::isLive(int y, int x) const
{
  //  qWarning() << "getLife(" << field[y][x] << ")";
    return field[y][x];
}

const QVector<QBitArray> &Age::data() const
{
    return field;
}

void Age::clear()
{
    for(int i=0; i<m_height; ++i) {
        for(int j=0; j<m_width; ++j) {
            field[i][j] = false;
        }
    }

    emit ageChanged();
}

// изменения применяются к этому же объекту
void Age::nextAge()
{
    int neighbors;
    points.clear();

    // сохраняются только изменившиеся клекти
    for(int i=0; i<m_height; ++i) {
        for(int j=0; j<m_width; ++j) {
            neighbors = neighborhood(i, j);
            if(field[i][j]) { // живая клетка
                if(neighbors != 2 && neighbors != 3) {
                    points.append(ChangedPoints(i, j, false));
                }
            } else {    // метрвая
                if(neighbors == 3) {
                    points.append(ChangedPoints(i, j, true));
                }
            }
        }
    }

    qDebug() << "Size is " << points.size();
    foreach(ChangedPoints cp, points) {
        field[cp.y][cp.x] = cp.val;
    }

    emit ageChanged();
}

bool Age::isValid(int y, int x)
{
    if(y < 0 || x < 0 || y == m_height || x == m_width) {
        return false;
    }

    return true;
}

int Age::neighborhood(int y, int x)
{
    int count = 0;
    int i, j;

    for(i = y - 1; i <= (y+1); ++i) {
        for(j = x - 1; j <= (x+1); ++j) {
            if(isValid(i, j)) {
                if(field[i][j]) {
                    ++count;
                }
            }
        }
    }

    // избавляемся от дублирования
    if(field[y][x]) {
        --count;
    }

    return count;
}
