#include "UserInterface.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
   QApplication a(argc, argv);
   QApplication::setOrganizationName("AlgoFlux");
   QApplication::setOrganizationDomain("algoflux.io");
   QApplication::setApplicationName("2048");

   UserInterface mainWindow;
   
   mainWindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
   mainWindow.show();
   return a.exec();
}
