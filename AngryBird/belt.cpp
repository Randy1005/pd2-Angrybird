#include "belt.h"

Belt::Belt(float x, float y, float radius, int amount, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene, b2Body *groundBody):GameItem(world)
{
    //Create circle shape
    b2CircleShape bodyshape;
    bodyshape.m_radius = radius;

    //define fixture for that piece of something
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = BELT_DENSITY;
    fixturedef.friction = BELT_FRICTION;
    fixturedef.restitution = BELT_RESTITUTION;

    //calculate x,y coordinate for each piece
    for(int i=0;i<amount;i++)
    {
        float x_cor = x;
        float y_cor = y + i*radius;

        //Create body for that piece of something
        b2BodyDef bodydef;
        bodydef.type = b2_dynamicBody;
        bodydef.bullet = true;
        bodydef.position.Set(x_cor,y_cor);
        bodydef.userData = this;
        g_body = world->CreateBody(&bodydef);

        //fixture
        g_body->SetAngularDamping(3);
        g_body->CreateFixture(&fixturedef);

        //pixmap for each piece of something
        g_pixmap.setPixmap(pixmap);
        g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
        g_size = QSize(radius*2,radius*2);

        //Here's the different part, we attach each of that "piece" with REVOLUTE JOINT
        b2RevoluteJointDef revjointDef;
        revjointDef.bodyA = g_body;
        revjointDef.bodyB = groundBody;
        //b2RevoluteJoint *revjoint = new b2RevoluteJoint(&revjointDef);
        //revjoint->SetMaxMotorTorque(1);
        //revjoint->EnableMotor(true);

        //bound timer
        connect(timer, SIGNAL(timeout()), this,SLOT(paint()));
        //add the pic
        scene->addItem(&g_pixmap);
    }


}
