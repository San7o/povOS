// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/input/keymap.h>

input_keymap_t input_keymap_us = {
  // Letters
  [KEY_A] = { 'a', 'A' },
  [KEY_B] = { 'b', 'B' },
  [KEY_C] = { 'c', 'C' },
  [KEY_D] = { 'd', 'D' },
  [KEY_E] = { 'e', 'E' },
  [KEY_F] = { 'f', 'F' },
  [KEY_G] = { 'g', 'G' },
  [KEY_H] = { 'h', 'H' },
  [KEY_I] = { 'i', 'I' },
  [KEY_J] = { 'j', 'J' },
  [KEY_K] = { 'k', 'K' },
  [KEY_L] = { 'l', 'L' },
  [KEY_M] = { 'm', 'M' },
  [KEY_N] = { 'n', 'N' },
  [KEY_O] = { 'o', 'O' },
  [KEY_P] = { 'p', 'P' },
  [KEY_Q] = { 'q', 'Q' },
  [KEY_R] = { 'r', 'R' },
  [KEY_S] = { 's', 'S' },
  [KEY_T] = { 't', 'T' },
  [KEY_U] = { 'u', 'U' },
  [KEY_V] = { 'v', 'V' },
  [KEY_W] = { 'w', 'W' },
  [KEY_X] = { 'x', 'X' },
  [KEY_Y] = { 'y', 'Y' },
  [KEY_Z] = { 'z', 'Z' },

  // Numbers (top row)
  [KEY_1] = { '1', '!' },
  [KEY_2] = { '2', '@' },
  [KEY_3] = { '3', '#' },
  [KEY_4] = { '4', '$' },
  [KEY_5] = { '5', '%' },
  [KEY_6] = { '6', '^' },
  [KEY_7] = { '7', '&' },
  [KEY_8] = { '8', '*' },
  [KEY_9] = { '9', '(' },
  [KEY_0] = { '0', ')' },

  // Symbols
  [KEY_GRAVE]         = { '`', '~' },
  [KEY_MINUS]         = { '-', '_' },
  [KEY_EQUAL]         = { '=', '+' },
  [KEY_OPEN_BRACKET]  = { '[', '{' },
  [KEY_CLOSE_BRACKET] = { ']', '}' },
  [KEY_BACKSLASH]     = { '\\', '|' },
  [KEY_SEMICOLON]     = { ';', ':' },
  [KEY_APOSTROPHE]    = { '\'', '"' },
  [KEY_COMMA]         = { ',', '<' },
  [KEY_PERIOD]        = { '.', '>' },
  [KEY_SLASH]         = { '/', '?' },

  // Space
  [KEY_SPACE] = { ' ', ' ' },

  // Keypad (no shift variants here)
  [KEY_KP_0] = { '0', '0' },
  [KEY_KP_1] = { '1', '1' },
  [KEY_KP_2] = { '2', '2' },
  [KEY_KP_3] = { '3', '3' },
  [KEY_KP_4] = { '4', '4' },
  [KEY_KP_5] = { '5', '5' },
  [KEY_KP_6] = { '6', '6' },
  [KEY_KP_7] = { '7', '7' },
  [KEY_KP_8] = { '8', '8' },
  [KEY_KP_9] = { '9', '9' },

  [KEY_KP_MINUS] = { '-', '-' },
  [KEY_KP_PLUS]  = { '+', '+' },
  [KEY_KP_MULT]  = { '*', '*' },
  [KEY_KP_DIV]   = { '/', '/' },
  [KEY_KP_SLASH] = { '/', '/' },
  [KEY_KP_DOT]   = { '.', '.' },
};
