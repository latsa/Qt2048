#include "UserInterface.h"
#include "SearchTree.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QString>
#include <QDebug>

UserInterface::UserInterface(QWidget *parent)
   : QMainWindow(parent) {
   ui.setupUi(this);
   setWindowIcon(QIcon(":/Icons/AppIcon"));

   m_solver = new QTimer(this);
   connect(m_solver,SIGNAL(timeout()), this,SLOT(nextMove()));
   m_solver->start(100);

   m_dlgAbout = new DlgAbout(this);
   connect(ui.pbNewGame,SIGNAL(clicked()), this, SLOT(newGame()));
   connect(ui.pbAbout, SIGNAL(clicked()), this, SLOT(about()));
   connect(ui.pbSettings, SIGNAL(clicked()), this, SLOT(settings()));
   connect(ui.pbSolve, SIGNAL(clicked()), this, SLOT(solve()));
   connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(cancel()));
   connect(ui.pbUndo, SIGNAL(clicked()), this, SLOT(undo()));
   connect(ui.pbExit, SIGNAL(clicked()), this, SLOT(exit()));
   connect(ui.gvBoard, SIGNAL(newScore(int, int)), this, SLOT(score(int, int)));

   // Set board size based on the screen size. Make sure the playing area is square.
   
   QDesktopWidget desk;
   int a = std::min(desk.width(), desk.height()) * 0.8;
   int s = a * 0.7;
   setFixedSize(QSize(a, a * 0.925));
   ui.gvBoard->setFixedSize(QSize(s, s));
   ui.frame->setFixedSize(QSize(s,90));

   ui.lblBestValue->setText(QString::number(ui.gvBoard->getBest()));

   ui.pbCancel->hide();
   ui.pbSettings->hide();
}

UserInterface::~UserInterface() {
}

Board* UserInterface::getBoard() {
   return ui.gvBoard;
}

//
// Keyboard event handlers
//

void UserInterface::keyPressEvent(QKeyEvent* event) {
   bool gameover = false;

   if (event->key() == Qt::Key_Escape) {
      exit();
   } else if (!m_solving) {
      if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
         if (!m_paused)
            gameover = ui.gvBoard->moveLeft();
      }
      else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
         if (!m_paused)
            gameover = ui.gvBoard->moveRight();
      }
      else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
         if (!m_paused)
            gameover = ui.gvBoard->moveUp();
      }
      else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
         if (!m_paused)
            gameover = ui.gvBoard->moveDown();
      }
   }

   ui.gvBoard->viewport()->repaint();
}

//
// Mouse event handlers
//

void UserInterface::mousePressEvent(QMouseEvent* evt) {
   m_mouse_oldPos = evt->globalPos();
   m_mouse_dragging = true;
}

void UserInterface::mouseReleaseEvent(QMouseEvent* evt) {
   m_mouse_dragging = false;
}

void UserInterface::mouseMoveEvent(QMouseEvent* evt) {
   if (!m_mouse_dragging)
      return;

   const QPoint delta = evt->globalPos() - m_mouse_oldPos;
   move(x() + delta.x(), y() + delta.y());
   m_mouse_oldPos = evt->globalPos();
}

//
// Pushbutton handlers
//

void UserInterface::newGame() {
   cancel();
   ui.gvBoard->newGame();
}

void UserInterface::undo() {
   ui.gvBoard->undo();
}

void UserInterface::about() {
   //QMessageBox::aboutQt(this,tr("About Qt.."));
   if (!m_dlgAbout)
      return;

   m_dlgAbout->show();
}

void UserInterface::exit() {
   ui.gvBoard->exit();
}

void UserInterface::settings() {
   QMessageBox::information(this, tr("Q2048 Settings"), tr("This feature is not yet implemented."), tr("OK"));
}

void UserInterface::solve() {
   if (m_solving) {
      m_solving = false;
      m_paused = true;
      ui.pbSolve->setText(tr("Continue"));
      //..
   }
   else {
      m_solving = true;
      m_paused = false;
      ui.pbCancel->show();
      ui.pbSolve->setText(tr("Pause"));
   }
}

void UserInterface::cancel() {
   m_solving = false;
   m_paused = false;
   ui.pbCancel->hide();
   ui.pbSolve->setText(tr("Solve!"));
}

//
// Update score display and keep track of best score
//

void UserInterface::score(int v, int b) {
   ui.lblScoreValue->setText(QString::number(v));
   ui.lblBestValue->setText(QString::number(b));
}

//
// Calculate next move automatically
//

void UserInterface::nextMove() {

   if (!m_solving)
      return;

   State state = ui.gvBoard->getState();

   SearchTree searchTree(state, 10);

   switch (searchTree.getNextMove()) {
   case MOVE_LEFT:
      ui.gvBoard->moveLeft();
      break;
   case MOVE_RIGHT:
      ui.gvBoard->moveRight();
      break;
   case MOVE_UP:
      ui.gvBoard->moveUp();
      break;
   case MOVE_DOWN:
      ui.gvBoard->moveDown();
      break;
   }
}