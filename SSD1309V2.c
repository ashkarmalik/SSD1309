/*
 * File:   SSD1309V2.c
 * Author: Apollo
 *
 * Created on February 23, 2019, 6:07 PM
 */

#include <xc.h>
#include "SSD1309V2.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define PROGMEM /* empty */
#define pgm_read_byte(x) (*(x))
#define pgm_read_word(x) (*(x))
#define pgm_read_float(x) (*(x))

#define _XTAL_FREQ 10000000
/*#############################################################################
###################################### fonts ##################################
#############################################################################*/

static const char  font[241][5] =			// small 5x7 font
{				
	{0x00,0x00,0x00,0x00,0x00},		//   (  0)    - 0x20 Space
	{0x00,0x00,0x4F,0x00,0x00},		//   (  1)  ! - 0x21 Exclamation Mark
	{0x00,0x07,0x00,0x07,0x00},		//   (  2)  " - 0x22 Quotation Mark
	{0x14,0x7F,0x14,0x7F,0x14},		//   (  3)  # - 0x23 Number Sign
	{0x24,0x2A,0x7F,0x2A,0x12},		//   (  4)  $ - 0x24 Dollar Sign
	{0x23,0x13,0x08,0x64,0x62},		//   (  5)  % - 0x25 Percent Sign
	{0x36,0x49,0x55,0x22,0x50},		//   (  6)  & - 0x26 Ampersand
	{0x00,0x05,0x03,0x00,0x00},		//   (  7)  ' - 0x27 Apostrophe
	{0x00,0x1C,0x22,0x41,0x00},		//   (  8)  ( - 0x28 Left Parenthesis
	{0x00,0x41,0x22,0x1C,0x00},		//   (  9)  ) - 0x29 Right Parenthesis
	{0x14,0x08,0x3E,0x08,0x14},		//   ( 10)  * - 0x2A Asterisk
	{0x08,0x08,0x3E,0x08,0x08},		//   ( 11)  + - 0x2B Plus Sign
	{0x00,0x50,0x30,0x00,0x00},		//   ( 12)  , - 0x2C Comma
	{0x08,0x08,0x08,0x08,0x08},		//   ( 13)  - - 0x2D Hyphen-Minus
	{0x00,0x60,0x60,0x00,0x00},		//   ( 14)  . - 0x2E Point
	{0x20,0x10,0x08,0x04,0x02},		//   ( 15)  / - 0x2F Solidus
	{0x3E,0x51,0x49,0x45,0x3E},		//   ( 16)  0 - 0x30 Digit Zero
	{0x00,0x42,0x7F,0x40,0x00},		//   ( 17)  1 - 0x31 Digit One
	{0x42,0x61,0x51,0x49,0x46},		//   ( 18)  2 - 0x32 Digit Two
	{0x21,0x41,0x45,0x4B,0x31},		//   ( 19)  3 - 0x33 Digit Three
	{0x18,0x14,0x12,0x7F,0x10},		//   ( 20)  4 - 0x34 Digit Four
	{0x27,0x45,0x45,0x45,0x39},		//   ( 21)  5 - 0x35 Digit Five
	{0x3C,0x4A,0x49,0x49,0x30},		//   ( 22)  6 - 0x36 Digit Six
	{0x01,0x71,0x09,0x05,0x03},		//   ( 23)  7 - 0x37 Digit Seven
	{0x36,0x49,0x49,0x49,0x36},		//   ( 24)  8 - 0x38 Digit Eight
	{0x06,0x49,0x49,0x29,0x1E},		//   ( 25)  9 - 0x39 Digit Nine
	{0x00,0x36,0x36,0x00,0x00},		//   ( 26)  : - 0x3A Colon
	{0x00,0x56,0x36,0x00,0x00},		//   ( 27)  ; - 0x3B Semicolon
	{0x08,0x14,0x22,0x41,0x00},		//   ( 28)  < - 0x3C Less-Than Sign
	{0x14,0x14,0x14,0x14,0x14},		//   ( 29)  = - 0x3D Equals Sign
	{0x00,0x41,0x22,0x14,0x08},		//   ( 30)  > - 0x3E Greater-Than Sign
	{0x02,0x01,0x51,0x09,0x06},		//   ( 31)  ? - 0x3F Question Mark
	{0x32,0x49,0x79,0x41,0x3E},		//   ( 32)  @ - 0x40 Commercial At
	{0x7E,0x11,0x11,0x11,0x7E},		//   ( 33)  A - 0x41 Latin Capital Letter A
	{0x7F,0x49,0x49,0x49,0x36},		//   ( 34)  B - 0x42 Latin Capital Letter B
	{0x3E,0x41,0x41,0x41,0x22},		//   ( 35)  C - 0x43 Latin Capital Letter C
	{0x7F,0x41,0x41,0x22,0x1C},		//   ( 36)  D - 0x44 Latin Capital Letter D
	{0x7F,0x49,0x49,0x49,0x41},		//   ( 37)  E - 0x45 Latin Capital Letter E
	{0x7F,0x09,0x09,0x09,0x01},		//   ( 38)  F - 0x46 Latin Capital Letter F
	{0x3E,0x41,0x49,0x49,0x7A},		//   ( 39)  G - 0x47 Latin Capital Letter G
	{0x7F,0x08,0x08,0x08,0x7F},		//   ( 40)  H - 0x48 Latin Capital Letter H
	{0x00,0x41,0x7F,0x41,0x00},		//   ( 41)  I - 0x49 Latin Capital Letter I
	{0x20,0x40,0x41,0x3F,0x01},		//   ( 42)  J - 0x4A Latin Capital Letter J
	{0x7F,0x08,0x14,0x22,0x41},		//   ( 43)  K - 0x4B Latin Capital Letter K
	{0x7F,0x40,0x40,0x40,0x40},		//   ( 44)  L - 0x4C Latin Capital Letter L
	{0x7F,0x02,0x0C,0x02,0x7F},		//   ( 45)  M - 0x4D Latin Capital Letter M
	{0x7F,0x04,0x08,0x10,0x7F},		//   ( 46)  N - 0x4E Latin Capital Letter N
	{0x3E,0x41,0x41,0x41,0x3E},		//   ( 47)  O - 0x4F Latin Capital Letter O
	{0x7F,0x09,0x09,0x09,0x06},		//   ( 48)  P - 0x50 Latin Capital Letter P
	{0x3E,0x41,0x51,0x21,0x5E},		//   ( 49)  Q - 0x51 Latin Capital Letter Q
	{0x7F,0x09,0x19,0x29,0x46},		//   ( 50)  R - 0x52 Latin Capital Letter R
	{0x46,0x49,0x49,0x49,0x31},		//   ( 51)  S - 0x53 Latin Capital Letter S
	{0x01,0x01,0x7F,0x01,0x01},		//   ( 52)  T - 0x54 Latin Capital Letter T
	{0x3F,0x40,0x40,0x40,0x3F},		//   ( 53)  U - 0x55 Latin Capital Letter U
	{0x1F,0x20,0x40,0x20,0x1F},		//   ( 54)  V - 0x56 Latin Capital Letter V
	{0x3F,0x40,0x38,0x40,0x3F},		//   ( 55)  W - 0x57 Latin Capital Letter W
	{0x63,0x14,0x08,0x14,0x63},		//   ( 56)  X - 0x58 Latin Capital Letter X
	{0x07,0x08,0x70,0x08,0x07},		//   ( 57)  Y - 0x59 Latin Capital Letter Y
	{0x61,0x51,0x49,0x45,0x43},		//   ( 58)  Z - 0x5A Latin Capital Letter Z
	{0x00,0x7F,0x41,0x41,0x00},		//   ( 59)  [ - 0x5B Left Square Bracket
	{0x02,0x04,0x08,0x10,0x20},		//   ( 58)  \ - 0x5C Reverse Solidus
	{0x00,0x41,0x41,0x7F,0x00},		//   ( 61)  ] - 0x5D Right Square Bracket
	{0x04,0x02,0x01,0x02,0x04},		//   ( 62)  ^ - 0x5E Circumflex Accent
	{0x40,0x40,0x40,0x40,0x40},		//   ( 63)  _ - 0x5F Low Line
	{0x01,0x02,0x04,0x00,0x00},		//   ( 64)  ` - 0x60 Grave Accent
	{0x20,0x54,0x54,0x54,0x78},		//   ( 65)  a - 0x61 Latin Small Letter A
	{0x7F,0x48,0x44,0x44,0x38},		//   ( 66)  b - 0x62 Latin Small Letter B
	{0x38,0x44,0x44,0x44,0x20},		//   ( 67)  c - 0x63 Latin Small Letter C
	{0x38,0x44,0x44,0x48,0x7F},		//   ( 68)  d - 0x64 Latin Small Letter D
	{0x38,0x54,0x54,0x54,0x18},		//   ( 69)  e - 0x65 Latin Small Letter E
	{0x08,0x7E,0x09,0x01,0x02},		//   ( 70)  f - 0x66 Latin Small Letter F
	{0x08,0x54,0x54,0x54,0x3C},		//   ( 71)  g - 0x67 Latin Small Letter G
	{0x7F,0x08,0x04,0x04,0x78},		//   ( 72)  h - 0x68 Latin Small Letter H
	{0x00,0x44,0x7D,0x40,0x00},		//   ( 73)  i - 0x69 Latin Small Letter I
	{0x20,0x40,0x44,0x3D,0x00},		//   ( 74)  j - 0x6A Latin Small Letter J
	{0x7F,0x10,0x28,0x44,0x00},		//   ( 75)  k - 0x6B Latin Small Letter K
	{0x00,0x41,0x7F,0x40,0x00},		//   ( 76)  l - 0x6C Latin Small Letter L
	{0x7C,0x04,0x18,0x04,0x7C},		//   ( 77)  m - 0x6D Latin Small Letter M
	{0x7C,0x08,0x04,0x04,0x78},		//   ( 78)  n - 0x6E Latin Small Letter N
	{0x38,0x44,0x44,0x44,0x38},		//   ( 79)  o - 0x6F Latin Small Letter O
	{0x7C,0x14,0x14,0x14,0x08},		//   ( 80)  p - 0x70 Latin Small Letter P
	{0x08,0x14,0x14,0x18,0x7C},		//   ( 81)  q - 0x71 Latin Small Letter Q
	{0x7C,0x08,0x04,0x04,0x08},		//   ( 82)  r - 0x72 Latin Small Letter R
	{0x48,0x54,0x54,0x54,0x20},		//   ( 83)  s - 0x73 Latin Small Letter S
	{0x04,0x3F,0x44,0x40,0x20},		//   ( 84)  t - 0x74 Latin Small Letter T
	{0x3C,0x40,0x40,0x20,0x7C},		//   ( 85)  u - 0x75 Latin Small Letter U
	{0x1C,0x20,0x40,0x20,0x1C},		//   ( 86)  v - 0x76 Latin Small Letter V
	{0x3C,0x40,0x30,0x40,0x3C},		//   ( 87)  w - 0x77 Latin Small Letter W
	{0x44,0x28,0x10,0x28,0x44},		//   ( 88)  x - 0x78 Latin Small Letter X
	{0x0C,0x50,0x50,0x50,0x3C},		//   ( 89)  y - 0x79 Latin Small Letter Y
	{0x44,0x64,0x54,0x4C,0x44},		//   ( 90)  z - 0x7A Latin Small Letter Z
	{0x00,0x08,0x36,0x41,0x00},		//   ( 91)  { - 0x7B Left Curly Bracket
	{0x00,0x00,0x7F,0x00,0x00},		//   ( 92)  | - 0x7C Vertical Line
	{0x00,0x41,0x36,0x08,0x00},		//   ( 93)  } - 0x7D Right Curly Bracket
	{0x02,0x01,0x02,0x04,0x02},		//   ( 94)  ~ - 0x7E Tilde
	{0x00,0x00,0x00,0x00,0x00},		//   ( 95)    - 0xA0 No-Break Space
	{0x00,0x00,0x79,0x00,0x00},		//   ( 96)  ! - 0xA1 Inverted Exclamation Mark
	{0x18,0x24,0x74,0x2E,0x24},		//   ( 97)  c - 0xA2 Cent Sign
	{0x48,0x7E,0x49,0x42,0x40},		//   ( 98)  L - 0xA3 Pound Sign
	{0x5D,0x22,0x22,0x22,0x5D},		//   ( 99)  o - 0xA4 Currency Sign
	{0x15,0x16,0x7C,0x16,0x15},		//   (100)  Y - 0xA5 Yen Sign
	{0x00,0x00,0x77,0x00,0x00},		//   (101)  | - 0xA6 Broken Bar
	{0x0A,0x55,0x55,0x55,0x28},		//   (102)  § - 0xA7 Section Sign
	{0x00,0x01,0x00,0x01,0x00},		//   (103)  " - 0xA8 Diaeresis
	{0x3E,0x41,0x41,0x41,0x22},     //   (104) (C)- 0xA9 Copyright
	{0x00,0x0A,0x0D,0x0A,0x04},		//   (105)    - 0xAA Feminine Ordinal Indicator
	{0x08,0x14,0x2A,0x14,0x22},		//   (106) << - 0xAB Left-Pointing Double Angle Quotation Mark
	{0x04,0x04,0x04,0x04,0x1C},		//   (107)    - 0xAC Not Sign
	{0x00,0x08,0x08,0x08,0x00},		//   (108)  - - 0xAD Soft Hyphen
	{0x7F,0x09,0x19,0x29,0x46},		//   (109) (R)- 0xAE Registered
	{0x01,0x01,0x01,0x01,0x01},		//   (110)    - 0xAF Macron
	{0x00,0x02,0x05,0x02,0x00},		//   (111)    - 0xB0 Degree Sign
	{0x44,0x44,0x5F,0x44,0x44},		//   (112) +- - 0xB1 Plus-Minus Sign
	{0x00,0x00,0x1D,0x17,0x00},		//	 (113)  ² - 0xB2 SUPERSCRIPT TWO
	{0x00,0x00,0x15,0x1F,0x00},		//	 (114)  ³ - 0xB3 SUPERSCRIPT THREE
	{0x00,0x00,0x04,0x02,0x01},		//   (115)  ` - 0xB4 Acute Accent
	{0x7E,0x20,0x20,0x10,0x3E},		//   (116)  u - 0xB5 Micro Sign
	{0x06,0x0F,0x7F,0x00,0x7F},		//   (117)    - 0xB6 Pilcrow Sign
	{0x00,0x18,0x18,0x00,0x00},		//   (118)  . - 0xB7 Middle Dot
	{0x00,0x40,0x50,0x20,0x00},		//   (119)    - 0xB8 Cedilla
	{0x00,0x00,0x02,0x0F,0x00},		//	 (120)  1 - 0xB9 SUPERSCRIPT ONE
	{0x00,0x0A,0x0D,0x0A,0x00},		//   (121)    - 0xBA Masculine Ordinal Indicator
	{0x22,0x14,0x2A,0x14,0x08},		//   (122) >> - 0xBB Right-Pointing Double Angle Quotation Mark
	{0x17,0x08,0x34,0x2A,0x7D},		//   (123) /4 - 0xBC Vulgar Fraction One Quarter
	{0x17,0x08,0x04,0x6A,0x59},		//   (124) /2 - 0xBD Vulgar Fraction One Half
	{0x25,0x12,0x08,0x34,0x62},		//	 (125) 3/4- 0xBE Vulgar Fraction Three Quarter
	{0x30,0x48,0x45,0x40,0x20},		//   (126)  ? - 0xBF Inverted Question Mark
	{0x70,0x29,0x26,0x28,0x70},		//   (127) `A - 0xC0 Latin Capital Letter A with Grave
	{0x70,0x28,0x26,0x29,0x70},		//   (128) 'A - 0xC1 Latin Capital Letter A with Acute
	{0x70,0x2A,0x25,0x2A,0x70},		//   (129) ^A - 0xC2 Latin Capital Letter A with Circumflex
	{0x72,0x29,0x26,0x29,0x70},		//   (130) ~A - 0xC3 Latin Capital Letter A with Tilde
	{0x70,0x29,0x24,0x29,0x70},		//   (131) "A - 0xC4 Latin Capital Letter A with Diaeresis
	{0x70,0x2A,0x2D,0x2A,0x70},		//   (132)  A - 0xC5 Latin Capital Letter A with Ring Above
	{0x7E,0x11,0x7F,0x49,0x49},		//   (133) AE - 0xC6 Latin Capital Letter Ae
	{0x0E,0x51,0x51,0x71,0x11},		//   (134)  C - 0xC7 Latin Capital Letter C with Cedilla
	{0x7C,0x55,0x56,0x54,0x44},		//   (135) `E - 0xC8 Latin Capital Letter E with Grave
	{0x7C,0x55,0x56,0x54,0x44},		//   (136) 'E - 0xC9 Latin Capital Letter E with Acute
	{0x7C,0x56,0x55,0x56,0x44},		//   (137) ^E - 0xCA Latin Capital Letter E with Circumflex
	{0x7C,0x55,0x54,0x55,0x44},		//   (138) "E - 0xCB Latin Capital Letter E with Diaeresis
	{0x00,0x45,0x7E,0x44,0x00},		//   (139) `I - 0xCC Latin Capital Letter I with Grave
	{0x00,0x44,0x7E,0x45,0x00},		//   (140) 'I - 0xCD Latin Capital Letter I with Acute
	{0x00,0x46,0x7D,0x46,0x00},		//   (141) ^I - 0xCE Latin Capital Letter I with Circumflex
	{0x00,0x45,0x7C,0x45,0x00},		//   (142) "I - 0xCF Latin Capital Letter I with Diaeresis
	{0x7F,0x49,0x49,0x41,0x3E},		//   (143)  D - 0xD0 Latin Capital Letter Eth
	{0x7C,0x0A,0x11,0x22,0x7D},		//   (144) ~N - 0xD1 Latin Capital Letter N with Tilde
	{0x38,0x45,0x46,0x44,0x38},		//   (145) `O - 0xD2 Latin Capital Letter O with Grave
	{0x38,0x44,0x46,0x45,0x38},		//   (146) 'O - 0xD3 Latin Capital Letter O with Acute
	{0x38,0x46,0x45,0x46,0x38},		//   (147) ^O - 0xD4 Latin Capital Letter O with Circumflex
	{0x38,0x46,0x45,0x46,0x39},		//   (148) ~O - 0xD5 Latin Capital Letter O with Tilde
	{0x38,0x45,0x44,0x45,0x38},		//   (149) "O - 0xD6 Latin Capital Letter O with Diaeresis
	{0x22,0x14,0x08,0x14,0x22},		//   (150)  x - 0xD7 Multiplcation Sign
	{0x2E,0x51,0x49,0x45,0x3A},		//   (151)  O - 0xD8 Latin Capital Letter O with Stroke
	{0x3C,0x41,0x42,0x40,0x3C},		//   (152) `U - 0xD9 Latin Capital Letter U with Grave
	{0x3C,0x40,0x42,0x41,0x3C},		//   (153) 'U - 0xDA Latin Capital Letter U with Acute
	{0x3C,0x42,0x41,0x42,0x3C},		//   (154) ^U - 0xDB Latin Capital Letter U with Circumflex
	{0x3C,0x41,0x40,0x41,0x3C},		//   (155) "U - 0xDC Latin Capital Letter U with Diaeresis
	{0x0C,0x10,0x62,0x11,0x0C},		//   (156) `Y - 0xDD Latin Capital Letter Y with Acute
	{0x7F,0x12,0x12,0x12,0x0C},		//   (157)  P - 0xDE Latin Capital Letter Thom
	{0x40,0x3E,0x01,0x49,0x36},		//   (158)  B - 0xDF Latin Capital Letter Sharp S
	{0x20,0x55,0x56,0x54,0x78},		//   (159) `a - 0xE0 Latin Small Letter A with Grave
	{0x20,0x54,0x56,0x55,0x78},		//   (160) 'a - 0xE1 Latin Small Letter A with Acute
	{0x20,0x56,0x55,0x56,0x78},		//   (161) ^a - 0xE2 Latin Small Letter A with Circumflex
	{0x20,0x55,0x56,0x55,0x78},		//   (162) ~a - 0xE3 Latin Small Letter A with Tilde
	{0x20,0x55,0x54,0x55,0x78},		//   (163) "a - 0xE4 Latin Small Letter A with Diaeresis
	{0x20,0x56,0x57,0x56,0x78},		//   (164)  a - 0xE5 Latin Small Letter A with Ring Above
	{0x24,0x54,0x78,0x54,0x58},		//   (165) ae - 0xE6 Latin Small Letter Ae
	{0x0C,0x52,0x52,0x72,0x13},		//   (166)  c - 0xE7 Latin Small Letter c with Cedilla
	{0x38,0x55,0x56,0x54,0x18},		//   (167) `e - 0xE8 Latin Small Letter E with Grave
	{0x38,0x54,0x56,0x55,0x18},		//   (168) 'e - 0xE9 Latin Small Letter E with Acute
	{0x38,0x56,0x55,0x56,0x18},		//   (169) ^e - 0xEA Latin Small Letter E with Circumflex
	{0x38,0x55,0x54,0x55,0x18},		//   (170) "e - 0xEB Latin Small Letter E with Diaeresis
	{0x00,0x49,0x7A,0x40,0x00},		//   (171) `i - 0xEC Latin Small Letter I with Grave
	{0x00,0x48,0x7A,0x41,0x00},		//   (172) 'i - 0xED Latin Small Letter I with Acute
	{0x00,0x4A,0x79,0x42,0x00},		//   (173) ^i - 0xEE Latin Small Letter I with Circumflex
	{0x00,0x4A,0x78,0x42,0x00},		//   (174) "i - 0xEF Latin Small Letter I with Diaeresis
	{0x31,0x4A,0x4E,0x4A,0x30},		//   (175)    - 0xF0 Latin Small Letter Eth
	{0x7A,0x11,0x0A,0x09,0x70},		//   (176) ~n - 0xF1 Latin Small Letter N with Tilde
	{0x30,0x49,0x4A,0x48,0x30},		//   (177) `o - 0xF2 Latin Small Letter O with Grave
	{0x30,0x48,0x4A,0x49,0x30},		//   (178) 'o - 0xF3 Latin Small Letter O with Acute
	{0x30,0x4A,0x49,0x4A,0x30},		//   (179) ^o - 0xF4 Latin Small Letter O with Circumflex
	{0x30,0x4A,0x49,0x4A,0x31},		//   (180) ~o - 0xF5 Latin Small Letter O with Tilde
	{0x30,0x4A,0x48,0x4A,0x30},		//   (181) "o - 0xF6 Latin Small Letter O with Diaeresis
	{0x08,0x08,0x2A,0x08,0x08},		//   (182)  + - 0xF7 Division Sign
	{0x38,0x64,0x54,0x4C,0x38},		//   (183)  o - 0xF8 Latin Small Letter O with Stroke
	{0x38,0x41,0x42,0x20,0x78},		//   (184) `u - 0xF9 Latin Small Letter U with Grave
	{0x38,0x40,0x42,0x21,0x78},		//   (185) 'u - 0xFA Latin Small Letter U with Acute
	{0x38,0x42,0x41,0x22,0x78},		//   (186) ^u - 0xFB Latin Small Letter U with Circumflex
	{0x38,0x42,0x40,0x22,0x78},		//   (187) "u - 0xFC Latin Small Letter U with Diaeresis
	{0x0C,0x50,0x52,0x51,0x3C},		//   (188) 'y - 0xFD Latin Small Letter Y with Acute
	{0x7E,0x14,0x14,0x14,0x08},		//   (189)  p - 0xFE Latin Small Letter Thom
	{0x0C,0x51,0x50,0x51,0x3C}		//   (190) "y - 0xFF Latin Small Letter Y with Diaeresis
};										  



