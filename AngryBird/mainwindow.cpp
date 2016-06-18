#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;


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
    goomba = new FloppyBird(6,8,0.3f,&timer,QPixmap(":/image/goo.png").scaled(height()/9.0,height()/9.0),world,scene);
    blue = new SprintBird(8,8,0.27f,&timer,QPixmap(":/image/blue_bird").scaled(height()/9.0,height()/9.0),world,scene);
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

    /*Chain*/
    //Belt *belt = new Belt(10,10,0.6,5,&timer,QPixmap(":/image/blue_bird.png").scaled(height()/10.0,height()/10.0),world,scene);
    /*can't do shit about this =3=*/


    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    timer.start(100/6);
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{

    if(event->type() == QEvent::MouseButtonPress)
    {
        mousePressed = true;

        click++;
        if(click >= 5)
        {
            cnt++;
            click = 0;
        }
        cout<<cnt<<endl;
        /*
        if(cnt == 1 && firstFired == false)
        {
            delete mushroom;
            mushroom = new Bird(2.4,12,0.27f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/9.0,height()/9.0),world,scene);
            firstFired = true;
            mousePressed = true;
        }

        if(cnt == 2 && secondFired == false) //cnt = 3 because I clicked once to set the goomba on slingshot
        {
            delete goomba;
            goomba = new FloppyBird(2.4,12,0.27f,&timer,QPixmap(":/image/goo.png").scaled(height()/9.0,height()/9.0),world,scene);
            secondFired = true;
            mousePressed = true;
        }
        */


        //std::cout << "Press !" << std::endl ;
    }
    if(event->type() == QEvent::MouseMove)
    {
        float x = QCursor::pos().x() * 32/960 - 7.0;
        float y = 22.0 - QCursor::pos().y() * 18/540;
        mousePosition = b2Vec2(x,y);
        if(cnt==1 && mousePressed == true)
        {
            mushroom->g_body->SetTransform(mousePosition,mushroom->g_body->GetAngle());
        }

        if(cnt==2 && mousePressed == true)
        {
            goomba->g_body->SetTransform(mousePosition,goomba->g_body->GetAngle());
        }

        if(cnt==3 && mousePressed == true)
        {
            blue->g_body->SetTransform(mousePosition,blue->g_body->GetAngle());
        }

        /*
        if(mousePressed)
        {
            b2MouseJointDef mousejointDef;
            mousejointDef.bodyA = itemList.at(1)->g_body;
            mousejointDef.bodyB = mushroom->g_body;
            mousejointDef.collideConnected = true;
            mousejointDef.maxForce = 400*mushroom->g_body->GetMass();
            mousejointDef.dampingRatio = 0;
            world->CreateJoint(&mousejointDef);
        }
        */

        //std::cout << "Move !" << std::endl ;
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        /* TODO : add your code here */
        mousePressed = false;
        cout << "Release !" << endl;
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Z)
    {
        delete mushroom;
        mushroom = new Bird(2.4,12,0.27f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/9.0,height()/9.0),world,scene);
    }


    if(event->key() == Qt::Key_X)
    {
        delete goomba;
        goomba = new FloppyBird(2.4,12,0.27f,&timer,QPixmap(":/image/goo.png").scaled(height()/9.0,height()/9.0),world,scene);
    }

    if(event->key() == Qt::Key_C)
    {
        delete blue;
        blue = new SprintBird(2.4,12,0.27f,&timer,QPixmap(":/image/blue_bird").scaled(height()/9.0,height()/9.0),world,scene);
    }

}
