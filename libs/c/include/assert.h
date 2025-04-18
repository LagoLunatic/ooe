#ifndef ASSERT_H
#define ASSERT_H

void assert(const char* assertion, const char* file, const char* function, int line);
// clang-format off
#define assert(cond) if (!(cond)) { assert(#cond, __FILE__, __FUNCTION__, __LINE__); }
// clang-format on

#endif /* ASSERT_H */
