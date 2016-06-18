#ifndef STRIKEBIRD_H
#define STRIKEBIRD_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
#include <bird.h>

#define BIRD_DENSITY 10.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.5f

class StrikeBird : public GameItem
{
public:
    StrikeBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    void setLinearVelocity(b2Vec2 velocity);
    virtual void birdFunction();


    //The fist of god!!!!!!
    Bird *fist;
    bool activateAbility = false;
};

#endif // STRIKEBIRD_H
