// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/hpet.h>   // implements

u32_t hpet_get_period_fs(void* hpet_base_reg)
{
  u64_t cap_reg = *(u64_t*)((u64_t)hpet_base_reg + HPET_GENERAL_CAPABILITIES_REGISTER_OFFSET);
  return (u32_t)(cap_reg >> 32);
}

void hpet_enable(void* hpet_base_reg, bool legacy_replacement)
{
  u64_t* reg = (u64_t*)((u64_t)hpet_base_reg + HPET_GENERAL_CONFIG_REGISTER_OFFSET);
  *reg &= ~(0x3);   // clear
  *reg |= (0x1 | (legacy_replacement << 1));
}

void hpet_disable(void* hpet_base_reg)
{
  u64_t* reg = (u64_t*)((u64_t)hpet_base_reg + HPET_GENERAL_CONFIG_REGISTER_OFFSET);
  *reg &= ~(0x1);
}

u64_t hpet_poll(void* hpet_base_reg)
{
  return *(u64_t*)((u64_t)hpet_base_reg + HPET_MAIN_COUNTER_REGISTER_OFFSET);
}
