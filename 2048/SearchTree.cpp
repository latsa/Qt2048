#include "SearchTree.h"
#include <QDateTime>
#include <random>

int getRand(int min, int max) {
   unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
   std::mt19937 gen(ms);
   std::uniform_int_distribution<> uid(min, max);
   return uid(gen);
}

SearchTree::SearchTree(State& _state, int steps) : m_steps(steps) {
   m_root = new SearchNode(_state, 0);
}

int SearchTree::getNextMove() {
   return getRand(0, 5);

}
