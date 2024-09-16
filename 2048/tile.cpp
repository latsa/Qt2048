#include "tile.h"
#include <QDebug>

Tile::Tile(int val) {
   clear();
   value = val;
}

Tile& Tile::operator =(Tile& other) {

   if (this == &other)
      return *this;

   value = other.value;
   ismoving = other.ismoving;
   lsource = other.lsource;
   ltarget = other.ltarget;
   pcurrent = other.pcurrent;
   psource = other.psource;
   ptarget = other.ptarget;
   return *this;
}

bool Tile::operator==(Tile& other) {
   if (value != other.value)
      return false;
   if (ismoving != other.ismoving)
      return false;
   if (lsource != other.lsource)
      return false;
   if (ltarget != other.ltarget)
      return false;
   if (psource != other.psource)
      return false;
   if (ptarget != other.ptarget)
      return false;

   return true;
}

bool Tile::operator!=(Tile& other) {
   return !operator==(other);
}

void Tile::clear() {
   value = 0;
   ismoving = false;
   lsource.clear();
   ltarget.clear();
   psource.clear();
   ptarget.clear();
   pcurrent.clear();
}

void Tile::dump(QString prompt) {
   if (prompt.size())
      qDebug() << prompt << ":";

   qDebug() << "value=" << value << ", ismoving=" << (ismoving ? "true" : "false");
   lsource.dump("l source");
   ltarget.dump("l target");
   psource.dump("p source");
   ptarget.dump("p target");
   pcurrent.dump("p current");
}

