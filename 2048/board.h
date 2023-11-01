#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include <QPoint>
#include <QTimer>
#include <QDebug>
#include "Tile.h"

class Board : 
   public QGraphicsView {
   Q_OBJECT

public:
   Board(QWidget* parent);
   void newGame();
   void moveLeft();
   void moveRight();
   void moveUp();
   void moveDown();

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
   int addTile();
   int freeSlots();
   bool moveTile(int sx, int sy, int tx, int ty);
   bool canMove(int sx, int sy, int tx, int ty);
   void updateTile(int lx, int ly);
   bool isMoving();
   void checkGameOver();

   QGraphicsScene* m_graphicsScene = Q_NULLPTR;
   QWidget* m_parent = Q_NULLPTR;
   QTimer* m_animation_timer = Q_NULLPTR;
   const int m_animation_speed = 20;

   int m_score = 0;
   Tile m_tiles[4][4]; // contains the values and animation data
   Cell m_pos[4][4];   // contains the physical position of tiles on the board


   // define the order in which tiles are processed after an arrow key was pressed
   Cell m_order_up[16] = { {0,0},{0,1},{0,2},{0,3}, {1,0},{1,1},{1,2},{1,3}, {2,0},{2,1},{2,2},{2,3}, {3,0},{3,1},{3,2},{3,3} };
   Cell m_order_down[16] = { {3,3},{3,2},{3,1},{3,0}, {2,3},{2,2},{2,1},{2,0}, {1,3},{1,2},{1,1},{1,0}, {0,3},{0,2},{0,1},{0,0} };
   Cell m_order_left[16] = { {0,0},{1,0},{2,0},{3,0}, {0,1},{1,1},{2,1},{3,1}, {0,2},{1,2},{2,2},{3,2}, {0,3},{1,3},{2,3},{3,3} };
   Cell m_order_right[16] = { {3,3},{2,3},{1,3},{0,3},  {3,2},{2,2},{1,2},{0,2}, {3,1},{2,1},{1,1},{0,1}, {3,0},{2,0},{1,0},{0,0} };

   Cell m_order_forward[16] = { {0,0},{1,0},{2,0},{3,0},{0,1},{1,1},{2,1},{3,1},{0,2},{1,2},{2,2},{3,2},{0,3},{1,3},{2,3},{3,3} };
   Cell m_order_backward[16]  = {{3,3},{2,3},{1,3},{0,3},{3,2},{2,2},{1,2},{0,2},{3,1},{2,1},{1,1},{0,1},{3,0},{2,0},{1,0},{0,0}};

   
   // map values to colors
   QMap<int,QColor>  m_fg_map = {
      { 0, QColor(0x77,0x6e,0x65) }, { 2, QColor(0x77,0x6e,0x65) }, { 4, QColor(0x77,0x6e,0x65) }, { 8,QColor(0xf9,0xf6,0xf2) },
      { 16,QColor(0xf9,0xf6,0xf2) }, { 32,QColor(0xf9,0xf6,0xf2) }, { 64,QColor(0xf9,0xf6,0xf2) }, { 128,QColor(0xf9,0xf6,0xf2) }, 
      { 256,QColor(0xf9,0xf6,0xf2) }, { 512,QColor(0xf9,0xf6,0xf2) }, { 1024,QColor(0xf9,0xf6,0xf2) }, { 2048,QColor(0xf9,0xf6,0xf2) }, 
      { 4096,QColor(0xf9,0xf6,0xf2) } };

   QMap<int, QColor> m_bg_map = {
      { 0, QColor(0xee,0xe4,0xda) }, { 2, QColor(0xee,0xe4,0xda) }, { 4, QColor(0xed,0xe0,0xc8) }, { 8, QColor(0xf2,0xb1,0x79) }, 
      { 16, QColor(0xf5,0x95,0x63) }, { 32, QColor(0xf6,0x7c,0x5f) }, { 64, QColor(0xf6,0x5e,0x3b) }, { 128, QColor(0xed,0xcf,0x72) }, 
      { 256, QColor(0xed,0xcc,0x61) }, { 512, QColor(0xed,0xc8,0x50) }, { 1024, QColor(0xed,0xc5,0x3f) }, { 2048, QColor(0xed,0xc2,0x2e) }, 
      { 4096, QColor(0x3c,0x3a,0x32) }};
};