#ifndef BELT_H
#define BELT_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BELT_DENSITY 1.0f
#define BELT_FRICTION 1.0f
#define BELT_RESTITUTION 0.3f

class Belt : public GameItem
{
public:
    Belt(float x, float y, float radius,int amount, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,b2Body *relativeBody);

};

#endif // BELT_H
