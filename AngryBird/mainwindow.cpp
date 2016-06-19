#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

    //set background
    QImage bg;
    bg.load(":/image/background.jpg");
    bg = bg.scaled(960,540);
    scene->setBackgroundBrush(bg);

    //bgm
    bgm = new QMediaPlayer();
    bgm->setMedia(QUrl("qrc:/sound/Super Mario Bros. Music - Ground Theme (1).1.mp3"));
    bgm->setVolume(50);
    bgm->play();

    //Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));

    //Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());

    //Create ground
    itemList.push_back(new Land(16,1.5,32,3,QPixmap(":/image/ground.png").scaled(width()*2,height()/5.0),world,scene));

    //add slingshot image (just image, no box2D body)
    QGraphicsPixmapItem *slingShot = new QGraphicsPixmapItem();
    QPixmap slingPic;
    slingPic.load(":/image/slingshot.png");
    slingPic = slingPic.scaled(slingPic.width()*0.07, slingPic.height()*0.07, Qt::KeepAspectRatio);
    slingShot->setPixmap(slingPic);
    slingShot->setPos(170,300);
    scene->addItem(slingShot);

    //Create slingshot(removed tempararily)
    //sling = new Slingshot(3.5,5.5,2.5,4,QPixmap(":/image/slingshot.png").scaled(width()*0.06,height()*0.3),world,scene);

    /*Replace slingshot with a base point(with respect to the final cursor point we dragged to)
     * and try to use ApplyForce() on the boody (requires b2Vec2(force))
     */
    //add base point(reference point), to indicate direction
    QPen pen;
    scene->addEllipse(200,300,10,10,pen,QBrush(Qt::red));

    //Create bird
    mushroom = new Bird(6,8,0.5f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/8.0,height()/8.0),world,scene);
    goomba = new FloppyBird(7,8,0.5f,&timer,QPixmap(":/image/goo.png").scaled(height()/8.0,height()/8.0),world,scene);
    kirby = new SprintBird(8,8,0.5f,&timer,QPixmap(":/image/kirby.png").scaled(height()/7.0,height()/7.0),world,scene);
    turtle = new StrikeBird(9,8,0.5f,&timer,QPixmap(":/image/turtle.png").scaled(height()/8.0,height()/8.0),world,scene);
    fist = new Bird(18,40,0.9f,&timer,QPixmap(":/image/fist.png").scaled(height()*0.5,height()*0.5),world,scene);
    fist->g_body->SetAwake(false); //deactivate the fist at first, wait for strike bird


    //Create barrier
    wood = new Barrier(24,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    wood2 = new Barrier(18,6,1,5,&timer,QPixmap(":/image/barrier.png").scaled(height()/10.0,height()/2.0),world,scene);
    hori_wood = new Barrier(19,16,5,1,&timer,QPixmap(":/image/barrier_hori.png").scaled(height()/2.0,height()/2.8),world,scene);

    //Create enemy
    lbj = new Bird(21,6,1.0f,&timer,QPixmap(":/image/LBJ.png").scaled(height()/6.0,height()/5.0),world,scene);

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

    //scoreboard
    QLabel *scoreboard = new QLabel("SCORE:");
    scoreboard->setFont(QFont("Courier",20,QFont::Bold));
    scoreboard->setGeometry(200,0,100,40);
    scene->addWidget(scoreboard);

    //score
    score = new QLabel(QString::number(scoreNum));
    score->setFont(QFont("Courier",20,QFont::Bold));
    score->setGeometry(330,0,200,40);
    scene->addWidget(score);

    //if(!isConnected)
    //{
        // Timer and calculate score
        connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));

    //}
    timer.start(100/6);

    //button connect
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(closeGame()));
    connect(resetBtn,SIGNAL(clicked()),this,SLOT(resetGame()));


}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        /*to avoid counting mouseClicked when clicking buttons*/
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
        //and turn off the gravity first, or else the bird keeps falling off


    }

    if(event->type() == QEvent::MouseMove)
    {
        /*return cursor coordinate for mouse drag*/
        x_cor = QCursor::pos().x() * 32/960 - 7.0;
        y_cor = 22.0 - QCursor::pos().y() * 18/540;
        mousePosition = b2Vec2(x_cor,y_cor);

        /*store the base point*/
        basePt_x = 6.0;
        basePt_y = 9.0;
        basePosition = b2Vec2(basePt_x,basePt_y);

        /*calculate the vector of (basePosition - mousePosition) to get forceVector*/
        float k = 900; //spring constant
        forceVector = k*(basePosition - mousePosition);


        //then apply force to each body
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
        //std::cout <<"cursor: "<< x_cor <<" "<< y_cor <<std::endl ;
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
        mousePressed = false; //set flag back to false

        //then apply force to each body
        if(cnt==1)
        {
            //applyforce
            mushroom->g_body->ApplyForceToCenter(forceVector,true);
        }

        if(cnt==2)
        {
            //apply force
            goomba->g_body->ApplyForceToCenter(forceVector,true);
        }

        if(cnt==3)
        {
            //apply force
            kirby->g_body->ApplyForceToCenter(forceVector,true);
        }

        if(cnt==4)
        {
            //apply force
            turtle->g_body->ApplyForceToCenter(forceVector,true);
        }
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
    /*to avoid score incrementing when we first set up the scene(enemy already has speed),we set up a flag*/
    /*start score incrementing after I clicked for the first time*/

    if(cnt==1)
    {
        scoreStart = true; //When I clicked for the first time, activate score calculate
    }

    /*if the enemy "lbj" rotates(angular velocity !=0) or moves(linear velocity !=0) then we add up the score*/
    if((lbj->g_body->GetAngularVelocity() != 0 || lbj->g_body->GetLinearVelocity().x != 0 || lbj->g_body->GetLinearVelocity().y != 0) && scoreStart && enemyExist)
    {
        scoreIncrement();
    }

    world->Step(1.0/60.0,6,2);
    scene->update();
}

