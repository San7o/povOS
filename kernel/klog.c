// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/klog.h>   // implements

#include <libk/stdio.h>

enum klog_level glob_klog_level = KLOG_LEVEL_TRACE;

void klog_set_level(enum klog_level lvl)
{
  glob_klog_level = lvl;
}

int klog(enum klog_level lvl, const char* fmt, ...)
{
  if (lvl < glob_klog_level)
    return 0;

  va_list list;
  va_start(list, fmt);
  int ret = vprintk(fmt, list);
  va_end(list);

  return ret;
}
