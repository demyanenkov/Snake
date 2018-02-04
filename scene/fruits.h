#ifndef FRUIT_H
#define FRUIT_H

#include "scene/object.h"

class Strawberry: public Object
{
public:
    Strawberry(const Pos &pos, const double scale) : Object(pos,scale) {
        fileName=":/pics/strawberry.png";
        time = duration;
        score = 20;
        initPixmap(scale);
    }
};

class Cherry : public Object
{
public:
    Cherry(const Pos &pos, const double scale) : Object(pos,scale) {
        fileName=":/pics/cherry.png";
        time = duration*0.8;
        score = 10;
        initPixmap(scale);
    }
};

class Banan : public Object
{
public:
    Banan(const Pos &pos, const double scale) : Object(pos,scale) {
        fileName=":/pics/bananas.png";
        time = duration*0.6;
        score = 5;
        initPixmap(scale);
    }
};

#endif // FRUIT_H
