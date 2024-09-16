#pragma once
#include <QVector>
#include "State.h"

#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4

struct SearchNode {

   SearchNode(State& _state, int _step) :
      step(_step), state(_state) {
   }

   int step = 0;
   State state;

   SearchNode* parent = nullptr;
   SearchNode* left = nullptr;
   SearchNode* right = nullptr;
   SearchNode* up = nullptr;
   SearchNode* down = nullptr;
};

class SearchTree {
public:
   SearchTree(State& _state, int steps = 10);
   int getNextMove();

private:
   int m_steps = 0;
   SearchNode* m_root = nullptr;
   State m_state;
   QVector<SearchNode*> m_bestpath;
};


