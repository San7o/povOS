// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/input/keyboard.h>   // implements
#include <libk/string.h>

// Currently active keyboard
static keyboard_t *keyboard_active = NULL;

const char* keycode_str[_KEY_MAX] = {
    [KEY_NONE]           = "KEY_NONE",

    // Letters
    [KEY_A]              = "KEY_A",
    [KEY_B]              = "KEY_B",
    [KEY_C]              = "KEY_C",
    [KEY_D]              = "KEY_D",
    [KEY_E]              = "KEY_E",
    [KEY_F]              = "KEY_F",
    [KEY_G]              = "KEY_G",
    [KEY_H]              = "KEY_H",
    [KEY_I]              = "KEY_I",
    [KEY_J]              = "KEY_J",
    [KEY_K]              = "KEY_K",
    [KEY_L]              = "KEY_L",
    [KEY_M]              = "KEY_M",
    [KEY_N]              = "KEY_N",
    [KEY_O]              = "KEY_O",
    [KEY_P]              = "KEY_P",
    [KEY_Q]              = "KEY_Q",
    [KEY_R]              = "KEY_R",
    [KEY_S]              = "KEY_S",
    [KEY_T]              = "KEY_T",
    [KEY_U]              = "KEY_U",
    [KEY_V]              = "KEY_V",
    [KEY_W]              = "KEY_W",
    [KEY_X]              = "KEY_X",
    [KEY_Y]              = "KEY_Y",
    [KEY_Z]              = "KEY_Z",

    // Number row
    [KEY_1]              = "KEY_1",
    [KEY_2]              = "KEY_2",
    [KEY_3]              = "KEY_3",
    [KEY_4]              = "KEY_4",
    [KEY_5]              = "KEY_5",
    [KEY_6]              = "KEY_6",
    [KEY_7]              = "KEY_7",
    [KEY_8]              = "KEY_8",
    [KEY_9]              = "KEY_9",
    [KEY_0]              = "KEY_0",

    // Symbols
    [KEY_MINUS]          = "KEY_MINUS",
    [KEY_EQUAL]          = "KEY_EQUAL",
    [KEY_OPEN_BRACKET]   = "KEY_OPEN_BRACKET",
    [KEY_CLOSE_BRACKET]  = "KEY_CLOSE_BRACKET",
    [KEY_BACKSLASH]      = "KEY_BACKSLASH",
    [KEY_SEMICOLON]      = "KEY_SEMICOLON",
    [KEY_APOSTROPHE]     = "KEY_APOSTROPHE",
    [KEY_GRAVE]          = "KEY_GRAVE",
    [KEY_COMMA]          = "KEY_COMMA",
    [KEY_PERIOD]         = "KEY_PERIOD",
    [KEY_SLASH]          = "KEY_SLASH",

    // Whitespace / Control
    [KEY_SPACE]          = "KEY_SPACE",
    [KEY_TAB]            = "KEY_TAB",
    [KEY_ENTER]          = "KEY_ENTER",
    [KEY_BACKSPACE]      = "KEY_BACKSPACE",
    [KEY_ESCAPE]         = "KEY_ESCAPE",

    // Modifiers
    [KEY_LEFT_SHIFT]     = "KEY_LEFT_SHIFT",
    [KEY_RIGHT_SHIFT]    = "KEY_RIGHT_SHIFT",
    [KEY_LEFT_CTRL]      = "KEY_LEFT_CTRL",
    [KEY_RIGHT_CTRL]     = "KEY_RIGHT_CTRL",
    [KEY_LEFT_ALT]       = "KEY_LEFT_ALT",
    [KEY_RIGHT_ALT]      = "KEY_RIGHT_ALT",
    [KEY_LEFT_META]      = "KEY_LEFT_META",
    [KEY_RIGHT_META]     = "KEY_RIGHT_META",

    // Navigation
    [KEY_INSERT]         = "KEY_INSERT",
    [KEY_DELETE]         = "KEY_DELETE",
    [KEY_HOME]           = "KEY_HOME",
    [KEY_END]            = "KEY_END",
    [KEY_PAGE_UP]        = "KEY_PAGE_UP",
    [KEY_PAGE_DOWN]      = "KEY_PAGE_DOWN",

    // Arrows
    [KEY_UP_ARROW]       = "KEY_UP_ARROW",
    [KEY_DOWN_ARROW]     = "KEY_DOWN_ARROW",
    [KEY_LEFT_ARROW]     = "KEY_LEFT_ARROW",
    [KEY_RIGHT_ARROW]    = "KEY_RIGHT_ARROW",

    // Function keys
    [KEY_F1]             = "KEY_F1",
    [KEY_F2]             = "KEY_F2",
    [KEY_F3]             = "KEY_F3",
    [KEY_F4]             = "KEY_F4",
    [KEY_F5]             = "KEY_F5",
    [KEY_F6]             = "KEY_F6",
    [KEY_F7]             = "KEY_F7",
    [KEY_F8]             = "KEY_F8",
    [KEY_F9]             = "KEY_F9",
    [KEY_F10]            = "KEY_F10",
    [KEY_F11]            = "KEY_F11",
    [KEY_F12]            = "KEY_F12",

    // Numeric keypad
    [KEY_KP_0]           = "KEY_KP_0",
    [KEY_KP_1]           = "KEY_KP_1",
    [KEY_KP_2]           = "KEY_KP_2",
    [KEY_KP_3]           = "KEY_KP_3",
    [KEY_KP_4]           = "KEY_KP_4",
    [KEY_KP_5]           = "KEY_KP_5",
    [KEY_KP_6]           = "KEY_KP_6",
    [KEY_KP_7]           = "KEY_KP_7",
    [KEY_KP_8]           = "KEY_KP_8",
    [KEY_KP_9]           = "KEY_KP_9",
    [KEY_KP_PLUS]        = "KEY_KP_PLUS",
    [KEY_KP_MINUS]       = "KEY_KP_MINUS",
    [KEY_KP_MULT]        = "KEY_KP_MULT",
    [KEY_KP_DIV]         = "KEY_KP_DIV",
    [KEY_KP_ENTER]       = "KEY_KP_ENTER",
    [KEY_KP_DOT]         = "KEY_KP_DOT",
    [KEY_KP_SLASH]       = "KEY_KP_SLASH",

    // Locks
    [KEY_CAPS_LOCK]      = "KEY_CAPS_LOCK",
    [KEY_NUM_LOCK]       = "KEY_NUM_LOCK",
    [KEY_SCROLL_LOCK]    = "KEY_SCROLL_LOCK",

    // Extended / Multimedia
    [KEY_PRINT_SCREEN]   = "KEY_PRINT_SCREEN",
    [KEY_PAUSE]          = "KEY_PAUSE",
    [KEY_APPLICATION]    = "KEY_APPLICATION",
    [KEY_MUTE]           = "KEY_MUTE",
    [KEY_VOLUME_UP]      = "KEY_VOLUME_UP",
    [KEY_VOLUME_DOWN]    = "KEY_VOLUME_DOWN",
    [KEY_PLAY_PAUSE]     = "KEY_PLAY_PAUSE",
    [KEY_STOP]           = "KEY_STOP",
    [KEY_NEXT_TRACK]     = "KEY_NEXT_TRACK",
    [KEY_PREV_TRACK]     = "KEY_PREV_TRACK",

    // Launchers & Browser
    [KEY_CALCULATOR]     = "KEY_CALCULATOR",
    [KEY_MAIL]           = "KEY_MAIL",
    [KEY_BROWSER_HOME]   = "KEY_BROWSER_HOME",
    [KEY_MEDIA_SELECT]   = "KEY_MEDIA_SELECT",
    [KEY_LOCAL_BROWSER]  = "KEY_LOCAL_BROWSER",
    [KEY_AC_SEARCH]      = "KEY_AC_SEARCH",
    [KEY_AC_BOOKMARKS]   = "KEY_AC_BOOKMARKS",
    [KEY_AC_REFRESH]     = "KEY_AC_REFRESH",
    [KEY_AC_STOP]        = "KEY_AC_STOP",
    [KEY_AC_FORWARD]     = "KEY_AC_FORWARD",
    [KEY_AC_BACK]        = "KEY_AC_BACK",

    // System
    [KEY_SYSTEM_POWER]   = "KEY_SYSTEM_POWER",
    [KEY_SYSTEM_SLEEP]   = "KEY_SYSTEM_SLEEP",
    [KEY_SYSTEM_WAKE]    = "KEY_SYSTEM_WAKE",
};

