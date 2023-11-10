#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include <QVector>
#include <QPoint>
#include <QTimer>
#include <QDebug>
#include "State.h"

class Board : 
   public QGraphicsView,
   public State {
   Q_OBJECT

public:
   Board(QWidget* parent);
   void newGame();
   static void newGame(Tile(*tiles)[4][4], int& score);
   bool moveLeft();
   bool moveRight();
   bool moveUp();
   bool moveDown();
   QVector<int> getTileValues();
   int getScore();

   static bool moveLeft(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score);
   static bool moveRight(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score);
   static bool moveUp(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score);
   static bool moveDown(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score);
   State getState();

protected:
   void drawForeground(QPainter* painter, const QRectF& rect) override;
   void drawBackground(QPainter* painter, const QRectF& rect) override;
   void mousePressEvent(QMouseEvent* evt) override;
   void mouseMoveEvent(QMouseEvent* evt) override;

signals:
   void newScore(int val);

private slots:
   void animation_timeout();

private:
   void updateTile(int lx, int ly);
   static int addTile(Tile(*tiles)[4][4]);
   static int freeSlots(Tile(*tiles)[4][4]);
   static bool moveTile(Tile(*tiles)[4][4], Cell(*pos)[4][4], int sx, int sy, int tx, int ty);
   static bool canMove(Tile(*tiles)[4][4], int sx, int sy, int tx, int ty);
   static bool isMoving(Tile(*tiles)[4][4]);
   static bool checkGameOver(Tile(*tiles)[4][4], int& score);

   QGraphicsScene* m_graphicsScene = Q_NULLPTR;
   QWidget* m_parent = Q_NULLPTR;
   QTimer* m_animation_timer = Q_NULLPTR;
   const int m_animation_speed = 20;
};