#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QVector>

#include "scene/pos.h"
#include "scene/snake.h"

class Object;
class Fruit;
class Bonus;
class Area;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene();
    void addItem(QGraphicsItem*item, const Pos &pos);
    void removeItem(QGraphicsItem*item, const Pos &pos);
    void removeObject(Object*item);
    void start(int speed=2, int max_x=32, int max_y=32);
    void stop();
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void increaseSpeed(){ speed++; }

signals:
    void endgame();
    void scoreUp(int score);
    void infoChanged(QString info);

private:
    void timerEvent(QTimerEvent *) override;
    Pos randomDirection();

    Pos max;                        // размеры сетки
    Snake *snake = nullptr;
    Area *area;

    QGraphicsTextItem *text;
    QGraphicsItem *border;

    const int startDelay = 3;       // Стартовая задержка
    int timerId;
    int speed;
    int timeSpeedup, timeNoborder;  // обратные счетчики бонусов
    quint64 time, quicktime;        // время в тактах и с учетом ускорения

    QList<Object*> fruits;
    QList<Object*> bonuses;
};

#endif // SCENE_H
