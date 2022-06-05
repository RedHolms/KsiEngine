#pragma once
#include "KsiMain.hpp"

#include <windows.h>

KSI_START

enum VKey : uint8_t {
   KSIVKEY_LBUTTON           = 0x01,
   KSIVKEY_RBUTTON           = 0x02,
   KSIVKEY_CANCEL            = 0x03,
   KSIVKEY_MBUTTON           = 0x04,    /* NOT contiguous with L & RBUTTON */

#if _WIN32_WINNT >= 0x0500
   KSIVKEY_XBUTTON1          = 0x05,    /* NOT contiguous with L & RBUTTON */
   KSIVKEY_XBUTTON2          = 0x06,    /* NOT contiguous with L & RBUTTON */
#endif

   KSIVKEY__RES000           = 0x07,

   KSIVKEY_BACK              = 0x08,
   KSIVKEY_TAB               = 0x09,

   KSIVKEY__RES001           = 0x0A,
   KSIVKEY__RES002           = 0x0B,

   KSIVKEY_CLEAR             = 0x0C,
   KSIVKEY_RETURN            = 0x0D,

   KSIVKEY_SHIFT             = 0x10,
   KSIVKEY_CONTROL           = 0x11,
   KSIVKEY_MENU              = 0x12,
   KSIVKEY_PAUSE             = 0x13,
   KSIVKEY_CAPITAL           = 0x14,

   KSIVKEY_KANA              = 0x15,
   KSIVKEY_IME_ON            = 0x16,
   KSIVKEY_JUNJA             = 0x17,
   KSIVKEY_FINAL             = 0x18,
   KSIVKEY_HANJA             = 0x19,
   KSIVKEY_KANJI             = 0x19,
   KSIVKEY_IME_OFF           = 0x1A,

   KSIVKEY_ESCAPE            = 0x1B,

   KSIVKEY_CONVERT           = 0x1C,
   KSIVKEY_NONCONVERT        = 0x1D,
   KSIVKEY_ACCEPT            = 0x1E,
   KSIVKEY_MODECHANGE        = 0x1F,

   KSIVKEY_SPACE             = 0x20,
   KSIVKEY_PRIOR             = 0x21,
   KSIVKEY_NEXT              = 0x22,
   KSIVKEY_END               = 0x23,
   KSIVKEY_HOME              = 0x24,
   KSIVKEY_LEFT              = 0x25,
   KSIVKEY_UP                = 0x26,
   KSIVKEY_RIGHT             = 0x27,
   KSIVKEY_DOWN              = 0x28,
   KSIVKEY_SELECT            = 0x29,
   KSIVKEY_PRINT             = 0x2A,
   KSIVKEY_EXECUTE           = 0x2B,
   KSIVKEY_SNAPSHOT          = 0x2C,
   KSIVKEY_INSERT            = 0x2D,
   KSIVKEY_DELETE            = 0x2E,
   KSIVKEY_HELP              = 0x2F,

   KSIVKEY_LWIN              = 0x5B,
   KSIVKEY_RWIN              = 0x5C,
   KSIVKEY_APPS              = 0x5D,

   KSIVKEY__RES003           = 0x5E,

   KSIVKEY_SLEEP             = 0x5F,

   KSIVKEY_NUMPAD0           = 0x60,
   KSIVKEY_NUMPAD1           = 0x61,
   KSIVKEY_NUMPAD2           = 0x62,
   KSIVKEY_NUMPAD3           = 0x63,
   KSIVKEY_NUMPAD4           = 0x64,
   KSIVKEY_NUMPAD5           = 0x65,
   KSIVKEY_NUMPAD6           = 0x66,
   KSIVKEY_NUMPAD7           = 0x67,
   KSIVKEY_NUMPAD8           = 0x68,
   KSIVKEY_NUMPAD9           = 0x69,
   KSIVKEY_MULTIPLY          = 0x6A,
   KSIVKEY_ADD               = 0x6B,
   KSIVKEY_SEPARATOR         = 0x6C,
   KSIVKEY_SUBTRACT          = 0x6D,
   KSIVKEY_DECIMAL           = 0x6E,
   KSIVKEY_DIVIDE            = 0x6F,
   KSIVKEY_F1                = 0x70,
   KSIVKEY_F2                = 0x71,
   KSIVKEY_F3                = 0x72,
   KSIVKEY_F4                = 0x73,
   KSIVKEY_F5                = 0x74,
   KSIVKEY_F6                = 0x75,
   KSIVKEY_F7                = 0x76,
   KSIVKEY_F8                = 0x77,
   KSIVKEY_F9                = 0x78,
   KSIVKEY_F10               = 0x79,
   KSIVKEY_F11               = 0x7A,
   KSIVKEY_F12               = 0x7B,
   KSIVKEY_F13               = 0x7C,
   KSIVKEY_F14               = 0x7D,
   KSIVKEY_F15               = 0x7E,
   KSIVKEY_F16               = 0x7F,
   KSIVKEY_F17               = 0x80,
   KSIVKEY_F18               = 0x81,
   KSIVKEY_F19               = 0x82,
   KSIVKEY_F20               = 0x83,
   KSIVKEY_F21               = 0x84,
   KSIVKEY_F22               = 0x85,
   KSIVKEY_F23               = 0x86,
   KSIVKEY_F24               = 0x87,

#if(_WIN32_WINNT >= 0x0604)

// UI navigation
   KSIVKEY_NAVIGATION_VIEW        = 0x88, // reserved
   KSIVKEY_NAVIGATION_MENU        = 0x89, // reserved
   KSIVKEY_NAVIGATION_UP          = 0x8A, // reserved
   KSIVKEY_NAVIGATION_DOWN        = 0x8B, // reserved
   KSIVKEY_NAVIGATION_LEFT        = 0x8C, // reserved
   KSIVKEY_NAVIGATION_RIGHT       = 0x8D, // reserved
   KSIVKEY_NAVIGATION_ACCEPT      = 0x8E, // reserved
   KSIVKEY_NAVIGATION_CANCEL      = 0x8F, // reserved

#endif

