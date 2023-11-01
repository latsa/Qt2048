#include "UserInterface.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QString>

UserInterface::UserInterface(QWidget *parent)
   : QMainWindow(parent) {
   ui.setupUi(this);
   setWindowIcon(QIcon(":/Icons/AppIcon"));

   m_dlgAbout = new DlgAbout(this);
   connect(ui.pbNewGame,SIGNAL(clicked()), this, SLOT(newGame()));
   connect(ui.pbAbout, SIGNAL(clicked()), this, SLOT(about()));
   connect(ui.pbSettings, SIGNAL(clicked()), this, SLOT(settings()));
   connect(ui.gvBoard, SIGNAL(newScore(int)), this, SLOT(score(int)));

   // Set board size based on the screen size. Make sure the playing area is square.
   QDesktopWidget desk;
   int a = std::min(desk.width(), desk.height()) * 0.8;
   int s = a * 0.813;
   setFixedSize(QSize(a * 0.84, a * 1.02));
   ui.gvBoard->setFixedSize(QSize(s, s));
   m_best = m_settings.value("highscore",0).toInt();
   ui.lblBestValue->setText(QString::number(m_best));

   ui.pbSettings->hide();
}

UserInterface::~UserInterface() {
}

void UserInterface::keyPressEvent(QKeyEvent* event) {
   if (event->key() == Qt::Key_Escape) {
      m_settings.setValue("highscore", m_best);
      QApplication::quit();
   } else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
      ui.gvBoard->moveLeft();
   }
   else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
      ui.gvBoard->moveRight();
   }
   else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
      ui.gvBoard->moveUp();
   }
   else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
      ui.gvBoard->moveDown();
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

