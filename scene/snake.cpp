#include "snake.h"
#include "scene/scene.h"

Snake::Snake(Scene *scene, const Pos &pos, const Pos &direction) : scene(scene), direction(direction)
{
    directionOld = direction;
    addSegment(pos-direction*2);
    addSegment(pos-direction);
    replacePixmapTail();
    replacePixmapBody();
    addSegment(pos);
}

Snake::~Snake()
{
    for(int i=path.size(); --i;) removeSegment();
}

void Snake::move(const bool grow)
{
    // Восстановление после прохода через стену ! пока без доп. защиты
    replacePixmapBody();
    addSegment(getNext());
    direction = normDirection(direction);

    if(!grow) {
        removeSegment();
        replacePixmapTail();
    }
}

Pos Snake::normDirection(Pos direction)
{
    if(direction.x()>1)  direction.setX(-1);
    if(direction.x()<-1) direction.setX(1);
    if(direction.y()>1)  direction.setY(-1);
    if(direction.y()<-1) direction.setY(1);

    return direction;
}

void Snake::addSegment(const Pos &pos)
{
    QString fileName = ":/pics/snake_head.png";
    double angle = getAngle(normDirection(direction));

    auto *segment = new QGraphicsPixmapItem(QPixmap(fileName).transformed(QTransform().rotate(angle)));
    segment->setScale(1./(segment->pixmap().height()*(1./32)));

    path.append(pos);
    chain.append(segment);
    scene->addItem(segment, pos);
}

void Snake::replacePixmapBody()
{
    Pos directionNew = normDirection(direction);

    double rotate = getAngle(directionNew) - getAngle(directionOld);
    rotate+= rotate>=180. ? -360 : rotate<-180 ? 360 : 0;

    directionOld = directionNew;

    QString fileName = rotate ? ":/pics/snake_rotate.png" : ":/pics/snake_body.png";
    double angle = rotate ? getAngle(directionOld)-90. - (fabs(rotate)-rotate)/2. :
                            direction.x() ? 90. : 0;
    chain.last()->setPixmap(QPixmap(fileName).transformed(QTransform().rotate(angle)));
}

void Snake::replacePixmapTail()
{
    QString fileName = ":/pics/snake_tail.png";
    double angle = getAngle(path[0]-path[1]);
    chain.first()->setPixmap(QPixmap(fileName).transformed(QTransform().rotate(angle)));
}

void Snake::removeSegment()
{
    auto segment = chain.first();
    scene->removeItem(segment, path.first());
    path.removeFirst();
    chain.removeFirst();
    delete segment;
}
