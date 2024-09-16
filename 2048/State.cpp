#include "State.h"

bool State::operator==(State& other) {
   if (m_score != other.m_score)
      return false;

   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++) {
         if (m_tiles[i][j] != other.m_tiles[i][j])
            return false;
         if (m_pos[i][j] != other.m_pos[i][j])
            return false;
      }

   return true;
}

bool State::operator!=(State& other) {
   return !operator==(other);
}

State& State::operator=(State& other) {
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
