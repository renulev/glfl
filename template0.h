#ifndef GLFL_H
#define GLFL_H
#define GLFL_VERSION_MAJOR 1
#define GLFL_VERSION_MINOR 0
#define GLFL_VERSION_PATCH 0
#define GLFL_VERSION_STRING "1.0.0"
#include <GL/glcorearb.h>
#if defined(_WIN32)
#include <libloaderapi.h>
#else
#include <GL/glx.h>
#include <dlfcn.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif
