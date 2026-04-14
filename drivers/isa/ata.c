// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/isa/ata.h>   // implements
#include <drivers/isa/isa.h>
#include <kernel/utils.h>

bool ata_enabled(port_t port)
{
  ata_status_reg_t ata_status = {0};
  ata_status.raw = port_inb(port + ATA_STATUS_REG_OFFSET);
  return ata_status.raw == 0xFF;
}

ata_status_reg_t ata_wait_status(port_t port)
{
  ata_status_reg_t status = {0};

  while (1)
  {
    status.raw = isa_readb(port + ATA_STATUS_REG_OFFSET);
    
    if (status.raw == 0xFF)
    {
      status.flags.err = 1; 
      return status;
    }

    if (status.flags.err || status.flags.df) return status;
    if (!status.flags.bsy && status.flags.drq) return status;
        
    io_wait();
  }
  
  return status;
}

static bool _ata_read_sector(port_t port, u8_t *dest, u32_t lba)
{
  ata_status_reg_t status;
  int max_attempts = 4, attempt = 0, i;
  u16_t w;
  ata_head_reg_t head_reg = {
    .flags = {
      .head = (lba >> 24) & 0xF,
      .set  = 1,
      .lba  = 1,
      .set2 = 1,
    }
  };

 try:

  isa_writeb(port + ATA_HEAD_REG_OFFSET, head_reg.raw);
  isa_writeb(port + ATA_FEATURES_REG_OFFSET, 0x00);
  isa_writeb(port + ATA_SECTOR_COUNT_REG_OFFSET, 1);
  isa_writeb(port + ATA_SECTOR_NUM_REG_OFFSET, (u8_t) lba);
  isa_writeb(port + ATA_CYLINDER_LOW_REG_OFFSET, (u8_t) (lba >> 8));
  isa_writeb(port + ATA_CYLINDER_HIGH_REG_OFFSET, (u8_t) (lba >> 16));
  isa_writeb(port + ATA_CMD_REG_OFFSET, ATA_CMD_PIO_READ_SECTORS);

  status = ata_wait_status(port);
  if (status.flags.err || status.flags.df)
  {
    attempt++;
    if (attempt >= max_attempts)
      return false;
    goto try;
  }

  for (i = 0; i < ATA_SECTOR_SIZE; i += 2)
  {
    w = isa_readw(port + ATA_DATA_REG_OFFSET);
    ((u16_t*)dest)[i / 2] = w;
  }

  return true;
}

bool ata_read(port_t port, u8_t *dest, u32_t from, u32_t sectors)
{
  if (sectors == 0) return true;
  
  u32_t i;
  
  disable_interrupts();

  for (i = 0; i < sectors; ++i)
  {
    if (!_ata_read_sector(port, dest, from + i))
    {
      enable_interrupts();
      return false;
    }
    dest += ATA_SECTOR_SIZE;
  }
  
  enable_interrupts();
  return true;
}

static bool _ata_write_sector(port_t port, u8_t *src, u32_t lba)
{
  ata_head_reg_t head_reg = {
    .flags = {
      .head = (lba >> 24) & 0xF,
      .set  = 1,
      .lba  = 1,
      .set2 = 1,
    }
  };

  isa_writeb(port + ATA_HEAD_REG_OFFSET, head_reg.raw);

  ata_wait_status(port);

  isa_writeb(port + ATA_HEAD_REG_OFFSET, head_reg.raw);
  isa_writeb(port + ATA_FEATURES_REG_OFFSET, 0x00);
  isa_writeb(port + ATA_SECTOR_COUNT_REG_OFFSET, 1);
  isa_writeb(port + ATA_SECTOR_NUM_REG_OFFSET, (u8_t) lba);
  isa_writeb(port + ATA_CYLINDER_LOW_REG_OFFSET, (u8_t) (lba >> 8));
  isa_writeb(port + ATA_CYLINDER_HIGH_REG_OFFSET, (u8_t) (lba >> 16));
  isa_writeb(port + ATA_CMD_REG_OFFSET, ATA_CMD_PIO_WRITE_SECTORS_RETRY);

  ata_wait_status(port);

  for (int i = 0; i < ATA_SECTOR_SIZE; i += 2)
  {
    isa_writew(port + ATA_DATA_REG_OFFSET, ((u16_t*)src)[i/2]);
    io_wait();
  }
  
  ata_wait_status(port);
  
  return true;
}

bool ata_write(port_t port, u8_t *src, u32_t to, u32_t sectors)
{
  u32_t i;
  
  disable_interrupts();

  for (i = 0; i < sectors; ++i)
  {
    if (!_ata_write_sector(port, src, to + i))
    {
      enable_interrupts();
      return false;
    }
    src += ATA_SECTOR_SIZE;

    io_wait();
  }
  
  enable_interrupts();
  return true;
}