   KSIVKEY_NUMLOCK           = 0x90,
   KSIVKEY_SCROLL            = 0x91,

// NEC PC-9800 kbd definitions
   KSIVKEY_OEM_NEC_EQUAL     = 0x92,   // '=' key on numpad

// Fujitsu/OASYS kbd definitions
   KSIVKEY_OEM_FJ_JISHO      = 0x92,   // 'Dictionary' key
   KSIVKEY_OEM_FJ_MASSHOU    = 0x93,   // 'Unregister word' key
   KSIVKEY_OEM_FJ_TOUROKU    = 0x94,   // 'Register word' key
   KSIVKEY_OEM_FJ_LOYA       = 0x95,   // 'Left OYAYUBI' key
   KSIVKEY_OEM_FJ_ROYA       = 0x96,   // 'Right OYAYUBI' key

/*
   L* & R* - left and right Alt, Ctrl and Shift virtual keys.
   Used only as parameters to GetAsyncKeyState() and GetKeyState().
   No other API or message will distinguish left and right keys in this way.
*/
   KSIVKEY_LSHIFT            = 0xA0,
   KSIVKEY_RSHIFT            = 0xA1,
   KSIVKEY_LCONTROL          = 0xA2,
   KSIVKEY_RCONTROL          = 0xA3,
   KSIVKEY_LMENU             = 0xA4,
   KSIVKEY_RMENU             = 0xA5,

#if _WIN32_WINNT >= 0x0500

   KSIVKEY_BROWSER_BACK           = 0xA6,
   KSIVKEY_BROWSER_FORWARD        = 0xA7,
   KSIVKEY_BROWSER_REFRESH        = 0xA8,
   KSIVKEY_BROWSER_STOP           = 0xA9,
   KSIVKEY_BROWSER_SEARCH         = 0xAA,
   KSIVKEY_BROWSER_FAVORITES      = 0xAB,
   KSIVKEY_BROWSER_HOME           = 0xAC,

   KSIVKEY_VOLUME_MUTE            = 0xAD,
   KSIVKEY_VOLUME_DOWN            = 0xAE,
   KSIVKEY_VOLUME_UP              = 0xAF,
   KSIVKEY_MEDIA_NEXT_TRACK       = 0xB0,
   KSIVKEY_MEDIA_PREV_TRACK       = 0xB1,
   KSIVKEY_MEDIA_STOP             = 0xB2,
   KSIVKEY_MEDIA_PLAY_PAUSE       = 0xB3,
   KSIVKEY_LAUNCH_MAIL            = 0xB4,
   KSIVKEY_LAUNCH_MEDIA_SELECT    = 0xB5,
   KSIVKEY_LAUNCH_APP1            = 0xB6,
   KSIVKEY_LAUNCH_APP2            = 0xB7,

#endif

   KSIVKEY__RES004           = 0xB8,
   KSIVKEY__RES005           = 0xB9,

   KSIVKEY_OEM_1             = 0xBA,   // ';:' for US
   KSIVKEY_OEM_PLUS          = 0xBB,   // '+' any country
   KSIVKEY_OEM_COMMA         = 0xBC,   // ',' any country
   KSIVKEY_OEM_MINUS         = 0xBD,   // '-' any country
   KSIVKEY_OEM_PERIOD        = 0xBE,   // '.' any country
   KSIVKEY_OEM_2             = 0xBF,   // '/?' for US
   KSIVKEY_OEM_3             = 0xC0,   // '`~' for US

