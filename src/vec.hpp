#pragma once

template<typename T>
struct vec2 {
  T x, y;

  vec2 operator+(const vec2& rhs) {
    return vec2{x + rhs.x, y + rhs.y};
  }
};

typedef vec2<float> vec2f;
typedef vec2<int> vec2i;

