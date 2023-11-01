#include <QApplication>
#include <QGraphicsScene>
#include <QEventloop>
#include <QPaintEvent>
#include <QPen>
#include <QTimer>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QDebug>

#include "board.h"

Board::Board(QWidget* parent): 
   m_parent(parent) {
   m_graphicsScene = new QGraphicsScene(this);
   setScene(m_graphicsScene);

   m_animation_timer = new QTimer(this);
   connect(m_animation_timer,SIGNAL(timeout()),this,SLOT(animation_timeout()));
   m_animation_timer->start(1);

   addTile();
}

// Returns the number of free slots.
int Board::freeSlots() {
   int free_slot_count = 0;

   for (int i = 0; i < 16; i++)
      if (0 == m_tiles[m_order_forward[i].px][m_order_forward[i].py].value)
         free_slot_count++;

   return free_slot_count;
}

// Add a random tile to the board. Possible values 2 or 4.
int Board::addTile() {

   // count free slots
   int free_slot_count = 0; 
   int free_slots[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
   int last_slot = -1;

   for (int i = 0; i < 16; i++) {
      if (0 == m_tiles[m_order_forward[i].px][m_order_forward[i].py].value) {
         last_slot = i;
         free_slots[free_slot_count] = i;
         free_slot_count++;
      }
   }

   if (!free_slot_count)
      return 0;

   int pos = (1 == free_slot_count) ? last_slot : free_slots[QRandomGenerator::global()->bounded(free_slot_count - 1)];
   int val = QRandomGenerator::global()->bounded(4) < 2 ? 2 : 4;

   m_tiles[m_order_forward[pos].px][m_order_forward[pos].py].value = val;
   return free_slot_count - 1;
}


void Board::updateTile(int lx, int ly) {
   m_tiles[lx][ly].ismoving = false;
   

   int x = m_tiles[lx][ly].ltarget.px;
   int y = m_tiles[lx][ly].ltarget.py;
   
   // clear target tile but preserve its value
   int value = m_tiles[x][y].value;
   m_tiles[x][y].clear();
   m_tiles[x][y].value = value;

   // update source of target tile
   m_tiles[x][y].lsource.px = lx;
   m_tiles[x][y].lsource.py = ly;

   // update score
   m_score += m_tiles[lx][ly].value;
   emit newScore(m_score);

   // update value of target tile
   if ((m_tiles[x][y].value == 0) || (m_tiles[x][y].value == m_tiles[lx][ly].value))
      m_tiles[x][y].value += m_tiles[lx][ly].value;

   // reset source tile   
   m_tiles[lx][ly].value = 0;
}

void Board::animation_timeout() {
   bool redraw = false;

   for (int ly = 0; ly < 4; ly++)
      for (int lx = 0; lx < 4; lx++) {

         // if tile arrived it should not move anymore
         if (m_tiles[lx][ly].psource.px == m_tiles[lx][ly].ptarget.px)
            if (m_tiles[lx][ly].psource.py == m_tiles[lx][ly].ptarget.py)
               m_tiles[lx][ly].ismoving = false;

         // if tile is not moving we don't do anything with it
         if (!m_tiles[lx][ly].ismoving)
            continue;

         // if tile is moving to the right, increment the x component of the tile's pixel position
         if (m_tiles[lx][ly].psource.px < m_tiles[lx][ly].ptarget.px) {
            m_tiles[lx][ly].pcurrent.px+=m_animation_speed;
            redraw = true;
            if (m_tiles[lx][ly].pcurrent.px >= m_tiles[lx][ly].ptarget.px) {
               updateTile(lx,ly);
            }
         } 

         // if tile is moving to the left, decrement the x component of the tile's pixel position
         if (m_tiles[lx][ly].psource.px > m_tiles[lx][ly].ptarget.px) {
            m_tiles[lx][ly].pcurrent.px-= m_animation_speed;
            redraw = true;
            if (m_tiles[lx][ly].pcurrent.px <= m_tiles[lx][ly].ptarget.px) {
               updateTile(lx, ly);
            }
         }

         // if tile is moving downwards, increment the y component of the tile's pixel position
         if (m_tiles[lx][ly].psource.py < m_tiles[lx][ly].ptarget.py) {
            m_tiles[lx][ly].pcurrent.py+= m_animation_speed;
            redraw = true;
            if (m_tiles[lx][ly].pcurrent.py >= m_tiles[lx][ly].ptarget.py) {
               updateTile(lx, ly);
            }
         }

         // if tile is moving upwards, decrement the y component of the tile's pixel position
         if (m_tiles[lx][ly].psource.py > m_tiles[lx][ly].ptarget.py) {
            m_tiles[lx][ly].pcurrent.py-= m_animation_speed;
            redraw = true;
            if (m_tiles[lx][ly].pcurrent.py <= m_tiles[lx][ly].ptarget.py) {
               updateTile(lx, ly);
            }
         }
      }
   
   if (redraw)
      viewport()->update();
}

void Board::mousePressEvent(QMouseEvent* evt) {
   QApplication::sendEvent(m_parent,evt);
}

void Board::mouseMoveEvent(QMouseEvent* evt) {
   QApplication::sendEvent(m_parent, evt);
}

void Board::drawForeground(QPainter* painter, const QRectF& rect) {
   QGraphicsView::drawForeground(painter, rect);

   qreal w4 = rect.width() / 4;
   qreal h4 = rect.height() / 4;

   for (int ly = 0; ly < 4; ly++)
      for (int lx = 0; lx < 4; lx++) {

         int px = rect.x() + qreal(w4) * lx;
         int py = rect.y() + qreal(h4) * ly;
         m_pos[lx][ly].px = px;
         m_pos[lx][ly].py = py;

         if (m_tiles[lx][ly].value) {

            if (m_tiles[lx][ly].ismoving) {
               px = m_tiles[lx][ly].pcurrent.px;
               py = m_tiles[lx][ly].pcurrent.py;
            }

            int val = m_tiles[lx][ly].value;
            if (val > 4096)
               val = 4096;

            QColor bg = m_bg_map[val];
            QColor fg = m_fg_map[val];

            // draw tile background
            painter->fillRect(px, py, w4, h4, bg);
            painter->setPen(QPen(QColor(187, 173, 160), 15));
            painter->drawRoundedRect(px, py, w4, h4, 15, 15);

            // draw number value
            painter->setPen(QPen(fg, 4));
            QFont font = painter->font();
            font.setPixelSize(w4 * 0.3);
            font.setBold(true);
            painter->setFont(font);

            QRect r = { px, py, int(w4), int(h4) };
            painter->drawText(r, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(m_tiles[lx][ly].value), &r);
         }
      }
}

void Board::drawBackground(QPainter* painter, const QRectF& rect) {
   QGraphicsView::drawBackground(painter, rect);

   // draw grid
   qreal w4 = rect.width() / 4;
   qreal h4 = rect.height() / 4;

   for (int i = 0, x = 0, y = 0; i < 16; i++) {
      int rx = rect.x() + w4 * x;
      int ry = rect.y() + h4 * y;

      painter->fillRect(rx, ry, w4, h4, QColor(205, 193, 180));

      painter->setPen(QPen(QColor(187, 173, 160), 15));
      painter->drawRect(rx, ry, w4, h4);

      x++;
      if (x >= 4)
         x = 0, y++;
   }
}

// Returns true if the tile can be moved from logical coordinates [sx,sy] to 
// [tx,ty], otherwise it returns false.
bool Board::canMove(int sx, int sy, int tx, int ty) {
   if (sx < 0 || sx > 3 || sy < 0 || sy > 3 || tx < 0 || tx > 3 || ty < 0 || ty > 3)
      return false;

   if (sx == tx && sy == ty)
      return false;

   if (m_tiles[sx][sy].value <= 0)
      return false;

   if ((m_tiles[tx][ty].value > 0) && (m_tiles[sx][sy].value != m_tiles[tx][ty].value))
      return false;

   return true;
}

// It sets up the animation parameters for a a tile to be moved
// from logical coordinates [sx,sy] to [tx,ty].
bool Board::moveTile(int sx, int sy, int tx, int ty) {

   if (!canMove(sx, sy, tx, ty))
      return false;

   m_tiles[sx][sy].lsource.px = sx;
   m_tiles[sx][sy].lsource.py = sy;
   m_tiles[sx][sy].ltarget.px = tx;
   m_tiles[sx][sy].ltarget.py = ty;

   m_tiles[sx][sy].psource.px = m_pos[sx][sy].px;
   m_tiles[sx][sy].psource.py = m_pos[sx][sy].py;
   m_tiles[sx][sy].ptarget.px = m_pos[tx][ty].px;
   m_tiles[sx][sy].ptarget.py = m_pos[tx][ty].py;

   m_tiles[sx][sy].pcurrent.px = m_tiles[sx][sy].psource.px;
   m_tiles[sx][sy].pcurrent.py = m_tiles[sx][sy].psource.py;
   m_tiles[sx][sy].ismoving = true;

   qDebug() << "move (" << sx << "," << sy << ") to (" << tx << "," << ty << ")";

   while (isMoving())
      qApp->processEvents();
   
   return true;
}

// Returns true is at least one tile is moving, otherwise it returns false.
bool Board::isMoving() {

   for (int ly = 0; ly < 4; ly++)
      for (int lx = 0; lx < 4; lx++)
         if (m_tiles[lx][ly].ismoving)
            return true;

   return false;
}

// It checks for game over condition: if there are no free slots left, and 
// none of the tiles can be merged then it is game over
void Board::checkGameOver() {
   bool gameover = true;

   if (freeSlots() > 0)
      gameover = false; // there are free slots left
   else {
      for (int i = 0; i < 16; i++) {
         int lx = m_order_forward[i].px;
         int ly = m_order_forward[i].py;

         if (((lx < 3) && (m_tiles[lx][ly].value == m_tiles[lx + 1][ly].value)) ||
            ((lx > 0) && (m_tiles[lx][ly].value == m_tiles[lx - 1][ly].value)) ||
            ((ly < 3) && (m_tiles[lx][ly].value == m_tiles[lx][ly + 1].value)) ||
            ((ly > 0) && (m_tiles[lx][ly].value == m_tiles[lx][ly - 1].value))) {
            gameover = false; // found mergeable tiles
            break;
         }
      }
   }

   if (gameover) {
      QMessageBox::information(m_parent, "2048", "Game over! Try again.", QMessageBox::StandardButton::Retry);
      newGame();
   }
}

void Board::moveLeft() {
   bool addNew = false;
   
   for (int ly=0;ly<4;ly++) {
      bool merged = false;
      for (int lx = 0; lx < 4; lx++) {

         if (lx < 1)
            continue;

         if (m_tiles[lx][ly].value <= 0)
            continue;

         int tx = lx;
         
         for (int mx=lx-1;mx>-1;mx--)
            if (0 == m_tiles[mx][ly].value)
               tx = mx;


         if (!merged && (tx>0) && (m_tiles[lx][ly].value == m_tiles[tx - 1][ly].value))
            tx--, merged = true;

         if (tx<lx && moveTile(lx, ly, tx, ly))
            addNew = true;
      }
   }

   if (addNew)
      addTile();

   checkGameOver();
}

void Board::moveRight() {
   bool addNew = false;

   for (int ly = 0; ly < 4; ly++) {
      bool merged = false;
      for (int lx = 3; lx > -1; lx--) {

         if (lx > 2)
            continue;

         if (m_tiles[lx][ly].value <= 0)
            continue;

         int tx = lx;

         for (int mx = lx + 1; mx < 4; mx++)
            if (0 == m_tiles[mx][ly].value)
               tx = mx;

         if (!merged && (tx<3) && (m_tiles[lx][ly].value == m_tiles[tx + 1][ly].value))
            tx++, merged = true;
            
         if (tx > lx && moveTile(lx, ly, tx, ly))
            addNew = true;
      }
   }

   if (addNew)
      addTile();

   checkGameOver();
}

void Board::moveUp() {
   bool addNew = false;


   for (int lx = 0; lx < 4; lx++) {
      bool merged = false;
      for (int ly = 0; ly < 4; ly++) {

         if (ly < 1)
            continue;

         if (m_tiles[lx][ly].value <= 0)
            continue;

         int ty = ly;

         for (int my = ly - 1; my > -1; my--)
            if (0 == m_tiles[lx][my].value)
               ty = my;

         if (!merged && (ty>0) && (m_tiles[lx][ly].value == m_tiles[lx][ty - 1].value))
            ty--, merged = true;

         if (ty < ly && moveTile(lx, ly, lx, ty))
            addNew = true;
      }
   }
   
   if (addNew)
      addTile();

   checkGameOver();
}

void Board::moveDown() {
   bool addNew = false;

   for (int lx = 0; lx < 4; lx++) {
      bool merged = false;
      for (int ly = 3; ly > -1; ly--) {

         if (ly > 2)
            continue;

         if (m_tiles[lx][ly].value <= 0)
            continue;

         int ty = ly;

         for (int my = ly + 1; my < 4; my++)
            if (0 == m_tiles[lx][my].value)
               ty = my;

         if (!merged && (ty<3) && (m_tiles[lx][ly].value == m_tiles[lx][ty+1].value))
            ty++, merged = true;

         if (ty > ly && moveTile(lx, ly, lx, ty))
            addNew = true;
      }
   }

   if (addNew)
      addTile();

   checkGameOver();
}

// Reset game state
void Board::newGame() {

   m_score = 0;

   for (int i=0;i<16;i++)
      m_tiles[m_order_forward[i].px][m_order_forward[i].py].clear();

   emit newScore(0);
   addTile();
   viewport()->repaint();
}

