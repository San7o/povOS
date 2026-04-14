// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/isa/isa.h>   // implements
#include <kernel/utils.h>
#include <kernel/macros.h>

#define ISA_INIT_KEY 0x6a

// Linear feedback shift register, used to provide an initiation key
// protocol and to provide a checksum verification during serial data
// read in the isolation protocol.
static u8_t isa_key_lfsr(u8_t val)
{
  return (((((val & 0x1) ^ ((val & 0x2) >> 1)) << 7 )) | (val >> 1));
}

//
// The major steps of the auto-configuration process are as follows:
//
//  - put all Plug-and-Play ISA cards in configuration mode
//  - isolate one Plug-and-Play ISA card at a time
//  - assign a handle and read the card's resource data structure
//  - after the resource requirements and capabilities are determined
//    for all cards, use the handle to assign conflict free resources
//    to each card
//  - activate all Plug-and-Play ISA cards and remove them from
//    configuration mode
//
// The BIOS shoud automatically do this for us, but we should not rely
// on that.
//
// Refer to the specification section 2.2 for this procedure.
//

void isa_pnp_set_config_mode(void)
{
  disable_interrupts();

  port_outb(ISA_PNP_ADDRESS_PORT, 0);
  port_outb(ISA_PNP_ADDRESS_PORT, 0);
  volatile u8_t key = ISA_INIT_KEY;
  for (int i = 0; i < 32; ++i)
  {
    port_outb(ISA_PNP_ADDRESS_PORT, key);    
    key = isa_key_lfsr(key);
  }

  enable_interrupts();
}