static const keyboard_event_t keyboard_scanmap_ps2_set1[] = {
    // Pressed keys
    [0x01] = { true,    KEY_ESCAPE        },
    [0x02] = { true,    KEY_1             },
    [0x03] = { true,    KEY_2             },
    [0x04] = { true,    KEY_3             },
    [0x05] = { true,    KEY_4             },
    [0x06] = { true,    KEY_5             },
    [0x07] = { true,    KEY_6             },
    [0x08] = { true,    KEY_7             },
    [0x09] = { true,    KEY_8             },
    [0x0A] = { true,    KEY_9             },
    [0x0B] = { true,    KEY_0             },
    [0x0C] = { true,    KEY_MINUS         },
    [0x0D] = { true,    KEY_EQUAL         },
    [0x0E] = { true,    KEY_BACKSPACE     },
    [0x0F] = { true,    KEY_TAB           },
    [0x10] = { true,    KEY_Q             },
    [0x11] = { true,    KEY_W             },
    [0x12] = { true,    KEY_E             },
    [0x13] = { true,    KEY_R             },
    [0x14] = { true,    KEY_T             },
    [0x15] = { true,    KEY_Y             },
    [0x16] = { true,    KEY_U             },
    [0x17] = { true,    KEY_I             },
    [0x18] = { true,    KEY_O             },
    [0x19] = { true,    KEY_P             },
    [0x1A] = { true,    KEY_OPEN_BRACKET  },
    [0x1B] = { true,    KEY_CLOSE_BRACKET },
    [0x1C] = { true,    KEY_ENTER         },
    [0x1D] = { true,    KEY_LEFT_CTRL     },
    [0x1E] = { true,    KEY_A             },
    [0x1F] = { true,    KEY_S             },
    [0x20] = { true,    KEY_D             },
    [0x21] = { true,    KEY_F             },
    [0x22] = { true,    KEY_G             },
    [0x23] = { true,    KEY_H             },
    [0x24] = { true,    KEY_J             },
    [0x25] = { true,    KEY_K             },
    [0x26] = { true,    KEY_L             },
    [0x27] = { true,    KEY_SEMICOLON     },
    [0x28] = { true,    KEY_APOSTROPHE    },
    [0x29] = { true,    KEY_GRAVE         },
    [0x2A] = { true,    KEY_LEFT_SHIFT    },
    [0x2B] = { true,    KEY_BACKSLASH     },
    [0x2C] = { true,    KEY_Z             },
    [0x2D] = { true,    KEY_X             },
    [0x2E] = { true,    KEY_C             },
    [0x2F] = { true,    KEY_V             },
    [0x30] = { true,    KEY_B             },
    [0x31] = { true,    KEY_N             },
    [0x32] = { true,    KEY_M             },
    [0x33] = { true,    KEY_COMMA         },
    [0x34] = { true,    KEY_PERIOD        },
    [0x35] = { true,    KEY_SLASH         },
    [0x36] = { true,    KEY_RIGHT_SHIFT   },
    [0x37] = { true,    KEY_KP_MULT       },
    [0x38] = { true,    KEY_LEFT_ALT      },
    [0x39] = { true,    KEY_SPACE         },
    [0x3A] = { true,    KEY_CAPS_LOCK     },
    [0x3B] = { true,    KEY_F1            },
    [0x3C] = { true,    KEY_F2            },
    [0x3D] = { true,    KEY_F3            },
    [0x3E] = { true,    KEY_F4            },
    [0x3F] = { true,    KEY_F5            },
    [0x40] = { true,    KEY_F6            },
    [0x41] = { true,    KEY_F7            },
    [0x42] = { true,    KEY_F8            },
    [0x43] = { true,    KEY_F9            },
    [0x44] = { true,    KEY_F10           },
    [0x45] = { true,    KEY_NUM_LOCK      },
    [0x46] = { true,    KEY_SCROLL_LOCK   },
    [0x47] = { true,    KEY_KP_7          },
    [0x48] = { true,    KEY_KP_8          },
    [0x49] = { true,    KEY_KP_9          },
    [0x4A] = { true,    KEY_KP_MINUS      },
    [0x4B] = { true,    KEY_KP_4          },
    [0x4C] = { true,    KEY_KP_5          },
    [0x4D] = { true,    KEY_KP_6          },
    [0x4E] = { true,    KEY_KP_PLUS       },
    [0x4F] = { true,    KEY_KP_1          },
    [0x50] = { true,    KEY_KP_2          },
    [0x51] = { true,    KEY_KP_3          },
    [0x52] = { true,    KEY_KP_0          },
    [0x53] = { true,    KEY_KP_DOT        },
    [0x57] = { true,    KEY_F11           },
    [0x58] = { true,    KEY_F12           },
  
    // Released keys
    [0x81] = { false,    KEY_ESCAPE        },
    [0x82] = { false,    KEY_1             },
    [0x83] = { false,    KEY_2             },
    [0x84] = { false,    KEY_3             },
    [0x85] = { false,    KEY_4             },
    [0x86] = { false,    KEY_5             },
    [0x87] = { false,    KEY_6             },
    [0x88] = { false,    KEY_7             },
    [0x89] = { false,    KEY_8             },
    [0x8A] = { false,    KEY_9             },
    [0x8B] = { false,    KEY_0             },
    [0x8C] = { false,    KEY_MINUS         },
    [0x8D] = { false,    KEY_EQUAL         },
    [0x8E] = { false,    KEY_BACKSPACE     },
    [0x8F] = { false,    KEY_TAB           },
    [0x90] = { false,    KEY_Q             },
    [0x91] = { false,    KEY_W             },
    [0x92] = { false,    KEY_E             },
    [0x93] = { false,    KEY_R             },
    [0x94] = { false,    KEY_T             },
    [0x95] = { false,    KEY_Y             },
    [0x96] = { false,    KEY_U             },
    [0x97] = { false,    KEY_I             },
    [0x98] = { false,    KEY_O             },
    [0x99] = { false,    KEY_P             },
    [0x9A] = { false,    KEY_OPEN_BRACKET  },
    [0x9B] = { false,    KEY_CLOSE_BRACKET },
    [0x9C] = { false,    KEY_ENTER         },
    [0x9D] = { false,    KEY_LEFT_CTRL     },
    [0x9E] = { false,    KEY_A             },
    [0x9F] = { false,    KEY_S             },
    [0xA0] = { false,    KEY_D             },
    [0xA1] = { false,    KEY_F             },
    [0xA2] = { false,    KEY_G             },
    [0xA3] = { false,    KEY_H             },
    [0xA4] = { false,    KEY_J             },
    [0xA5] = { false,    KEY_K             },
    [0xA6] = { false,    KEY_L             },
    [0xA7] = { false,    KEY_SEMICOLON     },
    [0xA8] = { false,    KEY_APOSTROPHE    },
    [0xA9] = { false,    KEY_GRAVE         },
    [0xAA] = { false,    KEY_LEFT_SHIFT    },
    [0xAB] = { false,    KEY_BACKSLASH     },
    [0xAC] = { false,    KEY_Z             },
    [0xAD] = { false,    KEY_X             },
    [0xAE] = { false,    KEY_C             },
    [0xAF] = { false,    KEY_V             },
    [0xB0] = { false,    KEY_B             },
    [0xB1] = { false,    KEY_N             },
    [0xB2] = { false,    KEY_M             },
    [0xB3] = { false,    KEY_COMMA         },
    [0xB4] = { false,    KEY_PERIOD        },
    [0xB5] = { false,    KEY_SLASH         },
    [0xB6] = { false,    KEY_RIGHT_SHIFT   },
    [0xB7] = { false,    KEY_KP_MULT       },
    [0xB8] = { false,    KEY_LEFT_ALT      },
    [0xB9] = { false,    KEY_SPACE         },
    [0xBA] = { false,    KEY_CAPS_LOCK     },
    [0xBB] = { false,    KEY_F1            },
    [0xBC] = { false,    KEY_F2            },
    [0xBD] = { false,    KEY_F3            },
    [0xBE] = { false,    KEY_F4            },
    [0xBF] = { false,    KEY_F5            },
    [0xC0] = { false,    KEY_F6            },
    [0xC1] = { false,    KEY_F7            },
    [0xC2] = { false,    KEY_F8            },
    [0xC3] = { false,    KEY_F9            },
    [0xC4] = { false,    KEY_F10           },
    [0xC5] = { false,    KEY_NUM_LOCK      },
    [0xC6] = { false,    KEY_SCROLL_LOCK   },
    [0xC7] = { false,    KEY_KP_7          },
    [0xC8] = { false,    KEY_KP_8          },
    [0xC9] = { false,    KEY_KP_9          },
    [0xCA] = { false,    KEY_KP_MINUS      },
    [0xCB] = { false,    KEY_KP_4          },
    [0xCC] = { false,    KEY_KP_5          },
    [0xCD] = { false,    KEY_KP_6          },
    [0xCE] = { false,    KEY_KP_PLUS       },
    [0xCF] = { false,    KEY_KP_1          },
    [0xD0] = { false,    KEY_KP_2          },
    [0xD1] = { false,    KEY_KP_3          },
    [0xD2] = { false,    KEY_KP_0          },
    [0xD3] = { false,    KEY_KP_DOT        },
    [0xD7] = { false,    KEY_F11           },
    [0xD8] = { false,    KEY_F12           },
};

