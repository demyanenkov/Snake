#ifndef BONUS_H
#define BONUS_H

#include "scene/object.h"

class Bonus : public Object
{
public:
    Bonus(const Pos &pos, const double scale) : Object(pos,scale)
    {
        switch(rand()%2){
        case 1:
            fileName=":/pics/nowall.png";
            type = NOWALL;
            time = duration;
            break;

        default:
            fileName=":/pics/speed.png";
            time = duration*2;
            type = SPEED;
        }

        initPixmap(scale);
    }
};

#endif // BONUS_H