static const char  bigDigit[15][26] =			// big 13x16 font
{				
	{0x00,0x00,0x80,0x80,0x80,0xF0,0xF0,0x80,0x80,0x80,0x00,0x00,0x00, 0x00,0x00,0x01,0x01,0x01,0x0F,0x0F,0x01,0x01,0x01,0x00,0x00,0x00},	//   (  0)  + - 0x2B Plus Sign
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0xB8,0xF8,0x78,0x00,0x00,0x00,0x00,0x00},	//   (  1)  , - 0x2C Comma
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00, 0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00},	//   (  2)  - - 0x2D Minus
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0x00,0x00,0x00,0x00,0x00},	//   (  3)  . - 0x2E Point
	{0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x06, 0x00,0x18,0x1C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00},	//   (  4)  / - 0x2F Solidus
	{0x00,0xF8,0xFE,0x06,0x03,0x83,0xC3,0x63,0x33,0x1E,0xFE,0xF8,0x00, 0x00,0x07,0x1F,0x1E,0x33,0x31,0x30,0x30,0x30,0x18,0x1F,0x07,0x00},	//   (  5)  0 - 0x30 Digit Zero
	{0x00,0x00,0x00,0x0C,0x0C,0x0E,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x30,0x30,0x30,0x3F,0x3F,0x30,0x30,0x30,0x00,0x00},	//   (  6)  1 - 0x31 Digit One
	{0x00,0x1C,0x1E,0x07,0x03,0x03,0x83,0xC3,0xE3,0x77,0x3E,0x1C,0x00, 0x00,0x30,0x38,0x3C,0x3E,0x37,0x33,0x31,0x30,0x30,0x30,0x30,0x00},	//   (  7)  2 - 0x32 Digit Two
	{0x00,0x0C,0x0E,0x07,0xC3,0xC3,0xC3,0xC3,0xC3,0xE7,0x7E,0x3C,0x00, 0x00,0x0C,0x1C,0x38,0x30,0x30,0x30,0x30,0x30,0x39,0x1F,0x0E,0x00},	//   (  8)  3 - 0x33 Digit Three
	{0x00,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x07,0xFF,0xFF,0x00,0x00,0x00, 0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x3F,0x3F,0x03,0x03,0x00},	//   (  9)  4 - 0x34 Digit Four
	{0x00,0x3F,0x7F,0x63,0x63,0x63,0x63,0x63,0x63,0xE3,0xC3,0x83,0x00, 0x00,0x0C,0x1C,0x38,0x30,0x30,0x30,0x30,0x30,0x38,0x1F,0x0F,0x00},	//   ( 10)  5 - 0x35 Digit Five
	{0x00,0x00,0xC0,0xF0,0xF8,0xDC,0xCE,0xC7,0xC3,0xC3,0xC3,0x80,0x00, 0x00,0x07,0x0F,0x1F,0x39,0x30,0x30,0x30,0x30,0x30,0x39,0x1F,0x0F},	//   ( 11)  6 - 0x36 Digit Six
	{0x00,0x03,0x03,0x03,0x03,0x03,0x03,0xC3,0xF3,0x3F,0x0F,0x03,0x00, 0x00,0x00,0x00,0x00,0x30,0x3C,0x0F,0x03,0x00,0x00,0x00,0x00,0x00},	//   ( 12)  7 - 0x37 Digit Seven
	{0x00,0xBC,0xFE,0xE7,0xC3,0xC3,0xC3,0xE7,0xFE,0xBC,0x00,0x00,0x00, 0x0F,0x1F,0x39,0x30,0x30,0x30,0x30,0x30,0x39,0x1F,0x0F,0x00,0x00},	//   ( 13)  8 - 0x38 Digit Eight
	{0x00,0x3C,0x7E,0xE7,0xC3,0xC3,0xC3,0xC3,0xC3,0xE7,0xFE,0xFC,0x00, 0x00,0x00,0x00,0x30,0x30,0x30,0x38,0x1C,0x0E,0x07,0x03,0x00,0x00} 	//   ( 14)  9 - 0x39 Digit Nine
};