static const keyboard_event_t keyboard_scanmap_ps2_set1_escaped[] = {
    // Pressed keys
    [0x10] = { true,    KEY_PREV_TRACK      },
    [0x19] = { true,    KEY_NEXT_TRACK      },
    [0x1C] = { true,    KEY_KP_ENTER        },
    [0x1D] = { true,    KEY_RIGHT_CTRL      },
    [0x20] = { true,    KEY_MUTE            },
    [0x21] = { true,    KEY_CALCULATOR      },
    [0x22] = { true,    KEY_PLAY_PAUSE      },
    [0x24] = { true,    KEY_STOP            },
    [0x2E] = { true,    KEY_VOLUME_DOWN     },
    [0x30] = { true,    KEY_VOLUME_UP       },
    [0x32] = { true,    KEY_BROWSER_HOME    },
    [0x35] = { true,    KEY_KP_SLASH        },
    [0x37] = { true,    KEY_PRINT_SCREEN    },
    [0x38] = { true,    KEY_RIGHT_ALT       },
    [0x46] = { true,    KEY_PAUSE           },
    [0x47] = { true,    KEY_HOME            },
    [0x48] = { true,    KEY_UP_ARROW        },
    [0x49] = { true,    KEY_PAGE_UP         },
    [0x4B] = { true,    KEY_LEFT_ARROW      },
    [0x4D] = { true,    KEY_RIGHT_ARROW     },
    [0x4F] = { true,    KEY_END             },
    [0x50] = { true,    KEY_DOWN_ARROW      },
    [0x51] = { true,    KEY_PAGE_DOWN       },
    [0x52] = { true,    KEY_INSERT          },
    [0x53] = { true,    KEY_DELETE          },
    [0x5B] = { true,    KEY_LEFT_META       },
    [0x5C] = { true,    KEY_RIGHT_META      },
    [0x5D] = { true,    KEY_APPLICATION     },
    [0x5E] = { true,    KEY_SYSTEM_POWER    },
    [0x5F] = { true,    KEY_SYSTEM_SLEEP    },
    [0x63] = { true,    KEY_SYSTEM_WAKE     },
    [0x65] = { true,    KEY_AC_SEARCH       },
    [0x66] = { true,    KEY_AC_BOOKMARKS    },
    [0x67] = { true,    KEY_AC_REFRESH      },
    [0x68] = { true,    KEY_AC_STOP         },
    [0x69] = { true,    KEY_AC_FORWARD      },
    [0x6A] = { true,    KEY_AC_BACK         },
    [0x6B] = { true,    KEY_LOCAL_BROWSER   },
    [0x6C] = { true,    KEY_MAIL            },
    [0x6D] = { true,    KEY_MEDIA_SELECT    },

    // Released keys
    [0x90] = { false,   KEY_PREV_TRACK      },
    [0x99] = { false,   KEY_NEXT_TRACK      },
    [0x9C] = { false,   KEY_KP_ENTER        },
    [0x9D] = { false,   KEY_RIGHT_CTRL      },
    [0xA0] = { false,   KEY_MUTE            },
    [0xA1] = { false,   KEY_CALCULATOR      },
    [0xA2] = { false,   KEY_PLAY_PAUSE      },
    [0xA4] = { false,   KEY_STOP            },
    [0xAE] = { false,   KEY_VOLUME_DOWN     },
    [0xB0] = { false,   KEY_VOLUME_UP       },
    [0xB2] = { false,   KEY_BROWSER_HOME    },
    [0xB5] = { false,   KEY_KP_SLASH        },
    [0xB7] = { false,   KEY_PRINT_SCREEN    },
    [0xB8] = { false,   KEY_RIGHT_ALT       },
    [0xC6] = { false,   KEY_PAUSE           },
    [0xC7] = { false,   KEY_HOME            },
    [0xC8] = { false,   KEY_UP_ARROW        },
    [0xC9] = { false,   KEY_PAGE_UP         },
    [0xCB] = { false,   KEY_LEFT_ARROW      },
    [0xCD] = { false,   KEY_RIGHT_ARROW     },
    [0xCF] = { false,   KEY_END             },
    [0xD0] = { false,   KEY_DOWN_ARROW      },
    [0xD1] = { false,   KEY_PAGE_DOWN       },
    [0xD2] = { false,   KEY_INSERT          },
    [0xD3] = { false,   KEY_DELETE          },
    [0xDB] = { false,   KEY_LEFT_META       },
    [0xDC] = { false,   KEY_RIGHT_META      },
    [0xDD] = { false,   KEY_APPLICATION     },
    [0xDE] = { false,   KEY_SYSTEM_POWER    },
    [0xDF] = { false,   KEY_SYSTEM_SLEEP    },
    [0xE3] = { false,   KEY_SYSTEM_WAKE     },
    [0xE5] = { false,   KEY_AC_SEARCH       },
    [0xE6] = { false,   KEY_AC_BOOKMARKS    },
    [0xE7] = { false,   KEY_AC_REFRESH      },
    [0xE8] = { false,   KEY_AC_STOP         },
    [0xE9] = { false,   KEY_AC_FORWARD      },
    [0xEA] = { false,   KEY_AC_BACK         },
    [0xEB] = { false,   KEY_LOCAL_BROWSER   },
    [0xEC] = { false,   KEY_MAIL            },
    [0xED] = { false,   KEY_MEDIA_SELECT    },
};

