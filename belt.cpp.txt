#include "belt.h"

Belt::Belt(float x, float y, float radius, int amount, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):GameItem(world)
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

    //We create "one" body first as the head of the chain
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.bullet = true;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    prev_body = world->CreateBody(&bodydef);

    //calculate x,y coordinate for each chain piece
    for(int i=1;i<amount;i++)
    {
        float x_cor = x;
        float y_cor = y - i*radius*0.5;

        //Create body for that piece of chain
        b2BodyDef piecedef;
        piecedef.type = b2_dynamicBody;
        piecedef.bullet = true;
        piecedef.position.Set(x_cor,y_cor);
        piecedef.userData = this;
        g_body = world->CreateBody(&piecedef);

        //fixture
        g_body->SetAngularDamping(3);
        g_body->CreateFixture(&fixturedef);

        //pixmap for each piece of chain (basically same picture)
        g_pixmap.setPixmap(pixmap);
        g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
        g_size = QSize(radius*2,radius*2);

        //Here's the different part, we attach each of the chain piece with REVOLUTE JOINT
        b2RevoluteJointDef revjointDef;
        revjointDef.localAnchorA.Set(0.75,0);
        revjointDef.localAnchorB.Set(-0.75,0);
        revjointDef.bodyA = prev_body;
        revjointDef.bodyB = g_body;
        world->CreateJoint(&revjointDef);  /*======= this line messed me up  @_@ =========*/
        //revjoint->SetMaxMotorTorque(1);
        //revjoint->EnableMotor(true);
        //change to the next body to form a chain (iterate)
        prev_body = g_body;

        //add the pic
        scene->addItem(&g_pixmap);
    }

    //bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

}
