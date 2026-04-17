// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/unistd.h>   // implements
#include <kernel/utils.h>
#include <kernel/time.h>
#include <kernel/macros.h>

unsigned int sleep_ms(u64_t milliseconds)
{
  u64_t elapsed_time_ms = 0;
  u64_t last_time_ms = time_ms;
  
  while(elapsed_time_ms < milliseconds)
  {
    elapsed_time_ms += time_ms - last_time_ms;
    last_time_ms = time_ms;
    cpu_halt();
  }

  return 0;
}

unsigned int sleep(u64_t seconds)
{
  return sleep_ms(seconds * 1000);
}

void sleep_ticks(u64_t clicks)
{
  if (clicks) while(clicks--) { NOP; };
}