void keyboard_init(keyboard_t *keyboard, keyboard_type_t type)
{
  if (!keyboard) return;

  keyboard->type      = type;
  keyboard->_internal = 0;
  memset(keyboard->state, 0, _KEY_MAX * sizeof(keycode_t));
  
  return;
}

void keyboard_update(keyboard_t *keyboard, keyboard_event_t event)
{
  if (!keyboard || event.key == KEY_NONE
      || event.key < 0 || event.key >= _KEY_MAX) return;

  keyboard->state[event.key] = event.pressed;
  
  return;
}

// Implements keyboard_event_from_scancode for keyboards of type
// KEYBOARD_TYPE_PS2_SET1
static keyboard_event_t
keyboard_event_from_scancode_ps2_set1(keyboard_t *keyboard,
                                      u8_t        scancode)
{
  if (!keyboard)
    goto exit;
  
  if (keyboard->_internal == 1)
  {
    keyboard->_internal = 0;
    return keyboard_scanmap_ps2_set1_escaped[scancode];
  }
  else
  {
    if (scancode == 0xE0)
    {
      keyboard->_internal = 1;
      goto exit;
    }
    
    return keyboard_scanmap_ps2_set1[scancode];
  }

 exit:
  return (keyboard_event_t) {
    .pressed = false,
    .key     = KEY_NONE,
  };
}

keyboard_event_t keyboard_event_from_scancode(keyboard_t *keyboard,
                                              u8_t        scancode)
{
  if (!keyboard)
    goto exit;

  switch (keyboard->type)
  {
  case KEYBOARD_TYPE_PS2_SET1:
    return keyboard_event_from_scancode_ps2_set1(keyboard, scancode);
    break;
  case KEYBOARD_TYPE_PS2_SET2:
  case KEYBOARD_TYPE_PS2_SET3:
  default:
    // Do nothing
    break;
  }

 exit:
  return (keyboard_event_t) {
    .pressed = false,
    .key     = KEY_NONE,
  };
}

const char* keyboard_string_from_keycode(keycode_t key)
{
  if (key < _KEY_MAX && key >= 0 && keycode_str[key] != NULL)
  {
    return keycode_str[key];
  }

  return "KEY_UNKNOWN";
}

void keyboard_set_active(keyboard_t *keyboard)
{
  keyboard_active = keyboard;
  return;
}

keyboard_t *keyboard_get_active(void)
{
  return keyboard_active;
}
