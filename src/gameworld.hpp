#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include <iostream>

// Состояние игрового мира.

class GameWorld {
 public:
  GameWorld();
  ~GameWorld();

 private:
  size_t dimension_;  // Размерность куба игрового мира.
};

#endif  // GAMEWORLD_HPP
