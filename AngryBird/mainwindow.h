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

    //items
    Bird *mushroom;
    FloppyBird *goomba;
    SprintBird *blue;
    QList<GameItem *> BirdList;
    Barrier *wood;
    Barrier *wood2;
    Barrier *hori_wood;
    Bird *lbj;
    b2Vec2 mousePosition;
    bool mousePressed;


    int click = 0; //mouse click time capture, when I click once it runs 5 times
    int cnt = 0; //to record the 'real' mouse click time

    bool firstFired = false;
    bool secondFired = false;


signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    // For debug slot
    void QUITSLOT();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QTimer timer;
};

#endif // MAINWINDOW_H
