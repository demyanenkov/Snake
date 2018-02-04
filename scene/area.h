#ifndef AREA_H
#define AREA_H

#include <QObject>
#include <QVector>
#include <QGraphicsItem>
#include <math.h>

#include "pos.h"

class Area
{
public:
    Area(const Pos &max):max(max)
    {
        vector = new QVector<QGraphicsItem*> (max.x()*max.y());
        vector->fill(nullptr);
    }
    ~Area(){ delete vector; }

    void add(const Pos &pos, QGraphicsItem *item)  { vector->replace(index(pos),item); }
    void remove(const Pos &pos)                    { add(pos, nullptr); }
    QGraphicsItem* get(const Pos &pos)             { return vector->at(index(pos)); }
    bool isEmpty(const Pos &pos)                   { return (get(pos)==nullptr); }
    bool isOut(const Pos &pos)                     { return pos.x()<0 || pos.y()<0 || pos.x()>=max.x() || pos.y()>=max.y(); }
    Pos getEmpty(bool nocorner = false);

private:
    int index(const Pos &pos) { return pos.x() + pos.y()*max.x(); }

    Pos max;
    QVector<QGraphicsItem*> *vector;
};

// РЕАЛИЗАЦИЯ
Pos Area::getEmpty(bool nocorner)
{
    Pos pos;
    do pos = { rand()%max.x(), rand()%max.y() };
    while(!isEmpty(pos) || (nocorner && (pos.x()==0 || pos.x()==(max.x()-1)) && (pos.y()==0 || pos.y()==(max.y()-1))) );
    return pos;
}

#endif // AREA_H
