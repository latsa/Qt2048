#include "Setup2048.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Setup2048 w;
    w.show();
    return a.exec();
}
