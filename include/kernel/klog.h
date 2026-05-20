// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_LOGGER_H
#define POVOS_KERNEL_LOGGER_H

//
// Kernel logger
// =============
//

enum klog_level {
  KLOG_LEVEL_TRACE = 0,
  KLOG_LEVEL_DEBUG,
  KLOG_LEVEL_INFO,
  KLOG_LEVEL_WARN,
  KLOG_LEVEL_ERR,
  KLOG_LEVEL_FATAL,
  _KLOG_LEVEL_MAX,
};

extern enum klog_level glob_klog_level;

void klog_set_level(enum klog_level lvl);

int klog(enum klog_level lvl, const char* fmt, ...);

#define KLOG_LEVEL_TRACE_STR   "[trace] "
#define KLOG_LEVEL_DEBUG_STR   "[debug] "
#define KLOG_LEVEL_INFO_STR    "[info] "
#define KLOG_LEVEL_WARN_STR    "[warn] "
#define KLOG_LEVEL_ERR_STR     "[error] "
#define KLOG_LEVEL_FATAL_STR   "[fatal] "

#define ktrace(...)  klog(KLOG_LEVEL_TRACE, KLOG_LEVEL_TRACE_STR __VA_ARGS__)
#define kdebug(...)  klog(KLOG_LEVEL_DEBUG, KLOG_LEVEL_DEBUG_STR __VA_ARGS__)
#define kinfo(...)   klog(KLOG_LEVEL_INFO,  KLOG_LEVEL_INFO_STR  __VA_ARGS__)
#define kwarn(...)   klog(KLOG_LEVEL_WARN,  KLOG_LEVEL_WARN_STR  __VA_ARGS__)
#define kerr(...)    klog(KLOG_LEVEL_ERR,   KLOG_LEVEL_ERR_STR   __VA_ARGS__)
#define kfatal(...)  klog(KLOG_LEVEL_FATAL, KLOG_LEVEL_FATAL_STR __VA_ARGS__)

#endif // POVOS_KERNEL_LOGGER_H
