#pragma once

// macros
// namespace
#if !defined(NAMESPACE_BEGIN)
#define NAMESPACE_BEGIN(name) namespace name {
#endif
#if !defined(NAMESPACE_END)
#define NAMESPACE_END(name) }
#endif

NAMESPACE_BEGIN(DR)
using uint = unsigned int;
// suppress the unused-parameter warning
template <class T> void ignore(const T &) {}
NAMESPACE_END(DR)