   KSIVKEY__RES006           = 0xC1,
   KSIVKEY__RES007           = 0xC2,

#if _WIN32_WINNT >= 0x0604

// Gamepad input
   KSIVKEY_GAMEPAD_A                            = 0xC3, // reserved
   KSIVKEY_GAMEPAD_B                            = 0xC4, // reserved
   KSIVKEY_GAMEPAD_X                            = 0xC5, // reserved
   KSIVKEY_GAMEPAD_Y                            = 0xC6, // reserved
   KSIVKEY_GAMEPAD_RIGHT_SHOULDER               = 0xC7, // reserved
   KSIVKEY_GAMEPAD_LEFT_SHOULDER                = 0xC8, // reserved
   KSIVKEY_GAMEPAD_LEFT_TRIGGER                 = 0xC9, // reserved
   KSIVKEY_GAMEPAD_RIGHT_TRIGGER                = 0xCA, // reserved
   KSIVKEY_GAMEPAD_DPAD_UP                      = 0xCB, // reserved
   KSIVKEY_GAMEPAD_DPAD_DOWN                    = 0xCC, // reserved
   KSIVKEY_GAMEPAD_DPAD_LEFT                    = 0xCD, // reserved
   KSIVKEY_GAMEPAD_DPAD_RIGHT                   = 0xCE, // reserved
   KSIVKEY_GAMEPAD_MENU                         = 0xCF, // reserved
   KSIVKEY_GAMEPAD_VIEW                         = 0xD0, // reserved
   KSIVKEY_GAMEPAD_LEFT_THUMBSTICK_BUTTON       = 0xD1, // reserved
   KSIVKEY_GAMEPAD_RIGHT_THUMBSTICK_BUTTON      = 0xD2, // reserved
   KSIVKEY_GAMEPAD_LEFT_THUMBSTICK_UP           = 0xD3, // reserved
   KSIVKEY_GAMEPAD_LEFT_THUMBSTICK_DOWN         = 0xD4, // reserved
   KSIVKEY_GAMEPAD_LEFT_THUMBSTICK_RIGHT        = 0xD5, // reserved
   KSIVKEY_GAMEPAD_LEFT_THUMBSTICK_LEFT         = 0xD6, // reserved
   KSIVKEY_GAMEPAD_RIGHT_THUMBSTICK_UP          = 0xD7, // reserved
   KSIVKEY_GAMEPAD_RIGHT_THUMBSTICK_DOWN        = 0xD8, // reserved
   KSIVKEY_GAMEPAD_RIGHT_THUMBSTICK_RIGHT       = 0xD9, // reserved
   KSIVKEY_GAMEPAD_RIGHT_THUMBSTICK_LEFT        = 0xDA, // reserved

#endif

   KSIVKEY_OEM_4             = 0xDB,  //  '[{' for US
   KSIVKEY_OEM_5             = 0xDC,  //  '\|' for US
   KSIVKEY_OEM_6             = 0xDD,  //  ']}' for US
   KSIVKEY_OEM_7             = 0xDE,  //  ''"' for US
   KSIVKEY_OEM_8             = 0xDF,

   KSIVKEY__RES008           = 0xE0,

// Various extended or enhanced keyboards
   KSIVKEY_OEM_AX            = 0xE1,  //  'AX' key on Japanese AX kbd
   KSIVKEY_OEM_102           = 0xE2,  //  "<>" or "\|" on RT 102-key kbd.
   KSIVKEY_ICO_HELP          = 0xE3,  //  Help key on ICO
   KSIVKEY_ICO_00            = 0xE4,  //  00 key on ICO

#if WINVER >= 0x0400
   KSIVKEY_PROCESSKEY        = 0xE5,
#endif

   KSIVKEY_ICO_CLEAR         = 0xE6,


#if _WIN32_WINNT >= 0x0500
   KSIVKEY_PACKET            = 0xE7,
#endif

// Nokia/Ericsson definitions
   KSIVKEY_OEM_RESET         = 0xE9,
   KSIVKEY_OEM_JUMP          = 0xEA,
   KSIVKEY_OEM_PA1           = 0xEB,
   KSIVKEY_OEM_PA2           = 0xEC,
   KSIVKEY_OEM_PA3           = 0xED,
   KSIVKEY_OEM_WSCTRL        = 0xEE,
   KSIVKEY_OEM_CUSEL         = 0xEF,
   KSIVKEY_OEM_ATTN          = 0xF0,
   KSIVKEY_OEM_FINISH        = 0xF1,
   KSIVKEY_OEM_COPY          = 0xF2,
   KSIVKEY_OEM_AUTO          = 0xF3,
   KSIVKEY_OEM_ENLW          = 0xF4,
   KSIVKEY_OEM_BACKTAB       = 0xF5,

   KSIVKEY_ATTN              = 0xF6,
   KSIVKEY_CRSEL             = 0xF7,
   KSIVKEY_EXSEL             = 0xF8,
   KSIVKEY_EREOF             = 0xF9,
   KSIVKEY_PLAY              = 0xFA,
   KSIVKEY_ZOOM              = 0xFB,
   KSIVKEY_NONAME            = 0xFC,
   KSIVKEY_PA1               = 0xFD,
   KSIVKEY_OEM_CLEAR         = 0xFE,

   KSIVKEY__MAX
};

KSI_END