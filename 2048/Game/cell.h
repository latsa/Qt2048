#pragma once
#include <QString>

struct Cell {
   int px = 0;
   int py = 0;

   Cell(int _px = 0, int _py = 0);
   Cell& operator =(Cell other);
   bool operator ==(Cell& other);
   bool operator !=(Cell& other);
   void clear();
   void dump(const QString& prompt = QString());
};