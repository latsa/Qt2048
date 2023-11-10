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
   connect(ui.gvBoard, SIGNAL(newScore(int)), this, SLOT(score(int)));

   // Set board size based on the screen size. Make sure the playing area is square.
   QDesktopWidget desk;
   int a = std::min(desk.width(), desk.height()) * 0.8;
   int s = a * 0.813;
   setFixedSize(QSize(a * 0.84, a * 1.02));
   ui.gvBoard->setFixedSize(QSize(s, s));
   m_best = m_settings.value("highscore",0).toInt();
   ui.lblBestValue->setText(QString::number(m_best));

   ui.pbCancel->hide();
   ui.pbSettings->hide();
}

UserInterface::~UserInterface() {
}

Board* UserInterface::getBoard() {
   return ui.gvBoard;
}

void UserInterface::keyPressEvent(QKeyEvent* event) {
   bool gameover = false;

   if (event->key() == Qt::Key_Escape) {
      m_settings.setValue("highscore", m_best);
      QApplication::quit();
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

void UserInterface::newGame() {
   cancel();
   ui.gvBoard->newGame();
}

void UserInterface::about() {
   //QMessageBox::aboutQt(this,tr("About Qt.."));
   if (!m_dlgAbout)
      return;

   m_dlgAbout->show();
}

void UserInterface::settings() {
   QMessageBox::information(this,tr("Q2048 Settings"),tr("This feature is not yet implemented."),tr("OK"));
}

void UserInterface::score(int v) {
   m_score = (v>0) ? v : 0;
   ui.lblScoreValue->setText(QString::number(m_score));

   if (m_best < m_score) {
      m_best = m_score;
      ui.lblBestValue->setText(QString::number(m_best));
   }

}

void UserInterface::solve() {
   if (m_solving) {
      m_solving = false;
      m_paused = true;
      ui.pbSolve->setText(tr("Continue"));
      //..
   } else {
      m_solving = true;
      m_paused = false;
      ui.pbCancel->show();
      ui.pbSolve->setText(tr("Pause"));
   }
}

void UserInterface::nextMove() {

   if (!m_solving) 
      return;

   //State state = ui.gvBoard->getState();
   State state = *ui.gvBoard;

   SearchTree searchTree(state, 10);
   
   switch(searchTree.getNextMove()) {
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

void UserInterface::cancel() {
   m_solving = false;
   m_paused = false;
   ui.pbCancel->hide();
   ui.pbSolve->setText(tr("Solve!"));
}
