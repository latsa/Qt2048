#include "cell.h"
#include <QDebug>

Cell::Cell(int _px, int _py) :
   px(_px), py(_py) {
}


Cell& Cell::operator =(Cell other) {
   if (this == &other)
      return *this;

   px = other.px;
   py = other.py;

   return *this;
}

bool Cell::operator ==(Cell& other) {
   if (px != other.px)
      return false;

   if (py != other.py)
      return false;

   return true;
}

bool Cell::operator !=(Cell& other) {
   return !operator==(other);
}

void Cell::clear() {
   px = 0;
   py = 0;
}

void Cell::dump(const QString& prompt) {
   if (prompt.size())
      qDebug() << "   " << prompt << ":";
   qDebug() << "px=" << px << ", py=" << py;
}