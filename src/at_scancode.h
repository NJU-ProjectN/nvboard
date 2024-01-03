#ifndef _VFPGA_AT_SCANCODE_H
#define _VFPGA_AT_SCANCODE_H

#include <macro.h>

#define AT_SCANCODE_A	0x1C
#define AT_SCANCODE_B	0x32
#define AT_SCANCODE_C	0x21
#define AT_SCANCODE_D	0x23
#define AT_SCANCODE_E	0x24
#define AT_SCANCODE_F	0x2B
#define AT_SCANCODE_G	0x34
#define AT_SCANCODE_H	0x33
#define AT_SCANCODE_I	0x43
#define AT_SCANCODE_J	0x3B
#define AT_SCANCODE_K	0x42
#define AT_SCANCODE_L	0x4B
#define AT_SCANCODE_M	0x3A
#define AT_SCANCODE_N	0x31
#define AT_SCANCODE_O	0x44
#define AT_SCANCODE_P	0x4D
#define AT_SCANCODE_Q	0x15
#define AT_SCANCODE_R	0x2D
#define AT_SCANCODE_S	0x1B
#define AT_SCANCODE_T	0x2C
#define AT_SCANCODE_U	0x3C
#define AT_SCANCODE_V	0x2A
#define AT_SCANCODE_W	0x1D
#define AT_SCANCODE_X	0x22
#define AT_SCANCODE_Y	0x35
#define AT_SCANCODE_Z	0x1A
#define AT_SCANCODE_0	0x45
#define AT_SCANCODE_1	0x16
#define AT_SCANCODE_2	0x1E
#define AT_SCANCODE_3	0x26
#define AT_SCANCODE_4	0x25
#define AT_SCANCODE_5	0x2E
#define AT_SCANCODE_6	0x36
#define AT_SCANCODE_7	0x3D
#define AT_SCANCODE_8	0x3E
#define AT_SCANCODE_9	0x46

#define AT_SCANCODE_GRAVE	0x0E
#define AT_SCANCODE_MINUS 0x4E
#define AT_SCANCODE_EQUALS 0x55
#define AT_SCANCODE_BACKSLASH 0x5D
#define AT_SCANCODE_BACKSPACE 0x66
#define AT_SCANCODE_SPACE 0x29
#define AT_SCANCODE_TAB	0x0D
#define AT_SCANCODE_CAPSLOCK 0x58
#define AT_SCANCODE_LSHIFT	0x12
#define AT_SCANCODE_LCTRL	0x14
#define AT_SCANCODE_LGUI	0xE0,0x1F
#define AT_SCANCODE_LALT	0x11
#define AT_SCANCODE_RSHIFT 0x59
#define AT_SCANCODE_RCTRL 0xE0,0x14
#define AT_SCANCODE_RGUI	0xE0,0x27
#define AT_SCANCODE_RALT	0xE0,0x11
#define AT_SCANCODE_APPLICATION 0xE0,0x2F
#define AT_SCANCODE_RETURN	0x5A
#define AT_SCANCODE_ESCAPE	0x76
#define AT_SCANCODE_F1	0x5
#define AT_SCANCODE_F2	0x6
#define AT_SCANCODE_F3	0x4
#define AT_SCANCODE_F4	0x0C
#define AT_SCANCODE_F5	0x3
#define AT_SCANCODE_F6	0x0B
#define AT_SCANCODE_F7	0x83
#define AT_SCANCODE_F8	0x0A
#define AT_SCANCODE_F9	0x1
#define AT_SCANCODE_F10 0x9
#define AT_SCANCODE_F11 0x78
#define AT_SCANCODE_F12 0x7
#define AT_SCANCODE_PRINTSCREEN 0xE0,0x12
// SCRN	E0,7C (?)

/*SDL_SCANCODE_PAUSE	E1,14,77, E1,F0,14, F0,77  not supported */

