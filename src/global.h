#pragma once

#include <stdio.h>
#include <stdlib.h>

#define app_error(...) \
   { \
      fprintf(stderr, __VA_ARGS__); \
      exit(1); \
   }
