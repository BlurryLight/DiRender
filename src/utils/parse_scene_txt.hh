//
//* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
//

#pragma once
#include "di_global.h"

// NAMESPACE_BEGIN(DR)
// NAMESPACE_BEGIN(IMPL)
namespace DR {
struct Scene;
namespace impl {
inline constexpr int kMaxNumLights = 10;
inline constexpr int kMaxNumObjects = 2'000;

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(std::stringstream &s, const int numvals, float *values);

// copy from
// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split(const std::string &str,
                               const std::string &delim);

extern void make_render(Scene *scene, int spp, const std::string &type);
void parse_scene_txt(std::string filename, Scene *scene);
} // namespace impl

} // namespace DR

// NAMESPACE_END(IMPL)
// NAMESPACE_END(DR)