void WriteDATA(unsigned char dataToWrite)
{
DATA_PORT = dataToWrite;
CONTROL_PORT &= ~(DC);
CONTROL_PORT |= (CS|RES|RD|WR);
CONTROL_PORT |= (DC);
CONTROL_PORT &= ~(CS);
CONTROL_PORT &= ~(WR);
CONTROL_PORT |= (WR);
CONTROL_PORT &= ~(DC);
CONTROL_PORT |= (CS);
}

void WriteCMD(unsigned char commandToWrite)
{
DATA_PORT = commandToWrite;
CONTROL_PORT |= (DC|CS|WR|RES|RD);
CONTROL_PORT &= ~(DC);
CONTROL_PORT &= ~(CS);
CONTROL_PORT &= ~(WR);
CONTROL_PORT |= (WR);
CONTROL_PORT |= (DC);
CONTROL_PORT |= (CS);
}

void INIT_OLED(void)
{	
CONTROL_DIR &= ~(DC | WR | RD | CS | RES);
DATA_DIR = 0x00;
CONTROL_PORT |= (DC | WR | RD | CS | RES);
CONTROL_PORT |= (RES);
__delay_ms(10);
CONTROL_PORT &= ~ (RES);
__delay_ms(10);
CONTROL_PORT |= (RES);
__delay_ms(10);
}

