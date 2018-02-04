#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

#include "scene/pos.h"

class Scene;

class Snake
{
public:
    Snake(Scene *scene, const Pos &pos, const Pos &direction);
    ~Snake();

    void move(const bool grow = false);
    void setDirection(const Pos &d) { if(getDirectionOld()!=-d) direction = d; }
    Pos getDirectionOld()           { return directionOld; }
    Pos getDirection()              { return direction; }
    Pos getNext()                   { return path.last() + direction; }
    bool contains(const Pos &pos)   { return path.contains(pos); }

protected:
    void addSegment(const Pos &pos);
    void removeSegment();
    void replacePixmapBody();
    void replacePixmapTail();
    double getAngle(const Pos &a)   { return ( 2*abs(a.x())-a.x() + abs(a.y())+a.y() ) *90; }
    Pos normDirection(Pos direction);

    Scene *scene;
    Pos direction;   // Направление - приращение к текщей позиции
    Pos directionOld;
    QList<Pos> path; // координаты змеи от хвоста к голове
    QList<QGraphicsPixmapItem*> chain;
};

#endif // SNAKE_H
