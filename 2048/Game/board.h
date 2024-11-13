#pragma once
#include <QSettings>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include <QVector>
#include <QStack>
#include <QPoint>
#include <QTimer>
#include <QDebug>
#include "State.h"
#include "cell.h"

class Board : 
   public QGraphicsView {
   Q_OBJECT

public:
   Board(QWidget* parent);
   void newGame();
   void newGame(Tile(*tiles)[4][4], int& score);
   bool moveLeft();
   bool moveRight();
   bool moveUp();
   bool moveDown();
   QVector<int> getTileValues();
   int getScore();
   int getBest();
   void undo();
   void exit();

   bool moveLeft(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score, bool wait = true);
   bool moveRight(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score, bool wait = true);
   bool moveUp(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score, bool wait = true);
   bool moveDown(Tile(*tiles)[4][4], Cell(*pos)[4][4], int& score, bool wait = true);
   State getState();

protected:
   void drawForeground(QPainter* painter, const QRectF& rect) override;
   void drawBackground(QPainter* painter, const QRectF& rect) override;
   void mousePressEvent(QMouseEvent* evt) override;
   void mouseMoveEvent(QMouseEvent* evt) override;

signals:
   void newScore(int val, int best);

private slots:
   void animation_timeout();

private:
   void updateTile(int lx, int ly);
   int addTile(Tile(*tiles)[4][4]);
   int freeSlots(Tile(*tiles)[4][4]);
   static bool moveTile(Tile(*tiles)[4][4], Cell(*pos)[4][4], int sx, int sy, int tx, int ty, bool wait = true);
   static bool canMove(Tile(*tiles)[4][4], int sx, int sy, int tx, int ty);
   static bool isMoving(Tile(*tiles)[4][4]);
   bool checkGameOver(Tile(*tiles)[4][4], int& score);

   QGraphicsScene* m_graphicsScene = Q_NULLPTR;
   QWidget* m_parent = Q_NULLPTR;
   QTimer* m_animation_timer = Q_NULLPTR;
   const int m_animation_speed = 20;

   QSettings m_settings;

   State m_state;
   QStack<State> m_state_stack;

   int m_best = 0; // best game score
};