unsigned char ReadData(void)
{
unsigned char tmp;
DATA_DIR = 0xFF;
CONTROL_PORT &= ~(CS);
CONTROL_PORT |= (DC | WR);
CONTROL_PORT &= ~(RD);					//First data is a dummy
CONTROL_PORT |= (RD);
CONTROL_PORT &= ~(RD);
CONTROL_PORT |= (RD);
CONTROL_PORT |= (CS);
tmp =  DATA_PIN;
DATA_DIR = 0x00;
return tmp;
}

/**
  * @brief  controls the reset line of the display
  *
  * @param	resetRelease true  = perform a full reset cycle
  *				         false = make sure that the display is released from reset
  */



/*#############################################################################
############### command functions, not for user interaction ###################
#############################################################################*/

/**
  * @brief  Set the memory addressing mode
  *
  *         For more information see SSD1309 datasheet.
  *
  * @param	mode    0 = Horizontal Addressing Mode
  *				    1 = Vertical Addressing Mode
  *				    2 = Page Addressing Mode (default)
  */
static void cmd_AddressingMode(uint8_t mode)
{
	WriteCMD(0x20);			// Set Memory Addressing Mode
	WriteCMD(mode);			//   Default => 2
}

/**
  * @brief  Set column start Address for Page Addressing Mode
  *
  *         Set the column start address register for Page Addressing Mode.
  *         The initial display line register is reset to 0x00 after RESET.
  *
  * @note   This command is only for page addressing mode.
  *
  * @param	address The start address (0-255?)
  */
