#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_UserInterface.h"
#include "DlgAbout.h"

#include <QPoint>
#include <QTimer>

class UserInterface :
   public QMainWindow {
   Q_OBJECT

public:
   UserInterface(QWidget *parent = nullptr);
   ~UserInterface();
   Board* getBoard();

protected:
   void keyPressEvent(QKeyEvent* event) override;
   void mousePressEvent(QMouseEvent* evt) override;
   void mouseReleaseEvent(QMouseEvent* evt) override;
   void mouseMoveEvent(QMouseEvent* evt) override;

private slots:
   // push button handlers
   void solve();   // start/pause automatic solver
   void cancel();  // cancel automatic solving
   void newGame(); // start new game

   void about();   // show about dialog
   void settings();// show settings dialog
   void undo();    // undo last move

   void exit();

   // calculate next move automatically
   void nextMove();   
   
   // update game score display
   void score(int v, int b); 


private:
   Ui::MainUserInterfaceClass ui;

   // automatic solver
   QTimer* m_solver = Q_NULLPTR;
   bool m_solving = false;
   bool m_paused = false;
   
   DlgAbout* m_dlgAbout = Q_NULLPTR;

   // mouse input state
   QPoint m_mouse_oldPos;
   bool m_mouse_dragging = false;
};
