#ifndef OBJECT_H
#define OBJECT_H

#include <QGraphicsObject>
#include "pos.h"

class Object : public QGraphicsPixmapItem
{
public:
    enum TYPE { NOWALL, SPEED };

    Object(const Pos &pos, const double scale) : scale(scale), pos(pos) { }

    bool isLive()       { if(time>0) time--; return (time!=0); }
    Pos  getPos()       { return pos; }
    int  getScore()     { return score; }
    TYPE getType()      { return type; }
    int  getDuration()  { return duration; }

    void initPixmap(double scale) {
        setPixmap(QPixmap(fileName));
        setScale(1./(pixmap().height()*scale));
    }

protected:
    int time = -1;      // оставшееся время жизни
    int score = 0;
    int duration = 60;  // Продолжительность эффектов
    double scale;
    Pos pos;            // расположение на сетке
    TYPE type;
    QString fileName;
};

#endif // OBJECT_H
