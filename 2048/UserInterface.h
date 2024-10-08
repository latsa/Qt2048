#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_UserInterface.h"
#include "DlgAbout.h"
#include <QSettings>
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
   void newGame();
   void about();
   void settings();
   void score(int v);
   void solve();
   void nextMove();
   void cancel();

private:
   Ui::MainUserInterfaceClass ui;
   bool m_solving = false;
   bool m_paused = false;
   QTimer* m_solver = Q_NULLPTR;

   QSettings m_settings;
   DlgAbout* m_dlgAbout = Q_NULLPTR;

   // mouse input state
   QPoint m_mouse_oldPos;
   bool m_mouse_dragging = false;


   int m_score = 0;
   int m_best = 0;
};
