#pragma once
#include "Tile.h"

struct State {
   bool operator==(State& other);
   bool operator!=(State& other);
   State& operator=(State& other);

   int m_score = 0;
   Tile m_tiles[4][4]; // contains the values and animation data
   Cell m_pos[4][4];   // contains the physical position of tiles on the board
};

