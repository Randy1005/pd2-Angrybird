#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>

#include <gameitem.h>
#include <land.h>
#include <slingshot.h>
#include <bird.h>
#include <belt.h>
#include <barrier.h>
#include <floppybird.h>
#include <sprintbird.h>
#include <strikebird.h>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *,QEvent *event);
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *event);
    void resetBirds();
    void resetBarriers_n_Enemy();
    void scoreIncrement();

    //Birds
    Bird *mushroom;
    FloppyBird *goomba;
    SprintBird *kirby;
    StrikeBird *turtle;
    Bird *fist;
    QList<GameItem *> BirdList;

    //items
    /*I consider taking off the slingshot first, replace it by a single point, to avoid
     * the problem of getting blocked by the body of sling*/
    //Slingshot *sling;
    Barrier *wood;
    Barrier *wood2;
    Barrier *hori_wood;
    Bird *lbj;

    float x_cor;
    float y_cor;
    b2Vec2 mousePosition;

    float basePt_x;
    float basePt_y;
    b2Vec2 basePosition;

    b2Vec2 forceVector;

    bool mousePressed = false;
    bool scoreStart = false;
    //Score
    QLabel *score;
    int scoreNum = 0;

    //check if it's connected, or else the speed keeps increasing
    bool isConnected;

    int click = 0; //mouse click time capture, when I click once it runs 5 times
    int cnt = 0; //to record the 'real' mouse click time


signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    void QUITSLOT(); // For debug slot
    void resetGame();
    void closeGame();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QTimer timer;
};

#endif // MAINWINDOW_H
