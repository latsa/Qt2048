#pragma once
#include <QtWidgets/QMainWindow>
#include <QString>
#include "ui_Setup2048.h"

class Setup2048 : 
   public QMainWindow {
   Q_OBJECT

public:
   Setup2048(QWidget *parent = nullptr);
   ~Setup2048();

private slots:
   void select();
   void install();
   void cancel();

private:
   Ui::Setup2048Class ui;
   QString m_targetLocation = QString("C:/Program Files");
   QString m_installationPath = QString("/AlgoFlux/2048");
   bool m_startApp = false;  // option to start application after installation
   bool m_installed = false; // installation has been completed successfully
};
