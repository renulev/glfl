#ifndef GLFL_H
#define GLFL_H
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
