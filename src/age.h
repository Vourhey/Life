#ifndef AGE_H
#define AGE_H

#include <QObject>
#include <QVector>
#include <QBitArray>
#include <QList>
#include <QPoint>

class Age : public QObject
{
    Q_OBJECT
public:
    Age(QObject *parent = 0);
    Age(int height, int width, QObject *parent = 0);

    int height() const;
    int width() const;

    quint32 population() const;
    quint32 ageCount() const;

    void setLife(const QPoint &point, bool l);
    bool isLive(const QPoint &point) const;
    bool isLive(int x, int y) const;

    const QVector<QBitArray> &data() const;
    void setData(const QVector<QBitArray> &d);

signals:
    void ageChanged();

public slots:
    void nextAge();
    void clear();
    void setHeight(int height);
    void setWidth(int width);

private:
    void init();
    int neighborhood(int y, int x);
    bool isValid(int y, int x);

    struct ChangedPoints
    {
        ChangedPoints(int _y, int _x, bool _val)
        {
            x = _x;
            y = _y;
            val = _val;
        }

        int x;
        int y;
        bool val;
    };

    QVector<QBitArray> field;
    QList<ChangedPoints> points;
    int m_height, m_width;
    quint32 m_population;
    quint32 m_ageCount;
};

#endif // AGE_H
