# TODO

Future plans for the operating system.

## Bootloader

I want to port the operating system to multiple architectures. It all starts
from the bootloader, I have to be able to boot from different environments.
Currently, only the x86_64 with BIOS boot architecture is supported. To support
more architectures and firmwares, I need to create a few abstractions:

- backend: the code that is run right after the firmware. The backend is
  expected to setup the CPU to a well defined state in the bootloader
- bootloader intermediate API: an internal API used to match backend and
  frontend
- frontend: after the backend, the frontend sets up the CPU in a well defined
  state for the OS. This is the API that is presented to the OS, and it must be
  architecture specific.

After the bootloader, the OS would have the following abstractions:

- Kernel ARCH start: an arch dependent initialization phase (setup proper
  interrupts, paging...)
- Generic kernel start: now the cross-platform kernel logic can start, and it
  can use cross-platform abstractions over the hardware such as for memory
  management and interrupts.

Here is an high-level overview of the boot process:

![boot-process](./docs/boot-diagram.png)

Todo, in order:

- have a well-defined bootloader to kernel API
- have a well-defined intermediate bootloader API
- port the current bootloader to the new APIs
- have an uefi backed

An advanced topic would be to have the OS choose which frontend to use (by
setting some data in a specific section for example, but this would require the
kernel to be in ELF and the bootloader to know about ELF format).

In the real world, we would also have a trusted execution layer and signed
bootloaders, but that is for another day.

## Driver Model

One of the most important framework in the kernel is the driver model. We need:

- device class abstraction: manages the lifecycle of devices and drivers,
  defines operations that the driver must implement.

```c
struct devclass {
    enum devclass_type type;
    struct device_list devices;
    struct driver_list drivers;
};

struct devclass_ops {
    int (*probe) (struct device* dev);
    int (*remove) (struct device* dev);
};
```

- device abstraction: just data about a device (bus, ID, name). It is the
  equivalent to the device tree represented as a C api. It may contain hardcoded
  devices and probed devices during runtime, and it can be added or removed
  (plug and play), so it must be dynamic. 

```c
struct device {
    const char* name;
    enum devclass_type type;
};

struct pci_dev_id {
    int vendor_id;
    int device_id;
};

struct pci_dev {
    struct device dev;
    struct pci_device_id id;
};
```

- driver abstraction: a driver has some id data and belongs to a class, it
  implements its operations. A driver may be added or removed during runtime so
  there must exist a dynamic structure of drivers managed by the device class.

```c
struct driver {
    const char *name;
    int priv_size;
    enum devclass_type type;
    struct devclass_ops *ops;
};

struct pci_driver {
    struct driver driver;
    struct pci_device_id id;
};
```

The device looks at its devices and tries to match a driver (calling the
driver's `probe` function.

FreeBSD's design is very clean:
https://docs.freebsd.org/en/books/arch-handbook/newbus/

Also check out:

https://www.kernel.org/doc/html/latest/driver-api/driver-model/index.html
https://docs.u-boot.org/en/latest/develop/driver-model/design.html

## Scheduler

The current scheduler is a simple preentive Round Robin with a fixed time
"quantum", meaning that each process is divided equally time-wise. A good
addition would be to introduce priority queues with some kind of penalty
mechanism to avoid starvation, meaning that tasks with higher priority will be
executed first, then the ones with lower priority, but if a process with high
priority is taking too much time, then the scheduler will go to the lower ones
anyway (avoiding starvation).

Another design that takes into account real time constraints would also be nice
to have.

## Kernel

- interrupt API: you should be able to mask, register and redirect interrupts in
  an arch-independent way
  - have a software interrupt vector
  - Drivers should use `request_irq()` and `free_irq()`, `disable_irq()` and
    `enable_irq()`. Some lower level platform plumbing may also have to take
    into accout CPU local IRQ with an `request_percpu_irq()` api.
- general device model: bus, device, driver abstractions
- vfs: the VFS interface is already implemented, now a filesystem that uses it
  should be implemented too, like FAT32.
- testing framework
- shell: implement a small programming language like BASIC or Bash in the shell
- syscalls
- userspace
- have a `menuconfig`-like and `Kconfig`-like kernel configuration system
- use multiple cores eventually
- study realtime considerations (deterministic allocation time, interrupt top
  half and bottom half)

## Drivers

- AHCI: modern standard to communicate with SATA devices
- SMBus: required for I2C, which is useful for emulated sensors
- VESA:  high resolution graphics, but we need to mess with the bootloader
- ACPI: DSDT

## Chores

- read-write lock
- power off: surprisingly, doing this right is a bit more complicated than you
  think. It may require some power management infrastructure.
- develop a custom emulated device in qemu, such as a temperature sensor using
  i2c
