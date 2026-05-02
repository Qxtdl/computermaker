#pragma once

#include <stdio.h>
#include <stdlib.h>

#define APP_STAGE_STRING "alpha"
#define APP_VERSION_STRING "2.5.0"
#define APP_RELEASE_STRING APP_STAGE_STRING " " APP_VERSION_STRING

#define app_error(...) \
   { \
      fprintf(stderr, "[ERROR]: "__VA_ARGS__); \
      exit(1); \
   }

#define app_warn(...) \
   { \
      fprintf(stderr, "[WARN]: " __VA_ARGS__); \
   }

#define app_log(...) \
   { \
      fprintf(stderr, "[LOG]: " __VA_ARGS__); \
   }