static void cmd_ColumnStartAddress(uint8_t address)
{
	WriteCMD(0x00+address%16);		// Set Lower Column Start Address for Page Addressing Mode
	//   Default => 0x00
	WriteCMD(0x10+address/16);		// Set Higher Column Start Address for Page Addressing Mode
	//   Default => 0x10
}

/**
  * @brief  Set Page Start Address for Page Addressing Mode
  *
  *         Set GDDRAM Page Start Address (PAGE0~PAGE7) for Page Addressing Mode.
  *
  * @note   This command is only for page addressing mode.
  *
  * @param	page The start page (0-7)
  */
static void cmd_PageStartAddress(uint8_t page)
{
	WriteCMD(0xB0|page);			// Set Page Start Address for Page Addressing Mode
}

/**
  * @brief  Setup column start and end address.
  *
  * @note   This command is only for horizontal or vertical addressing mode.
  *
  * @param	start Column start address (0-127)
  * @para,  end	  Column end address (0-127)
  */
//static void cmd_ColumnAddress(uint8_t start, uint8_t end)
//{
	//WriteCMD(0x21);			// Set Column Address
	//WriteCMD(start);			//   Default => 0 (Column Start Address)
	//WriteCMD(end);				//   Default => 131 (Column End Address)
//}

/**
  * @brief  Setup page start and end address.
  *
  * @note   This command is only for horizontal or vertical addressing mode.
  *
  * @param	start Page start Address (0-7)
  * @para,  end	  Page end Address (0-7)
  */
