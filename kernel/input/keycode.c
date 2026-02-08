// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/input/keycodes.h>   // implements
#include <libk/stddef.h>

static const char* keycode_str[_KEY_MAX] = {
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

const char* keycode_from_string(keycode_t key)
{
  if (key < _KEY_MAX && key >= 0 && keycode_str[key] != NULL)
  {
    return keycode_str[key];
  }

  return "KEY_UNKNOWN";
}