#define AT_SCANCODE_SCROLLLOCK	0x7E
#define AT_SCANCODE_LEFTBRACKET	0x54
#define AT_SCANCODE_INSERT	0xE0,0x70
#define AT_SCANCODE_HOME	0xE0,0x6C
#define AT_SCANCODE_PAGEUP	0xE0,0x7D
#define AT_SCANCODE_DELETE	0xE0,0x71
#define AT_SCANCODE_END	0xE0,0x69
#define AT_SCANCODE_PAGEDOWN	0xE0,0x7A
#define AT_SCANCODE_UP	0xE0,0x75
#define AT_SCANCODE_LEFT	0xE0,0x6B
#define AT_SCANCODE_DOWN	0xE0,0x72
#define AT_SCANCODE_RIGHT	0xE0,0x74
#define AT_SCANCODE_NUMLOCKCLEAR	0x77
#define AT_SCANCODE_KP_DIVIDE	0xE0,0x4A
#define AT_SCANCODE_KP_MULTIPLY	0x7C
#define AT_SCANCODE_KP_MINUS	0x7B
#define AT_SCANCODE_KP_PLUS	0x79
#define AT_SCANCODE_KP_ENTER 0xE0,0x5A
#define AT_SCANCODE_KP_PERIOD	0x71
#define AT_SCANCODE_KP_0	0x70
#define AT_SCANCODE_KP_1	0x69
#define AT_SCANCODE_KP_2	0x72
#define AT_SCANCODE_KP_3	0x7A
#define AT_SCANCODE_KP_4	0x6B
#define AT_SCANCODE_KP_5	0x73
#define AT_SCANCODE_KP_6	0x74
#define AT_SCANCODE_KP_7	0x6C
#define AT_SCANCODE_KP_8	0x75
#define AT_SCANCODE_KP_9	0x7D
#define AT_SCANCODE_RIGHTBRACKET 0x5B
#define AT_SCANCODE_SEMICOLON	0x4C
#define AT_SCANCODE_APOSTROPHE	0x52
#define AT_SCANCODE_COMMA	0x41
#define AT_SCANCODE_PERIOD	0x49
#define AT_SCANCODE_SLASH	0x4A

#define MAP(c, f) c(f)
#define AT_PREFIX(a) concat(AT_SCANCODE, a)
#define SDL_PREFIX(a) concat(SDL_SCANCODE, a)


// count arguments
#define NARGS(...) NARGS_(__VA_ARGS__, 2, 2, 2, 1, 0)
#define NARGS_(_4, _3, _2, _1, N, ...) N
#define GET_FIRST(...) GET_ELEM(1, _, __VA_ARGS__)
#define GET_SECOND(...) GET_ELEM(NARGS(__VA_ARGS__), _, __VA_ARGS__ ,,,,,,,,,,,)

#define GET_ELEM(N, ...) concat(GET_ELEM_, N)(__VA_ARGS__)
#define GET_ELEM_0(_0, ...) _0
#define GET_ELEM_1(_0, _1, ...) _1
#define GET_ELEM_2(_0, _1, _2, ...) _2
#define GET_ELEM_3(_0, _1, _2, _3, ...) _3
#define GET_ELEM_4(_0, _1, _2, _3, _4, ...) _4

#define SCANCODE_LIST(f) \
f(_A) f(_B) f(_C) f(_D) f(_E) f(_F) f(_G) f(_H) f(_I) f(_J) f(_K) f(_L) f(_M) \
f(_N) f(_O) f(_P) f(_Q) f(_R) f(_S) f(_T) f(_U) f(_V) f(_W) f(_X) f(_Y) f(_Z) \
f(_0) f(_1) f(_2) f(_3) f(_4) f(_5) f(_6) f(_7) f(_8) f(_9) \
f(_GRAVE) f(_MINUS) f(_EQUALS) f(_BACKSLASH) f(_BACKSPACE) f(_SPACE) f(_TAB) f(_CAPSLOCK) \
f(_LSHIFT) f(_LCTRL) f(_LGUI) f(_LALT) f(_RSHIFT) f(_RCTRL) f(_RGUI) f(_RALT) \
f(_APPLICATION) f(_RETURN) f(_ESCAPE) \
f(_F1) f(_F2) f(_F3) f(_F4) f(_F5) f(_F6) f(_F7) f(_F8) f(_F9) f(_F10) f(_F11) f(_F12) \
f(_PRINTSCREEN) f(_SCROLLLOCK) f(_LEFTBRACKET) f(_INSERT) f(_HOME) f(_PAGEUP) f(_DELETE) f(_END) \
f(_PAGEDOWN) f(_UP) f(_LEFT) f(_DOWN) f(_RIGHT) f(_NUMLOCKCLEAR) \
f(_KP_DIVIDE) f(_KP_MULTIPLY) f(_KP_MINUS) f(_KP_PLUS) f(_KP_ENTER) f(_KP_PERIOD) \
f(_KP_0) f(_KP_1) f(_KP_2) f(_KP_3) f(_KP_4) f(_KP_5) f(_KP_6) f(_KP_7) f(_KP_8) f(_KP_9) \
f(_RIGHTBRACKET) f(_SEMICOLON) f(_APOSTROPHE) f(_COMMA) f(_PERIOD) f(_SLASH)


#endif