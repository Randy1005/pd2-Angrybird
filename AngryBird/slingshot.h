#ifndef SLINGSHOT_H
#define SLINGSHOT_H

#include <gameitem.h>
#include <QGraphicsScene>

class Slingshot : public GameItem
{
public:
    Slingshot(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};


#endif // SLINGSHOT_H
