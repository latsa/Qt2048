#pragma once
#include "cell.h"

struct Tile {
   int value = 0;

   // animation data
   bool ismoving = false; // true if tile is being animated
   Cell lsource; // logical source coordinates [0,3]
   Cell ltarget; // logical target coordinates [0,3]
   Cell psource; // pixel source coordinates
   Cell ptarget; // pixel target coordinates
   Cell pcurrent;// pixel coordinaes of current position
   
   Tile(int val = 0);
   Tile& operator =(Tile& other);
   void clear();
   void dump(QString prompt = QString());
};
