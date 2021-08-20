#pragma once

class Animation {
 public:
  Animation(int head, int body, int arms, int legs);
  int Next() const;
};