void MainWindow::closeGame()
{
    this->close();
}

void MainWindow::resetGame()
{
    /*reset all the objects and variables*/
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
    delete bgm;
    resetBirds();
    resetBarriers_n_Enemy();

    cnt = 0;
    scoreNum = 0;
    score->setText(QString::number(scoreNum));
    mousePressed = false;
    scoreStart = false;

}

void MainWindow::QUITSLOT()
{
    cout<<"quitGame signal received"<<endl;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    /*spawn different birds on slingshot*/
    /*
    if(event->key() == Qt::Key_Z)
    {
        delete mushroom;
        mushroom = new Bird(2.4,12,1.0f,&timer,QPixmap(":/image/mushroom.png").scaled(height()/8.0,height()/8.0),world,scene);
    }


    if(event->key() == Qt::Key_X)
    {
        delete goomba;
        goomba = new FloppyBird(2.4,12,1.0f,&timer,QPixmap(":/image/goo.png").scaled(height()/8.0,height()/8.0),world,scene);
    }

    if(event->key() == Qt::Key_C)
    {
        delete kirby;
        kirby = new SprintBird(2.4,12,1.0f,&timer,QPixmap(":/image/kirby.png").scaled(height()/8.0,height()/8.0),world,scene);
    }

    if(event->key() == Qt::Key_V)
    {
        delete turtle;
        turtle = new StrikeBird(2.4,12,1.0f,&timer,QPixmap(":/image/turtle.png").scaled(height()/8.0,height()/8.0),world,scene);
    }
    */

    /*activate abilities(birdFunction)*/

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

    //bgm
    bgm = new QMediaPlayer();
    bgm->setMedia(QUrl("qrc:/sound/Super Mario Bros. Music - Ground Theme (1).1.mp3"));
    bgm->setVolume(70);
    bgm->play();
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


void MainWindow::scoreIncrement()
{
    scoreNum += 1;
    //set a boundary score to eliminate enemy
    if(scoreNum >= 600)
    {
        score->setText("YOU WIN!!");
        score->setFont(QFont("Courier",24,QFont::Black));
        //enemyExist = false; //next we delete the enemy, change the bool value first
        //delete lbj;
    }
    else
        score->setText(QString::number(scoreNum));
}