//static void cmd_PageAddress(uint8_t start, uint8_t end)
//{
	//WriteCMD(0x22);			// Set Page Address
	//WriteCMD(start);			//   Default => 0 (Page Start Address)
	//WriteCMD(end);				//   Default => 7 (Page End Address)
//}

/**
  * @brief  Set display RAM display start line register.
  *
  * @param	line Display start line (0-63)
  */
static void cmd_StartLine(uint8_t line)
{
	WriteCMD(0x40|line);			// Set Display Start Line
	//   Default => 64 (0x00)
}

/**
  * @brief  set display contrast
  *
  *         Set the display contrast to a specific value.
  *
  * @param	contrast contrast value 0-255
  */
static void cmd_ContrastControl(uint8_t contrast)
{
	WriteCMD(0x81);			// Set Contrast Control for Bank 0
	WriteCMD(contrast);		// 
}

/**
  * @brief  Set Segment Remap
  *
  * @param	remap false: Column Address 0 Mapped to SEG0
  *               true:  Column Address 0 Mapped to SEG131
  */
static void cmd_SegmentRemap(bool remap)
{
	if (remap)
	{
		WriteCMD(0xA1);	// 0xA1 => Column Address 0 Mapped to SEG131
	} 
	else
	{
		WriteCMD(0xA0);	// 0xA0 => Column Address 0 Mapped to SEG0
	}
}

/**
  * @brief  Set COM Output Scan Direction
  *
  * @param	remap false: Scan from COM0 to 63
  *               true:  Scan from COM63 to 0
  */
static void cmd_ComRemap(bool remap)
{
	if (remap)
	{
		WriteCMD(0xC8);	// 0xC8 (0x08) => Scan from COM63 to 0
	}
	else
	{
		WriteCMD(0xC0);	// 0xC0 (0x00) => Scan from COM0 to 63
	}
}

/**
  * @brief  Display Test
  *
  *         Turns all pixel of the display on.
  *
  * @param	on  all pixel on if true, normal operation if false
  */
static void cmd_EntireDisplayON(bool on)
{
	if (on)
	{
		WriteCMD(0xA5);	//     0xA5 => Entire Display On
	} 
	else
	{
		WriteCMD(0xA4);	//     0xA4 => Normal Display
	}
}

/**
  * @brief  Normal/Inverse Display
  *
  * @param	inverse Inverse display content if true
  */
static void cmd_InverseDisplay(bool inverse)
{
	if (inverse)
	{
		WriteCMD(0xA7);	//     0xA7 => Inverse Display On
	}
	else
	{
		WriteCMD(0xA6);	//     0xA6 => Normal Display
	}
}

/**
  * @brief  Set Multiplex Ratio
  *
  * @param	mux	Multiplex Ratio (16-64)
  */
static void cmd_MultiplexRatio(uint8_t mux)
{
	WriteCMD(0xA8);			// Set Multiplex Ratio
	WriteCMD(mux-1);			//   Default => 64 (1/64 Duty)
}

/**
  * @brief  Turn Display on
  *
  *         Activate the display or go into sleep mode
  *
  * @param	on  display on if true, sleep when false
  */
static void cmd_DisplayOn(bool on)
{
	if (on)
	{
		WriteCMD(0xAF);	//     0xAF => Display On
	}
	else
	{
		WriteCMD(0xAE);	//     0xAE => Display Off
	}
}

/**
  * @brief  Set Display Offset
  *
  * @param	offset  Set vertical shift by COM (0-63)
  */
static void cmd_DisplayOffset(uint8_t offset)
{
	WriteCMD(0xD3);			// Set Display Offset
	WriteCMD(offset);		//   Default => 0
}

/**
  * @brief  Set Display Clock Divide Ratio/Oscillator Frequency
  *
  * @param	value see SSD1309 datasheet page 34
  */
