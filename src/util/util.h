#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdarg.h>

static inline void panic(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("ERROR: ");
  vprintf(fmt, args);
  va_end(args);

  exit(-1);
}


#endif
