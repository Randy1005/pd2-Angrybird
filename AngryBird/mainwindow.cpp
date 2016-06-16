#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Enable the event Filter
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    //Setting the QGraphicsScene
    scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);
    //Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));
    //Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());
    //Create ground
    itemList.push_back(new Land(16,1.5,32,3,QPixmap(":/image/ground.png").scaled(width()*2,height()/5.0),world,scene));
    //Create slingshot
    itemList.push_back(new Slingshot(3.5,5.5,2.5,4,QPixmap(":/image/slingshot.png").scaled(width()*0.06,height()*0.3),world,scene));
    //Create bird
    mushroom = new Bird(2,5,0.27f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/9.0,height()/9.0),world,scene);
    goomba = new Bird(2,5,0.3f,&timer,QPixmap(":/image/goo.png").scaled(height()/9.0,height()/9.0),world,scene);
    //Create barrier
    wood = new Barrier(24,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    wood2 = new Barrier(18,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    hori_wood = new Barrier(19,16,5,1,&timer,QPixmap(":/image/barrier_hori.png").scaled(height()/2.0,height()/2.8),world,scene);
    //Create enemy
    lbj = new Bird(21,6,0.55f,&timer,QPixmap(":/image/LBJ.png").scaled(height()/8.0,height()/6.0),world,scene);
    //Setting the Velocity
    mushroom->setLinearVelocity(b2Vec2(0,0));
    goomba->setLinearVelocity(b2Vec2(0,0));
    //Store items
    itemList.push_back(goomba);
    itemList.push_back(mushroom);
    /*Mouse Drag*/
    //mouse joint definition
    /*
    b2MouseJointDef *mouseJointDef = new b2MouseJointDef();
    mouseJointDef->bodyA = itemList.at(1)->g_body; //bodyA is the "Land" body
    mouseJointDef->bodyB = itemList.at(3)->g_body; //bodyB is the "mushroom" body
    mouseJointDef->target.Set(itemList.at(3)->g_body->GetPosition().x,itemList.at(3)->g_body->GetPosition().y); //set to the position of bodyB
    mouseJointDef->collideConnected = true; //still collide with "Land"
    mouseJointDef->maxForce = 300*itemList.at(3)->g_body->GetMass();
    */

    /*Chain*/


    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    timer.start(100/6);
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{

    // Hint: Notice the Number of every event!
    if(event->type() == QEvent::MouseButtonPress)
    {
        /* TODO : add your code here */
        click++;
        if(click >= 5)
        {
            cnt++;
            click = 0;
        }

        if(cnt == 1)
        {
            delete mushroom;
            Bird *mushroom = new Bird(2.4,12,0.27f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/9.0,height()/9.0),world,scene);
        }

        //std::cout << "Press !" << std::endl ;
    }
    if(event->type() == QEvent::MouseMove)
    {
        /* TODO : add your code here */

       //mouse_joint.SetTarget(b2vec2(QCursor::pos().x(),QCursor::pos().y());

        //std::cout << "Move !" << std::endl ;
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        /* TODO : add your code here */

        //world->DestroyJoint(mouse_joint);
        std::cout<<click<<std::endl;
        //std::cout << "Release !" << std::endl ;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::tick()
{
    world->Step(1.0/60.0,6,2);
    scene->update();
}

void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
}