static void cmd_DisplayClock(uint8_t value)
{
	WriteCMD(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	WriteCMD(value);		// Default => 0x70
	//     D[3:0] => Display Clock Divider
	//     D[7:4] => Oscillator Frequency
}

/**
  * @brief  Set Pre-charge Period
  *
  * @param	value see SSD1309 datasheet page 34
  */
static void cmd_PrechargePeriod(uint8_t value)
{
	WriteCMD(0xD9);			// Set Pre-Charge Period
	WriteCMD(value);		//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
	//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

/**
  * @brief  Set COM Pins Hardware Configuration
  *
  * @param	config  0: Sequential COM pin configuration, Disable COM Left/Right remap
  *                 1: Alternative COM pin configuration, Disable COM Left/Right remap (default)
  *                 2: Sequential COM pin configuration, Enable COM Left/Right remap
  *                 3: Alternative COM pin configuration, Enable COM Left/Right remap
  */
static void cmd_ComPins(uint8_t config)
{
	WriteCMD(0xDA);				// Set COM Pins Hardware Configuration
	WriteCMD((config<<4)|0x02);	// Default => 0x12
}

/**
  * @brief  Set VCOMH deselect Level
  *
  * @param	value see SSD1309 datasheet page 34
  */
static void cmd_Vcomh(uint8_t value)
{
	WriteCMD(0xDB);			// Set VCOMH deselect Level
	WriteCMD(value);		// Default => 52 (0.78*VCC)
}

/**
  * @brief  No operation command
  */
//static void cmd_NOP()
//{
	//WriteCMD(0xE3);			// Command for No Operation
//}


/*#############################################################################
################# high level functions for user interaction ###################
#############################################################################*/

void clear(void)
{
	unsigned char i,j;

	for(i=0;i<(SSD1309_ROW/8);i++)
	{
		cmd_PageStartAddress(i);
		cmd_ColumnStartAddress(0);

		for(j=0;j<SSD1309_COL;j++)
		{
			WriteDATA(0x00);
		}
	}
}

void putc(unsigned char aChar, uint8_t page, uint8_t startColumn)
{
	uint8_t i;
	
	if((aChar >= 0x20) && (aChar <= 0x7E))	// normal ASCII Char
	{
		aChar -= 0x20;						// subtract to match index
	}
	else if (aChar >= 0xA0)					// extended UTF8 char
	{
		aChar -= 0x41;						// subtract to match index
	}
	else									// char not available
	{
		aChar = '?' - 0x20;					// make a ?
	}	
	
	cmd_PageStartAddress(page);
	cmd_ColumnStartAddress(startColumn);

	for(i=0;i<5;i++)
	{
		WriteDATA(pgm_read_byte(&font[aChar][i]));
	}
	WriteDATA(0);
}

void putBigDigit(unsigned char aDigit, uint8_t page, uint8_t startColumn)
{
	uint8_t i;	
	
	if((aDigit >= '+') && (aDigit <= '9'))			// Digit is printable
	{
		cmd_PageStartAddress(page);
		cmd_ColumnStartAddress(startColumn);		
		
		for(i=0;i<13;i++)
		{
			WriteDATA(pgm_read_byte(&bigDigit[(aDigit-'+')][i]));
		}
		
		WriteDATA(0);
		WriteDATA(0);
		
		cmd_PageStartAddress(page+1);
		cmd_ColumnStartAddress(startColumn);
		
		for(i=0;i<13;i++)
		{
			WriteDATA(pgm_read_byte(&bigDigit[(aDigit-'+')][i+13]));
		}
		
		WriteDATA(0);
		WriteDATA(0);		
	}
	else								// make a whitespace
	{
		cmd_PageStartAddress(page);
		cmd_ColumnStartAddress(startColumn);		
		
		for(i=0;i<15;i++)
		{
			WriteDATA(0);
		}
		
		cmd_PageStartAddress(page+1);
		cmd_ColumnStartAddress(startColumn);
		
		for(i=0;i<15;i++)
		{
			WriteDATA(0);
		}
	}		
}


void print(char *aString, uint8_t page, uint8_t startColumn)
{
	char * Src_Pointer = aString;			// copy the pointer, because will will increment it
		
	while(*Src_Pointer != '\0')				// until string end
	{
		putc((unsigned char)*Src_Pointer, page, startColumn);
		Src_Pointer++;
		startColumn+=6;
	}
}

void print_bigDigit(char *aString, uint8_t page, uint8_t startColumn)
{
	char * Src_Pointer = aString;			// copy the pointer, because will will increment it
		
	while(*Src_Pointer != '\0')				// until string end
	{
		putBigDigit((unsigned char)*Src_Pointer, page, startColumn);
		Src_Pointer++;
		startColumn+=15;
	}
}

void print_P(const char* aString, uint8_t page, uint8_t startColumn)
{
	uint8_t tmp;
	const char* Src_Pointer = aString;			// copy the pointer, because will will increment it	
	
	tmp = pgm_read_byte(Src_Pointer);
	while(tmp != '\0')
	{
		putc(tmp, page, startColumn);
		Src_Pointer++;
		tmp = pgm_read_byte(Src_Pointer);
		startColumn+=6;
	}
}

void printf(uint8_t page, uint8_t startColumn, const char* __fmt, ...)
{
	char aString[64];
	va_list argumentlist;
	va_start(argumentlist, __fmt);
	sprintf(aString, __fmt, argumentlist);
	va_end(argumentlist);
	print(aString, page, startColumn);
}

void printf_P(uint8_t page, uint8_t startColumn, const char* __fmt, ...)
{
	char aString[64];	
	va_list argumentlist;
	va_start(argumentlist, __fmt);
	vsprintf(aString, __fmt, argumentlist);
	va_end(argumentlist);
	print(aString, page, startColumn);
}

void showPic(const uint8_t *pic, uint8_t startPage, uint8_t endPage, uint8_t startCol, uint8_t totalCol)
{
	uint8_t i,j;
	
	for(i=startPage;i<=endPage;i++)
	{
		cmd_PageStartAddress(i);
		cmd_ColumnStartAddress(startCol);

		for(j=0;j<totalCol;j++)
		{
			WriteDATA(pgm_read_byte(pic+i*totalCol+j));
		}
	}
}

void drawBargraph(uint8_t percent, uint8_t startPage, uint8_t endPage, uint8_t startCol, uint8_t totalCol)
{
	uint8_t i, j, bar, dummy;
	
	uint8_t height = ((endPage-startPage)*8+8);
	
	if (percent > 100)
	{
		percent = 100;
	}
		
	if (totalCol < 5)		// a bar have to be at least 5 pixels wide
	{
		return;
	}
		
	if (totalCol > height)	// with > height -> horizontal bar graph
	{
		// calculate the bar with
		bar = (uint8_t)(((uint16_t)(totalCol-4) * (uint16_t)percent + 50) / 100);		
		for(i=startPage;i<=endPage;i++)
		{
			cmd_PageStartAddress(i);
			cmd_ColumnStartAddress(startCol);			
			WriteDATA(0xFF);			
			if (endPage==startPage)
			{
				WriteDATA(0x81);
				for(j=0;j<bar;j++)
				{
					WriteDATA(0xBD);
				}
				for(j=bar;j<(totalCol-3);j++)
				{
					WriteDATA(0x81);
				}
			} 
			else if (i==startPage)
			{
				WriteDATA(0x01);
				for(j=0;j<bar;j++)
				{
					WriteDATA(0xFD);
				}
				for(j=bar;j<(totalCol-3);j++)
				{
					WriteDATA(0x01);
				}
			}
			else if (i==endPage)
			{
				WriteDATA(0x80);
				for(j=0;j<bar;j++)
				{
					WriteDATA(0xBF);
				}
				for(j=bar;j<(totalCol-3);j++)
				{
					WriteDATA(0x80);
				}
			}
			else
			{
				WriteDATA(0x00);
				for(j=0;j<bar;j++)
				{
					WriteDATA(0xFF);
				}
				for(j=bar;j<(totalCol-3);j++)
				{
					WriteDATA(0x00);
				}
			}			
			WriteDATA(0xFF);
		}
	} 
	else										// height >= with -> vertical bar graph
	{
		if (startPage==endPage)	// a vertical bar have to be higher than one page!
		{
			return;
		}		
		// calculate the bar height
		bar = (uint8_t)(((uint16_t)(height-4) * (uint16_t)percent + 50) / 100);		
		for(i=startPage;i<=endPage;i++)
		{
			cmd_PageStartAddress(i);
			cmd_ColumnStartAddress(startCol);			
			WriteDATA(0xFF);			
			if (i==startPage)
			{
				WriteDATA(0x01);
				dummy = height - 4 - bar;
				if (dummy > 6)
				{
					dummy = 6;
				}
				dummy = (0xFC<<dummy) | 0x01;
				for(j=2;j<(totalCol-2);j++)
				{
					WriteDATA(dummy);
				}
				WriteDATA(0x01);				
			} 
			else if (i==endPage)
			{
				WriteDATA(0x80);
				if (bar >= 6)
				{
					dummy = 0;
				}
				else
				{
					dummy = 6-bar;
				}
				dummy = (0xFF<<dummy) & 0xBF;
				for(j=2;j<(totalCol-2);j++)
				{
					WriteDATA(dummy);
				}
				WriteDATA(0x80);
			} 
			else
			{
				WriteDATA(0x00);
				dummy = (endPage - i) * 8 + 6;
				if (bar >= dummy)
				{
					dummy = 0;
				}
				else if ((bar + 8) <= dummy)
				{
					dummy = 8;
				}
				else
				{
					dummy = dummy - bar;
				}
				dummy = 0xFF<<dummy;
				for(j=2;j<(totalCol-2);j++)
				{
					WriteDATA(dummy);
				}
				WriteDATA(0x00);
			}	
			WriteDATA(0xFF);			
		}
	}	
}

void Oled_Init(void)
{	
	INIT_OLED();					// Init hardware Interface
	
	cmd_DisplayOn(false);				// Display Off
	cmd_DisplayClock(0xF0);				// Set Clock as 80 Frames/Sec
	cmd_MultiplexRatio(SSD1309_ROW);	// 1/64 Duty (16-64)
	cmd_DisplayOffset(0);				// Shift Mapping RAM Counter (0~63)
	cmd_StartLine(0);					// Set Mapping RAM Display Start Line (0~63)
	cmd_AddressingMode(2);				// Set Page Addressing Mode
	cmd_SegmentRemap(false);			// standard segment mapping
	cmd_ComRemap(false);				// standard col mapping
	cmd_ComPins(1);						// default com pins
	cmd_ContrastControl(10);			// Set SEG Output Current //Brightness
	cmd_PrechargePeriod(0xF1);			// Set Pre-Charge as 16 Clocks & Discharge as 1 Clocks  
	cmd_Vcomh(55);						// Set VCOM Deselect Level //55
	cmd_EntireDisplayON(false);			// Disable Entire Display On
	cmd_InverseDisplay(false);			// Disable Inverse Display
    clear();			          		// Clear Screen
    cmd_DisplayOn(true);				// Display On
}


#define COMSCANDIREC1	0xC8 //Direc Up to Down
#define RE_MAP1			0xA1 //Column adress 131 mapped to SEG0 (Direc Right to Left)
void orientation_down_up_left_right(void){
WriteCMD(COMSCANDIREC1);
WriteCMD(RE_MAP1);
}
void SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
unsigned char temp;  
GoTo(x, y); 
temp = ReadData(); 
GoTo(x, y);
  if(color) {
     temp = temp | (1 << (y % 8));
     WriteDATA(temp); 
  }else{
	 temp = temp & ~(1 << (y % 8));
	 WriteDATA(temp); 
	}  
}


void GoTo(unsigned char x,unsigned char y)
{
	y=y/8;
	WriteCMD(SETSTARTCOLUMN);
	WriteCMD(x);
	WriteCMD(SCREEN_WIDTH-1);
	WriteCMD(SET_PAGE+y);
}


void ssd1309_showPic(const uint8_t *pic, uint8_t startPage, uint8_t endPage, uint8_t startCol, uint8_t totalCol)
{
	uint8_t i,j;
	
	for(i=startPage;i<=endPage;i++)
	{
		cmd_PageStartAddress(i);
		cmd_ColumnStartAddress(startCol);

		for(j=0;j<totalCol;j++)
		{
			WriteDATA(pgm_read_byte(pic+i*totalCol+j));
		}
	}
}
