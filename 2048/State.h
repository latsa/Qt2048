#pragma once
#include "Tile.h"

struct State {

   State& operator=(State& other) {
      if (this == &other)
         return *this;

      m_score = other.m_score;
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++) {
            m_tiles[i][j] = other.m_tiles[i][j];
            m_pos[i][j] = other.m_pos[i][j];
         }

      return *this;
   }

   int m_score = 0;
   Tile m_tiles[4][4]; // contains the values and animation data
   Cell m_pos[4][4];   // contains the physical position of tiles on the board
};

