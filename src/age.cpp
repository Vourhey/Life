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
    init();
    setHeight(150);
    setWidth(150);
}

Age::Age(int height, int width, QObject *parent)
    : QObject(parent)
{
    init();
    setHeight(height);
    setWidth(width);
}

void Age::init()
{
    m_height     = 0;
    m_width      = 0;
    m_population = 0;
    m_ageCount   = 0;
}

void Age::setHeight(int height)
{
    qDebug() << "Height: " << height;

    m_height = height;
    field.resize(height);
    setWidth(m_width);

    emit ageChanged();
}

int Age::height() const
{
    return m_height;
}

void Age::setWidth(int width)
{
    qDebug() << "Width: " << m_width;
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

quint32 Age::population() const
{
    return m_population;
}

quint32 Age::ageCount() const
{
    return m_ageCount;
}

// не проверяется на выход за пределы
void Age::setLife(const QPoint &point)
{
 //   qWarning() << "setLife(" << l << ")";
 //   qWarning() << point;
    field[point.x()].toggleBit(point.y());

    if(field[point.x()][point.y()]) {
        ++m_population;
    } else {
        --m_population;
    }

    emit ageChanged();
}

// возвращает значение в (y, x)
bool Age::isLive(const QPoint &point) const
{
  //  qWarning() << "getLife(" << field[y][x] << ")";
    return field[point.x()][point.y()];
}

bool Age::isLive(int x, int y) const
{
    return field[x][y];
}

void Age::setData(const QVector<QBitArray> &d)
{
    field = d;
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

    m_population = 0;
    m_ageCount = 0;

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
                    --m_population;
                }
            } else {    // метрвая
                if(neighbors == 3) {
                    points.append(ChangedPoints(i, j, true));
                    ++m_population;
                }
            }
        }
    }

    qDebug() << "Size is " << points.size();
    foreach(ChangedPoints cp, points) {
        field[cp.y][cp.x] = cp.val;
    }

    ++m_ageCount;

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

QDataStream &operator<<(QDataStream &stream, const Age *a)
{
    stream << a->m_height << a->m_width << a->m_population;
    stream << a->field;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Age *a)
{
    stream >> a->m_height >> a->m_width >> a->m_population;
    stream >> a->field;
    a->m_ageCount = 0;
    return stream;
}

