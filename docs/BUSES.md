# Buses

A list of buses you would find on a modern PC. There are so many to
accomodade different devices at different speeds, and for legacy
reasons of course.

## High-Speed

- Memory Bus (DDR4/DDR5): The direct, high-width path between the CPU
  and your RAM modules. It is the most time-critical bus in the
  system.

- PCI Express (PCIe): The primary expansion bus. It uses "lanes" to
  scale bandwidth for GPUs, NVMe drives, and high-speed networking
  cards.

- DMI (Direct Media Interface): Intel’s proprietary link (essentially
  a specialized PCIe bus) that connects the CPU to the PCH.

- Infinity Fabric: AMD’s specialized internal bus used to link
  different "chiplets" (cores and I/O dies) within the processor
  package.

## Peripheral & Storage Buses

- USB (Universal Serial Bus): A versatile bus for external
  devices. Modern PCs run multiple versions (2.0, 3.2, USB4)
  simultaneously.

- SATA (Serial ATA): The standard bus for mechanical hard drives and
  older 2.5-inch SSDs.

- Thunderbolt / USB4: A high-speed external bus capable of "tunneling"
  other protocols like DisplayPort and PCIe over a single cable.

- DisplayPort / eDP: The dedicated video bus that carries pixel data
  from the GPU to the monitor or laptop panel.

## System Management & Legacy Buses

- eSPI (Enhanced Serial Peripheral Interface): The modern successor to
  LPC. It handles the "Super I/O" functions like power management and
  keyboard controller data.

- LPC (Low Pin Count): A legacy bus still found in some systems to
  connect the TPM (security chip) or the BIOS/UEFI flash chip.

- SPI (Serial Peripheral Interface): A simple, fast bus used primarily
  for the CPU/PCH to read the firmware stored on the UEFI/BIOS chip
  during boot.

- SMBus (System Management Bus): Derived from I2C, this is used for
  low-speed communication like reporting RAM temperatures or battery
  status in laptops.

- I2C (Inter-Integrated Circuit): A simple two-wire bus used for
  internal hardware like touchpads, RGB lighting controllers, and
  various sensors.

- HD Audio / SoundWire: Dedicated buses for transporting digital audio
  signals from the PCH to the audio codec (the chip that handles your
  headphone jack).

## Internal Processor Interconnects

- Ring Bus: Used in most Intel consumer CPUs to allow different cores
  and the integrated GPU to share the L3 cache.

- Mesh Interconnect: A grid-like bus architecture used in
  high-core-count server CPUs (like Intel Xeon) to prevent data
  traffic jams.
