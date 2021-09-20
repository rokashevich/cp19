#pragma once

class tribool {
  int state_;  // 1, 0, -1
 public:
  tribool() : state_{} {}
  tribool(int i) : state_{i > 0 ? 1 : (i < 0 ? -1 : 0)} {}

  friend int cmp(const tribool& lhs, const tribool& rhs) {
    return lhs.state_ == rhs.state_ ? 0 : (lhs.state_ > rhs.state_ ? 1 : -1);
  }
  bool operator==(const tribool& rhs) { return cmp(*this, rhs) == 0; }
  bool operator!=(const tribool& rhs) { return cmp(*this, rhs) != 0; }
  bool operator<(const tribool& rhs) { return cmp(*this, rhs) < 0; }
  bool operator>(const tribool& rhs) { return cmp(*this, rhs) > 0; }
  bool operator<=(const tribool& rhs) { return cmp(*this, rhs) <= 0; }
  bool operator>=(const tribool& rhs) { return cmp(*this, rhs) >= 0; }

  tribool& operator++() {
    state_ = state_ < 0 ? 0 : 1;
    return *this;
  }
  tribool& operator--() {
    state_ = state_ > 0 ? 0 : -1;
    return *this;
  }
  tribool operator++(int) {
    tribool t{*this};
    ++*this;
    return t;
  }
  tribool operator--(int) {
    tribool t{*this};
    --*this;
    return t;
  }

  tribool& operator+=(tribool& rhs) {
    this->state_ += rhs.state_;
    return *this;
  }
};