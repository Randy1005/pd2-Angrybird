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
    sling = new Slingshot(3.5,5.5,2.5,4,QPixmap(":/image/slingshot.png").scaled(width()*0.06,height()*0.3),world,scene);

    //Create bird
    mushroom = new Bird(6,8,0.27f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/8.0,height()/8.0),world,scene);
    goomba = new FloppyBird(7,8,0.27f,&timer,QPixmap(":/image/goo.png").scaled(height()/8.0,height()/8.0),world,scene);
    kirby = new SprintBird(8,8,0.27f,&timer,QPixmap(":/image/kirby.png").scaled(height()/7.0,height()/7.0),world,scene);
    turtle = new StrikeBird(9,8,0.27f,&timer,QPixmap(":/image/turtle.png").scaled(height()/8.0,height()/8.0),world,scene);
    fist = new Bird(18,40,0.5f,&timer,QPixmap(":/image/fist.png").scaled(height()*0.5,height()*0.5),world,scene);
    fist->g_body->SetAwake(false); //deactivate the fist at first, wait for strike bird


    //Create barrier
    wood = new Barrier(24,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    wood2 = new Barrier(18,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    hori_wood = new Barrier(19,16,5,1,&timer,QPixmap(":/image/barrier_hori.png").scaled(height()/2.0,height()/2.8),world,scene);

    //Create enemy
    lbj = new Bird(21,6,0.55f,&timer,QPixmap(":/image/LBJ.png").scaled(height()/8.0,height()/6.0),world,scene);

    //Store birds
    BirdList.push_back(mushroom);
    BirdList.push_back(goomba);
    BirdList.push_back(kirby);
    BirdList.push_back(turtle);

    //Store barriers and enemy
    itemList.push_back(wood);
    itemList.push_back(wood2);
    itemList.push_back(hori_wood);
    itemList.push_back(lbj);
    itemList.push_back(fist);

    //Reset button
    QPushButton *resetBtn = new QPushButton("RESET");
    resetBtn->setFont(QFont("Courier",16,QFont::Bold));
    resetBtn->setGeometry(100,0,70,40);
    scene->addWidget(resetBtn);

    //Quit button
    QPushButton *quitBtn = new QPushButton("QUIT");
    quitBtn->setFont(QFont("Courier",16,QFont::Bold));
    quitBtn->setGeometry(0,0,70,40);
    scene->addWidget(quitBtn);

    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));

    //button connect
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(closeGame()));
    connect(resetBtn,SIGNAL(clicked()),this,SLOT(resetGame()));

    timer.start(100/6);
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    /*to avoid counting mouseClicked when clicking reset*/
    if(event->type() == QEvent::MouseButtonPress)
    {

        if(mousePosition.x<=5 && mousePosition.x>=2)
        {
            if(mousePosition.y<=19 && mousePosition.y>=18)
            {
                mousePressed = false;
            }
            else
                mousePressed = true;
        }
        else
        {
            mousePressed = true;
        }


        click++;
        if(mousePressed == true && click >= 5)
        {
            cnt++;
            click = 0;
        }
        cout<<cnt<<endl;
    }
    if(event->type() == QEvent::MouseMove)
    {
        float x = QCursor::pos().x() * 32/960 - 7.0;
        float y = 22.0 - QCursor::pos().y() * 18/540;
        mousePosition = b2Vec2(x,y);
        if(cnt==1 && mousePressed == true)
        {
            mushroom->g_body->SetTransform(mousePosition, mushroom->g_body->GetAngle());
        }

        if(cnt==2 && mousePressed == true)
        {
            goomba->g_body->SetTransform(mousePosition, goomba->g_body->GetAngle());
        }

        if(cnt==3 && mousePressed == true)
        {
            kirby->g_body->SetTransform(mousePosition, kirby->g_body->GetAngle());
        }

        if(cnt==4 && mousePressed == true)
        {
            turtle->g_body->SetTransform(mousePosition, turtle->g_body->GetAngle());
        }

        /*debug*/
        //std::cout << x <<" "<< y <<std::endl ;
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        /* TODO : add your code here */
        mousePressed = false;
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

void MainWindow::closeGame()
{
    this->close();
}

void MainWindow::resetGame()
{
    BirdList.clear();
    itemList.clear();

    delete mushroom;
    delete goomba;
    delete kirby;
    delete turtle;
    delete fist;
    delete wood;
    delete wood2;
    delete hori_wood;
    delete lbj;
    resetBirds();
    resetBarriers_n_Enemy();

    cnt = 0;
    mousePressed = false;
}

void MainWindow::QUITSLOT()
{
    cout<<"quitGame signal received"<<endl;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    /*spawn different birds on slingshot*/

    if(event->key() == Qt::Key_Z)
    {
        delete mushroom;
        mushroom = new Bird(2.4,12,0.8f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/8.0,height()/8.0),world,scene);
    }


    if(event->key() == Qt::Key_X)
    {
        delete goomba;
        goomba = new FloppyBird(2.4,12,0.8f,&timer,QPixmap(":/image/goo.png").scaled(height()/8.0,height()/8.0),world,scene);
    }

    if(event->key() == Qt::Key_C)
    {
        delete kirby;
        kirby = new SprintBird(2.4,12,0.8f,&timer,QPixmap(":/image/kirby.png").scaled(height()/8.0,height()/8.0),world,scene);
    }

    if(event->key() == Qt::Key_V)
    {
        delete turtle;
        turtle = new StrikeBird(2.4,12,0.8f,&timer,QPixmap(":/image/turtle.png").scaled(height()/8.0,height()/8.0),world,scene);
    }

    if(event->key() == Qt::Key_Space)
    {
        if(cnt==2)
        {
            goomba->activateAbility = true;
            goomba->birdFunction();
        }

        if(cnt==3)
        {
            kirby->activateAbility = true;
            goomba->activateAbility = false;
            kirby->birdFunction();
        }

        if(cnt==4)
        {
            turtle->activateAbility = true;
            kirby->activateAbility = false;
            turtle->birdFunction();
            fist->g_body->SetAwake(true);
            fist->setLinearVelocity(b2Vec2(0,-50));
        }
    }

}

void MainWindow::resetBirds()
{
    //Create bird
    mushroom = new Bird(6,8,0.27f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/8.0,height()/8.0),world,scene);
    goomba = new FloppyBird(7,8,0.27f,&timer,QPixmap(":/image/goo.png").scaled(height()/8.0,height()/8.0),world,scene);
    kirby = new SprintBird(8,8,0.27f,&timer,QPixmap(":/image/kirby.png").scaled(height()/7.0,height()/7.0),world,scene);
    turtle = new StrikeBird(9,8,0.27f,&timer,QPixmap(":/image/turtle.png").scaled(height()/8.0,height()/8.0),world,scene);
    fist = new Bird(18,40,0.6f,&timer,QPixmap(":/image/fist.png").scaled(height()*0.75,height()*0.75),world,scene);
    fist->g_body->SetAwake(false); //deactivate the fist at first, wait for strike bird

}

void MainWindow::resetBarriers_n_Enemy()
{
    //Create barrier
    wood = new Barrier(24,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    wood2 = new Barrier(18,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    hori_wood = new Barrier(19,16,5,1,&timer,QPixmap(":/image/barrier_hori.png").scaled(height()/2.0,height()/2.8),world,scene);

    //Create enemy
    lbj = new Bird(21,6,0.55f,&timer,QPixmap(":/image/LBJ.png").scaled(height()/8.0,height()/6.0),world,scene);
}
