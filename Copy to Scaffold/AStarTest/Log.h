#ifndef LOG_H
#define LOG_H

#include "bc.h"
#include <stdio.h>

// Logger
#ifdef NDEBUG
#define log_error(condition, message) ((void)0)
#define CHECK_ERRORS() ((void)0)
#else

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#define log_error(condition, message)   \
if (!(condition)) {           \
  printf("[info] " __FILE__ ": " S__LINE__ ": " message); \
}

#define CHECK_ERRORS() \
if (bc_has_err()) { \
  char* err; \
  uint8_t code = bc_get_last_err(&err); \
  printf("[ERROR](" __FILE__ ": " S__LINE__ ") code %d: %s\n", code, err); \
  bc_free_string(err); \
}

#endif
#endif
