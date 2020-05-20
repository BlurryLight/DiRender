#pragma once
#include <cores/primitive.h>
#include <map>
#include <unordered_map>
#include <vector>

NAMESPACE_BEGIN(DR)
struct Scene // film
{
  int width = 800;
  int height = 800;
  float AspectRatio = 1.0f;
};
NAMESPACE_END(DR)
