#include "SearchTree.h"
#include "board.h"
#include <QDateTime>
#include <random>

int getRand(int min, int max) {
   unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
   std::mt19937 gen(ms);
   std::uniform_int_distribution<> uid(min, max);
   return uid(gen);
}

SearchTree::SearchTree(State& _state, int steps) : 
   m_state(_state), m_steps(steps) {
   m_root = new SearchNode(_state, 0);
}

int SearchTree::getNextMove() {
   return getRand(0, 5);
   /*
   int score = 0;
   int dir = 0;
   State state = m_state;
   State states[4];

   m_root->parent = nullptr;
   
   m_root->left = new SearchNode(m_state, 1);
   if (!Board::moveLeft(&m_root->left->state.m_tiles, &m_root->left->state.m_pos, m_root->left->state.m_score, false)) {
      states[0] = m_root->left->state;

      bool equal = true;
      for(int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++) {
            if (state.m_tiles[i][j] != m_root->left->state.m_tiles[i][j]) {
               equal = false;
               break;
            }
         }

      if (equal)
         states[0].m_score = 0;
   }
   
   m_root->right = new SearchNode(m_state, 1);
   if (!Board::moveRight(&m_root->right->state.m_tiles, &m_root->right->state.m_pos, m_root->right->state.m_score, false)) {
      states[1] = m_root->left->state;

      bool equal = true;
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++) {
            if (state.m_tiles[i][j] != m_root->right->state.m_tiles[i][j]) {
               equal = false;
               break;
            }
         }

      if (equal)
         states[1].m_score = 0;
   }

   m_root->up = new SearchNode(m_state, 1);
   if (!Board::moveUp(&m_root->up->state.m_tiles, &m_root->up->state.m_pos, m_root->up->state.m_score, false)) {
      states[2] = m_root->left->state;

      bool equal = true;
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++) {
            if (state.m_tiles[i][j] != m_root->up->state.m_tiles[i][j]) {
               equal = false;
               break;
            }
         }

      if (equal)
         states[2].m_score = 0;

   }

   m_root->down = new SearchNode(m_state, 1);
   if (!Board::moveDown(&m_root->down->state.m_tiles, &m_root->down->state.m_pos, m_root->down->state.m_score, false)) {
      states[3] = m_root->left->state;

      bool equal = true;
      for (int i = 0; i < 4; i++)
         for (int j = 0; j < 4; j++) {
            if (state.m_tiles[i][j] != m_root->down->state.m_tiles[i][j]) {
               equal = false;
               break;
            }
         }

      if (equal)
         states[3].m_score = 0;
   }

   for (int i = 0;i < 4;i++) {
      if (score <= states[i].m_score) {
         score = states[i].m_score;
         state = states[i];
         dir = 1+i;
      }
   }

   while (!dir)
      dir = getRand(0, 5);

   return dir;
   */
   }
