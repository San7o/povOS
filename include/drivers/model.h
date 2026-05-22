// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVER_MODEL_H
#define POVOS_DRIVER_MODEL_H

#include <libk/stdbool.h>

//
// Driver Model
// ============
//


// Forward declarations
struct device;
struct driver;
struct devclass;
struct devclass_ops;

enum devclass_type {
  DEVCLASS_NONE = 0,
  DEVCLASS_PCI,
  _DEVCLASS_MAX,
};

// Device
// ------
//
// A device represents metadata of a specific hardware or software unit, like a
// sensor or a controller.
struct device {
    const char* name;
    enum devclass_type type;
};

struct device_list_elem {
  struct device *dev;
  struct device_list_elem *next;
};

struct device_list {
  struct device_list_elem *head;
  struct device_list_elem *tail;
};

// Driver
// ------
//
// A driver implements funcionality for a device
struct driver {
  const char *name;
  int priv_size;
  enum devclass_type type;
  struct devclass_ops *ops;
};

struct driver_list_elem {
  struct driver *driver;
  struct driver_list_elem *next;
};

struct driver_list {
  struct driver_list_elem *head;
  struct driver_list_elem *tail;
};

// Device Class
// ------------
//
// A device class manages devices and drivers of the same class type. It a
// device to the right driver dynamically, managing their lifetime.
struct devclass {
  enum devclass_type type;
  struct device_list devices;
  struct driver_list drivers;
};

struct devclass_ops {
  int (*probe) (struct device* dev);
  int (*remove) (struct device* dev);
};

// Devclass Manager
// ----------------

struct devclass_manager {
  struct devclass devclasses[_DEVCLASS_MAX];
};

extern struct devclass_manager *glob_devclass_manager;

void devclass_manager_init(void);

bool devclass_register_device(struct device *dev);
bool devclass_register_driver(struct driver *driver);

bool devclass_try_match(struct device *dev, struct driver *driver);

#endif // POVOS_DRIVER_MODEL_H
