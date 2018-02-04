#include "scene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <ctime>

#include "scene/area.h"
#include "scene/object.h"
#include "scene/fruits.h"
#include "scene/bonus.h"

Scene::Scene()
{
    setBackgroundBrush(QPixmap(":/pics/ground.jpg"));
    srand(std::time(NULL));
}

void Scene::addItem(QGraphicsItem *item, const Pos &pos)
{
    area->add(pos, item);
    item->setPos(pos.x()*(width()/max.x()), pos.y()*(height()/max.y()));
    QGraphicsScene::addItem(item);
}

void Scene::removeItem(QGraphicsItem *item, const Pos &pos)
{
    area->remove(pos);
    QGraphicsScene::removeItem(item);
}

void Scene::removeObject(Object *item)
{
    fruits.removeAll(item);
    bonuses.removeAll(item);
    removeItem(item,item->getPos());
    delete item;
}

void Scene::start(int speed, int max_x, int max_y){
    clear();

    this->speed = speed;

    max = Pos(max_x, max_y);
    setSceneRect(0, 0, 1024, max.x() ? 1024*max.y()/max.x() : 0);
    update();

    // Отображение сетки
    QPen pen = QPen(QBrush("#CCCCCC"),1, Qt::DashLine);
    for(int i=1; i<max.x();i++)
        addLine(width()*i/max.x(), 0, width()*i/max.x(), height(), pen);
    for(int i=1; i<max.y();i++)
        addLine(0, height()*i/max.y(), width(), height()*i/max.y(), pen);

    pen.setStyle(Qt::SolidLine);
    border = addRect(0,0,width(),height(),pen);

    // Настройка игрового поля
    area = new Area(max);

    // Поиск места для змеи - не в углах и не в сторону границы
    int i=0;
    Pos pos = area->getEmpty(true), direction;
    do {
        direction = randomDirection();
        if(i>10) pos=area->getEmpty(true), i=0; i++; // защита от вечного цикла
    }
    while(area->isOut(pos+direction*5) || area->isOut(pos-2*direction));

    snake = new Snake(this, pos, direction);

    time = quicktime = 0;
    timeSpeedup = timeNoborder = 0;
    timerId = startTimer(500/speed);

    // Дополнительное сообщение при запуске
    text=addText("Waiting few seconds ...");
    text->setDefaultTextColor(Qt::white);
    text->setScale(height()/300);
    text->setPos(width()*.30, height()*.40);
    text->adjustSize();
}

void Scene::stop()
{
    killTimer(timerId);

    while(!fruits.isEmpty())  removeObject(fruits.last());
    while(!bonuses.isEmpty()) removeObject(bonuses.last());

    delete snake;
    delete area;

    clear();
}
void Scene::timerEvent(QTimerEvent *)
{
    ++quicktime;
    if(timeSpeedup<=0){ if(quicktime%2) return; }
    else timeSpeedup--;

    if(timeNoborder>0)  timeNoborder--;
    if(timeNoborder==1) border->show();

    ++time;

    // Надпись при запуске
    if(time<quint64(startDelay*speed)) return;
    else if(time==quint64(startDelay*speed)) {
        QGraphicsScene::removeItem(text);
        delete text;
    }

    // Надпись о состоянии
    QString info;
    if(timeNoborder>0) info+=QString("NoBorder...%1 ").arg(timeNoborder/speed);
    if(timeSpeedup>0)  info+=QString("SpeedUp...%1 ").arg(timeSpeedup/(speed*2));
    info.insert(0, (info.isEmpty() ? "" : "BONUSES: "));
    info.insert(0, QString("Level:%1  ").arg(speed));

    static QString oldInfo;
    if(oldInfo!=info) emit infoChanged(info);
    oldInfo = info;

    // Основа
    if(snake!=nullptr) {
        Pos next = snake->getNext();
        bool grow = false;

        // Выход за стенку
        if(area->isOut(next)) {
            if(timeNoborder)
                snake->setDirection((1-max.x())*snake->getDirection());
            else {
                stop();
                emit endgame();
                return;
            }
        }

        // Зацепился за хвост
        if(snake->contains(next)) {
            stop();
            emit endgame();
            return;
        }

        // Обход предметов на поле
        for(auto fruit:fruits){
            if(fruit->getPos()==next){
                grow = true;
                emit scoreUp(fruit->getScore());
                removeObject(fruit);
            }
            else if(!fruit->isLive()) removeObject(fruit);
        }

        for(auto bonus:bonuses){
            if(bonus->getPos()==next){
                if(bonus->getType() == Object::SPEED)
                    timeSpeedup += 2*bonus->getDuration();  // ускорение
                else{
                    timeNoborder += bonus->getDuration(); // отсутствие границы
                    border->hide();
                }
                removeObject(bonus);
            }
            else if(!bonus->isLive()) removeObject(bonus);
        }

        snake->move(grow);
    }

    // Создание новых фруктов
    double scale = 1./max.y();

    if(fruits.isEmpty() || (!(time%10) && fruits.size()<3)){
        Pos pos = area->getEmpty();

        Object *fruit;
        int n = rand()%6;
        if(n<2)      fruit = new Strawberry(pos, scale);
        else if(n<4) fruit = new Cherry(pos, scale);
        else         fruit = new Banan(pos, scale);

        addItem(fruit, fruit->getPos());
        fruits.append(fruit);
    }

    // Создание бонусов - намного реже фруктов
    if(!(time%50)){
        Bonus *bonus = new Bonus(area->getEmpty(),scale);
        addItem(bonus, bonus->getPos());
        bonuses.append(bonus);
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(snake!=nullptr){
        switch(event->key()){
        case Qt::Key_Left:  case Qt::Key_A: snake->setDirection(Pos(-1,0)); break;
        case Qt::Key_Right: case Qt::Key_D: snake->setDirection(Pos(1,0));  break;
        case Qt::Key_Up:    case Qt::Key_W: snake->setDirection(Pos(0,-1)); break;
        case Qt::Key_Down:  case Qt::Key_S: snake->setDirection(Pos(0,1));  break;
        }
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        Pos pos = (event->scenePos()).toPoint() / max.x();

        if(snake->getDirectionOld().x()==0) // вертикальное или горизонтальное движение
                snake->setDirection(Pos(snake->getNext().x()<pos.x() ? 1 : -1, 0));
        else    snake->setDirection(Pos(0, snake->getNext().y()<pos.y() ? 1 : -1));
    }
}

Pos Scene::randomDirection()
{
    int d = rand()%4;
    return Pos((d-2)%2, (d-1)%2);
}
